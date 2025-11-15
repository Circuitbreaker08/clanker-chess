import time

class Agent:
    def __init__(self, token):
        self.token = token
        self.heartbeat()
        self.games = []

    def heartbeat(self):
        self.expiration = int(time.time()) + 60