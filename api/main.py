from flask import Flask, request
from flask_cors import CORS

import hashlib
import json
import os

import game
import counter
import agent

app = Flask(__name__)
CORS(app, origins="*", send_wildcard=True)

agents: list[agent.Agent] = []

@app.route("/")
def root():
    return "{\"info\": \"Online\"}"

@app.route("/admin/new_user", methods=["POST"])
def new_user():
    # why did i do this again?
    if request.form.get("password") != "RikkaTakanashi2.718":
        return
    name = request.form.get("name")
    if name in os.listdir("./data/users"):
        print("Account already exists")
    else:
        id = counter.get_parameter("id")
        os.mkdir(f"./data/users{name}")
        with open(f"./data/users{name}.json", "w") as f:
            f.write(json.dumps(
                {
                    "UUID": hashlib.sha256(bytes(id + 10271)).hexdigest()
                }
            ))

@app.route("/users")
def users():
    return os.listdir("./data/users")

@app.route("/users/<username>")
def user_profile(username):
    return f"Profile of {username}"

@app.route("/user/<username>/<agent>")
def agent_profile(username, agent):
    return f"{username}'s {agent}"

@app.route("/heartbeat")
def heartbeat():
    token = request.form.get("token")
    for agent in agents:
        if agent.token == token:
            agent.heartbeat()
            break
    else:
        agents.add(agent.Agent(token))

@app.route("/game/<game_id>")
def game(game_id):
    return "e4"

@app.route("/game/<game_id>/status")
def game_status(game_id):
    return f"{game_id} is your mom"

@app.route("/game/<game_id>/position")
def game_position():
    return "[]"

@app.route("/game/<game_id>/move")
def game_move(game_id):
    return "OK"

app.run("0.0.0.0", 7000)