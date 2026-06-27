import socket, os, time

HOST = os.environ.get("SERVER_HOST", "koth_py_srv2")
PORT = int(os.environ.get("SERVER_PORT", 5002))
TAG  = f"[Py-Client→{HOST}:{PORT}]"

def get_score():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(5)
            s.connect((HOST, PORT))
            data = s.recv(1024).decode()
            print(f"{TAG} {data}", flush=True)
    except Exception as e:
        print(f"{TAG} Error: {e}", flush=True)

if __name__ == "__main__":
    print(f"{TAG} Starting...", flush=True)
    while True:
        get_score()
        time.sleep(10)
