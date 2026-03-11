import socket
import json
import hashlib
import binascii
import threading
import time
import logging
from collections import deque

class StratumClient:
    def __init__(self, config):
        self.config = config
        self.url = config['pool_url']
        self.wallet = config['wallet_address']
        self.worker_name = config.get('worker_name', 'python_worker')
        self.failover_pools = config.get('failover_pools', [])

        self.sock = None
        self.rpc_id = 1
        self.connected = False

        self.current_job = None
        self.extranonce1 = None
        self.extranonce2_size = None

        self.target = 0x0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
        self.difficulty = 1.0

        self.next_nonce = 0
        self.lock = threading.Lock()

        self.running = False

    def connect(self):
        try:
            # Parse URL
            host = self.url.replace('stratum+tcp://', '').split(':')[0]
            port = int(self.url.split(':')[-1])

            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((host, port))
            self.sock.settimeout(10)
            self.connected = True
            self.running = True

            logging.info(f"Connected to pool: {self.url}")

            # Subscribe
            self._send_rpc("mining.subscribe", [self.worker_name])

            # Authorize
            self._send_rpc("mining.authorize", [self.wallet, "x"])

            # Start listener
            threading.Thread(target=self._listen, daemon=True).start()
            return True
        except Exception as e:
            logging.error(f"Connection error: {e}")
            if self.failover_pools:
                logging.info("Attempting failover...")
                # Logic to switch to failover pool
            return False

    def _send_rpc(self, method, params):
        if not self.sock: return
        request = {
            "id": self.rpc_id,
            "method": method,
            "params": params
        }
        self.sock.sendall((json.dumps(request) + "\n").encode())
        self.rpc_id += 1

    def _listen(self):
        buffer = ""
        while self.running:
            try:
                data = self.sock.recv(1024).decode()
                if not data: break

                buffer += data
                while "\n" in buffer:
                    line, buffer = buffer.split("\n", 1)
                    if line:
                        self._handle_message(json.loads(line))
            except Exception as e:
                logging.error(f"Listener error: {e}")
                break
        self.connected = False
        logging.info("Disconnected from pool")

    def _handle_message(self, msg):
        method = msg.get("method")
        result = msg.get("result")

        if method == "mining.notify":
            params = msg.get("params")
            self._handle_notify(params)
        elif method == "mining.set_difficulty":
            self.difficulty = msg.get("params")[0]
            # Calculate target
            # For Scrypt: target = (0xFFFF << 208) / difficulty
            self.target = int(0x00000000ffff0000000000000000000000000000000000000000000000000000 / self.difficulty)
            logging.info(f"Pool set difficulty: {self.difficulty}")
        elif msg.get("id") == 1: # Subscribe result
            self.extranonce1 = result[1]
            self.extranonce2_size = result[2]
            logging.info(f"Subscribed. Extranonce1: {self.extranonce1}")

    def _handle_notify(self, params):
        job_id, prevhash, coinb1, coinb2, merkle_branch, version, nbits, ntime, clean_jobs = params

        # Build header (simplified)
        # In a real miner, we'd assemble the block header properly
        # For this prototype, we'll store the parts
        with self.lock:
            self.current_job = {
                "job_id": job_id,
                "prevhash": prevhash,
                "coinb1": coinb1,
                "coinb2": coinb2,
                "merkle_branch": merkle_branch,
                "version": version,
                "nbits": nbits,
                "ntime": ntime,
                "target": self.target,
                "header": binascii.unhexlify(prevhash) # Placeholder for real header construction
            }
            self.next_nonce = 0
            logging.info(f"New job received: {job_id}")

    def get_current_job(self):
        with self.lock:
            return self.current_job

    def get_current_job_id(self):
        with self.lock:
            return self.current_job["job_id"] if self.current_job else None

    def get_next_nonce_range(self, count):
        with self.lock:
            start = self.next_nonce
            self.next_nonce += count
            return start

    def submit_share(self, job_id, nonce):
        # mining.submit("username", "job_id", "extranonce2", "ntime", "nonce")
        # For simplicity, extranonce2 is just zeros
        extranonce2 = "00" * self.extranonce2_size
        ntime = self.current_job["ntime"]
        nonce_hex = binascii.hexlify(nonce.to_bytes(4, 'big')).decode()

        self._send_rpc("mining.submit", [self.wallet, job_id, extranonce2, ntime, nonce_hex])
        logging.info(f"Share submitted for job {job_id}")
