from flask import Flask, render_template, jsonify
import mysql.connector
import os

app = Flask(__name__)

DB_CONFIG = {
    "host":     os.environ.get("DB_HOST", "koth_db"),
    "user":     os.environ.get("DB_USER", "koth_user"),
    "password": os.environ.get("DB_PASS", "koth_pass"),
    "database": os.environ.get("DB_NAME", "koth"),
}

def get_scores():
    conn = mysql.connector.connect(**DB_CONFIG)
    cursor = conn.cursor(dictionary=True)
    cursor.execute(
        "SELECT user, points, datetime_stamp "
        "FROM koth_standings ORDER BY points DESC"
    )
    rows = cursor.fetchall()
    cursor.close()
    conn.close()
    return rows

@app.route("/")
def dashboard():
    try:
        scores = get_scores()
    except Exception as e:
        scores = []
        print(f"[WebApp] DB error: {e}", flush=True)
    return render_template("dashboard.html", scores=scores)

@app.route("/api/scores")
def api_scores():
    try:
        scores = get_scores()
        for row in scores:
            if row["datetime_stamp"]:
                row["datetime_stamp"] = str(row["datetime_stamp"])
        return jsonify(scores)
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    print("[WebApp] Starting on port 8080...", flush=True)
    app.run(host="0.0.0.0", port=8080, debug=False)
