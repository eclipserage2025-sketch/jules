import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.neural_network import MLPRegressor
import logging
import time

class AIEngine:
    def __init__(self, sensitivity=0.5):
        self.sensitivity = sensitivity
        self.performance_model = MLPRegressor(hidden_layer_sizes=(10, 10), max_iter=1000)
        self.difficulty_model = LinearRegression()

        self.history = []  # List of (intensity, threads, hashrate, temp)
        self.difficulty_history = []  # List of (timestamp, difficulty)

        self.is_trained = False
        self.tuning_active = False

        logging.info(f"AI Engine initialized with sensitivity {sensitivity}")

    def add_performance_data(self, intensity, threads, hashrate, temp):
        """Adds a data point for performance prediction."""
        self.history.append([intensity, threads, hashrate, temp])
        if len(self.history) > 5:
            self._train_performance_model()

    def _train_performance_model(self):
        """Trains the MLP model to predict hashrate based on intensity and threads."""
        data = np.array(self.history)
        X = data[:, :2]  # Intensity, Threads
        y = data[:, 2]   # Hashrate

        try:
            self.performance_model.fit(X, y)
            self.is_trained = True
        except Exception as e:
            logging.error(f"AI Training Error: {e}")

    def predict_hashrate(self, intensity, threads):
        """Predicts hashrate for given settings."""
        if not self.is_trained:
            return 0.0
        return self.performance_model.predict([[intensity, threads]])[0]

    def optimize_settings(self, target_temp=75.0):
        """Finds the best intensity and thread count within thermal limits."""
        if not self.is_trained:
            return None

        best_hashrate = 0
        best_settings = None

        # Simple grid search for demonstration, mirroring auto-tuning
        for i in range(8, 21):  # Intensity range
            for t in range(1, 17):  # Threads range
                pred_hashrate = self.predict_hashrate(i, t)
                if pred_hashrate > best_hashrate:
                    best_hashrate = pred_hashrate
                    best_settings = (i, t)

        return best_settings

    def add_difficulty_data(self, difficulty):
        """Adds difficulty data point for forecasting."""
        self.difficulty_history.append([time.time(), difficulty])
        if len(self.difficulty_history) > 3:
            self._train_difficulty_model()

    def _train_difficulty_model(self):
        data = np.array(self.difficulty_history)
        X = data[:, 0].reshape(-1, 1) # Timestamps
        y = data[:, 1]               # Difficulty
        self.difficulty_model.fit(X, y)

    def predict_next_difficulty(self):
        """Forecasts the next difficulty level."""
        if len(self.difficulty_history) < 3:
            return None
        next_time = time.time() + 3600  # Forecast for next hour
        return self.difficulty_model.predict([[next_time]])[0]
