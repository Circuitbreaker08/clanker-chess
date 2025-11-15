import threading
import json

with open("counter.json") as f: data = json.loads(f.read())

write_protect = threading.Lock()

def get_parameter(key: str):
    write_protect.acquire()
    out = data[key]
    data[key] += 1
    with open("counter.json", "w") as f: f.write(json.dumps(data))
    write_protect.release()
    return out