import socket, threading, time, os
import mysql.connector

USER = "Irfan"
PORT = 5003
TAG  = f"[Py-Srv:{USER}]"

DB_CONFIG = {
    "host":     os.environ.get("DB_HOST", "koth_db"),
    "user":     os.environ.get("DB_USER", "koth_user"),
    "password": os.environ.get("DB_PASS", "koth_pass"),
    "database": os.environ.get("DB_NAME", "koth"),
}

def get_db():
    return mysql.connector.connect(**DB_CONFIG)

def update_score():
    while True:
        try:
            conn = get_db()
            cur  = conn.cursor()
            cur.execute(
                "UPDATE koth_standings "
                "SET points = points + 1, datetime_stamp = NOW() "
                "WHERE user = %s", (USER,)
            )
            conn.commit()
            cur.close(); conn.close()
            print(f"{TAG} Score updated.", flush=True)
        except Exception as e:
            print(f"{TAG} DB error: {e}", flush=True)
        time.sleep(30)

def handle_client(conn, addr):
    print(f"{TAG} Client: {addr}", flush=True)
    try:
        db  = get_db()
        cur = db.cursor()
        cur.execute(
            "SELECT user, points, datetime_stamp "
            "FROM koth_standings WHERE user = %s", (USER,)
        )
        row = cur.fetchone()
        cur.close(); db.close()
        msg = (f"User: {row[0]} | Points: {row[1]} | Last Update: {row[2]}"
               if row else "No data.")
        conn.sendall(msg.encode())
    except Exception as e:
        conn.sendall(f"Error: {e}".encode())
    finally:
        conn.close()

def main():
    threading.Thread(target=update_score, daemon=True).start()
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind(("0.0.0.0", PORT))
        s.listen(5)
        print(f"{TAG} Listening on port {PORT}", flush=True)
        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()

if __name__ == "__main__":
    print(f"{TAG} Starting...", flush=True)
    main()
