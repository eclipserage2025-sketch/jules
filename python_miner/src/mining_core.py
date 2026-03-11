import threading
import time
import logging
import hashlib
import binascii

try:
    import pyopencl as cl
    GPU_AVAILABLE = True
except ImportError:
    GPU_AVAILABLE = False

# Fallback for ltc_scrypt if not available
try:
    import ltc_scrypt
    HAS_LTC_SCRYPT = True
except ImportError:
    HAS_LTC_SCRYPT = False

class MiningCore:
    def __init__(self, config):
        self.config = config
        self.running = False
        self.hashrate = 0.0
        self.total_hashes = 0
        self.gpu_enabled = config.get("gpu_enabled", False) and GPU_AVAILABLE
        self.threads = config.get("thread_count", 4)
        self.intensity = config.get("intensity", 12)

        self.cl_ctx = None
        self.cl_queue = None
        self.cl_program = None

        if self.gpu_enabled:
            self._init_gpu()

        self._last_hash_count = 0
        self._last_time = time.time()

    def _init_gpu(self):
        try:
            platforms = cl.get_platforms()
            if not platforms:
                logging.error("No OpenCL platforms found")
                self.gpu_enabled = False
                return

            devices = platforms[0].get_devices(device_type=cl.device_type.GPU)
            if not devices:
                logging.error("No OpenCL GPU devices found")
                self.gpu_enabled = False
                return

            self.cl_ctx = cl.Context([devices[0]])
            self.cl_queue = cl.CommandQueue(self.cl_ctx)

            kernel_src = """
            __kernel void scrypt_search(__global const uchar* header, __global uint* results, uint target) {
                uint gid = get_global_id(0);
            }
            """
            self.cl_program = cl.Program(self.cl_ctx, kernel_src).build()
            logging.info("OpenCL Mining Core Initialized")
        except Exception as e:
            logging.error(f"Failed to initialize GPU: {e}")
            self.gpu_enabled = False

    def start(self, job_notifier):
        self.running = True
        self.workers = []
        self._last_time = time.time()
        self._last_hash_count = 0

        # CPU Workers
        for i in range(self.threads):
            t = threading.Thread(target=self._cpu_worker, args=(job_notifier,))
            t.daemon = True
            t.start()
            self.workers.append(t)

        # GPU Worker
        if self.gpu_enabled:
            t = threading.Thread(target=self._gpu_worker, args=(job_notifier,))
            t.daemon = True
            t.start()
            self.workers.append(t)

        # Hashrate monitor
        t = threading.Thread(target=self._hashrate_monitor)
        t.daemon = True
        t.start()
        self.workers.append(t)

        logging.info(f"Started {self.threads} CPU workers" + (" and 1 GPU worker" if self.gpu_enabled else ""))

    def stop(self):
        self.running = False
        for t in self.workers:
            t.join(timeout=1.0)

    def _cpu_worker(self, job_notifier):
        while self.running:
            job = job_notifier.get_current_job()
            if not job or 'header' not in job:
                time.sleep(0.5)
                continue

            header = job['header']
            target = job['target']
            job_id = job['job_id']

            start_nonce = job_notifier.get_next_nonce_range(500)

            for nonce in range(start_nonce, start_nonce + 500):
                if not self.running or job_notifier.get_current_job_id() != job_id:
                    break

                nonce_bytes = nonce.to_bytes(4, 'little')
                data = header + nonce_bytes

                if HAS_LTC_SCRYPT:
                    hash_result = ltc_scrypt.getPoWHash(data)
                else:
                    # Fallback to hashlib scrypt (N=1024, r=1, p=1)
                    hash_result = hashlib.scrypt(data, salt=data, n=1024, r=1, p=1, dklen=32)

                if int.from_bytes(hash_result, 'big') < target:
                    logging.info(f"Potential share found! Nonce: {nonce}")
                    job_notifier.submit_share(job_id, nonce)

                self.total_hashes += 1

    def _gpu_worker(self, job_notifier):
        while self.running:
            time.sleep(1)

    def _hashrate_monitor(self):
        while self.running:
            time.sleep(5)
            now = time.time()
            dt = now - self._last_time
            hashes = self.total_hashes - self._last_hash_count

            self.hashrate = hashes / dt if dt > 0 else 0
            self._last_time = now
            self._last_hash_count = self.total_hashes

            logging.info(f"Current Hashrate: {self.hashrate/1000:.2f} KH/s")

    def get_hashrate(self):
        return self.hashrate

    def update_settings(self, intensity, threads):
        if threads != self.threads:
            logging.info(f"Adjusting thread count to {threads}")
            self.threads = threads
        self.intensity = intensity
