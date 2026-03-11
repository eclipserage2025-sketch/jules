import json
import logging
import time
import sys
import os
from src.stratum_client import StratumClient
from src.mining_core import MiningCore
from src.ai_engine import AIEngine

def setup_logging():
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s [%(levelname)s] %(message)s',
        handlers=[
            logging.FileHandler("miner.log"),
            logging.StreamHandler(sys.stdout)
        ]
    )

def load_config():
    with open('config.json', 'r') as f:
        return json.load(f)

def main():
    setup_logging()
    logging.info("Starting AI Python Miner for Litecoin...")

    try:
        config = load_config()
    except Exception as e:
        logging.error(f"Failed to load config: {e}")
        return

    # Initialize Components
    ai_engine = AIEngine(sensitivity=config.get("ai_sensitivity", 0.5))
    stratum = StratumClient(config)
    miner = MiningCore(config)

    # Start Stratum Connection
    if not stratum.connect():
        logging.error("Critical failure: Could not connect to pool.")
        return

    # Start Mining Workers
    miner.start(stratum)

    logging.info("Miner is running. Press Ctrl+C to stop.")

    try:
        while True:
            time.sleep(10)

            # AI Logic: Feed data to engine
            current_hashrate = miner.get_hashrate()
            current_intensity = miner.intensity
            current_threads = miner.threads

            # In a real scenario, we'd get temperature from a hardware monitoring library
            dummy_temp = 65.0 + (current_intensity * 0.5)

            ai_engine.add_performance_data(current_intensity, current_threads, current_hashrate, dummy_temp)

            # Difficulty Forecasting
            if stratum.difficulty:
                ai_engine.add_difficulty_data(stratum.difficulty)
                next_diff = ai_engine.predict_next_difficulty()
                if next_diff:
                    logging.info(f"AI Forecast: Next difficulty expected around {next_diff:.2f}")

            # AI Optimization
            if ai_engine.is_trained:
                best_settings = ai_engine.optimize_settings(target_temp=75.0)
                if best_settings:
                    new_intensity, new_threads = best_settings
                    if new_intensity != current_intensity or new_threads != current_threads:
                        logging.info("AI Optimization: Adjusting settings for better performance.")
                        miner.update_settings(new_intensity, new_threads)

    except KeyboardInterrupt:
        logging.info("Stopping miner...")
    finally:
        miner.stop()
        logging.info("Miner stopped.")

if __name__ == "__main__":
    main()
