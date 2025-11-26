from flask import Flask, render_template, request, redirect, url_for, flash, session
from werkzeug.security import check_password_hash, generate_password_hash
import sqlite3
from flask_login import login_required
from datetime import datetime

bas_url = ""

def get_something(input):
    pass


app = Flask(__name__)
app.secret_key = "CHANGE_ME"



def get_db():
    conn = sqlite3.connect("data.db")
    conn.row_factory = sqlite3.Row
    return conn

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        username = (request.form.get("username") or "").strip()
        password = request.form.get("password") or ""

        if not username or not password:
            flash("Please fill in all fields.", "error")
            return redirect(url_for("login"))

        db = get_db()
        user = db.execute("SELECT id, username, hash FROM users WHERE username = ?", (username,)).fetchone()
        if not user or not check_password_hash(user["hash"], password):
            flash("Invalid username or password.", "error")
            return redirect(url_for("login"))

        session["user_id"] = user["id"]
        session["username"] = user["username"]
        flash("Welcome back!", "success")
        return redirect(url_for("index"))

    return render_template("login.html")

@app.route("/logout")
def logout():
    session.clear()
    flash("Logged out.", "success")
    return redirect(url_for("index"))

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        username = (request.form.get("username") or "").strip()
        password = request.form.get("password") or ""
        confirmation = request.form.get("confirmation") or ""

        if not username or not password or not confirmation:
            flash("Please fill in all fields.", "error")
            return redirect(url_for("register"))
        if password != confirmation:
            flash("Passwords do not match.", "error")
            return redirect(url_for("register"))

        pw_hash = generate_password_hash(password, method="pbkdf2:sha256")
        db = get_db()
        db.execute("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT UNIQUE NOT NULL, hash TEXT NOT NULL)")
        try:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", (username, pw_hash))
            db.commit()
        except sqlite3.IntegrityError:
            flash("Username already taken.", "error")
            return redirect(url_for("register"))

        flash("Registered! Please log in.", "success")
        return redirect(url_for("login"))
    return render_template("register.html")



@app.route("/Chatting", methods=["GET", "POST"])
def chat():
    db = get_db()

    # current user (by username and id)
    me_username = session["username"]
    me_row = db.execute(
        "SELECT id FROM users WHERE username = ?",
        (me_username,)
    ).fetchone()
    if not me_row:
        flash("Current user not found.", "error")
        return redirect(url_for("index"))
    me_id = me_row["id"]

    if request.method == "POST":
        partner_name = (request.form.get("name") or "").strip()
        text = (request.form.get("text") or "").strip()

        if not partner_name:
            flash("Please fill in all fields.", "error")
            return redirect(url_for("chat"))

        partner_row = db.execute(
            "SELECT id FROM users WHERE username = ?",
            (partner_name,)
        ).fetchone()
        if not partner_row:
            flash("Username does not exist.", "error")
            return redirect(url_for("chat"))

        partner_id = partner_row["id"]

        # Find or create chat (order-insensitive pair)
        chat_row = db.execute(
            """
            SELECT id FROM chat
            WHERE (user_1 = ? AND user_2 = ?)
               OR (user_1 = ? AND user_2 = ?)
            """,
            (me_id, partner_id, partner_id, me_id)
        ).fetchone()

        if chat_row:
            chat_id = chat_row["id"]
        else:
            cur = db.execute(
                "INSERT INTO chat (user_1, user_2, created_at) VALUES (?, ?, ?)",
                (me_id, partner_id, datetime.utcnow().isoformat())
            )
            chat_id = cur.lastrowid

        # Only insert a message if provided
        if text:
            db.execute(
                "INSERT INTO messages (chat_id, sender_id, body, created_at) VALUES (?, ?, ?, ?)",
                (chat_id, me_id, text, datetime.utcnow().isoformat())
            )

        db.commit()
        # Redirect to GET view of this chat
        return redirect(url_for("chat", name=partner_name))

    # GET: show a conversation if ?name= is provided; otherwise show empty page
    partner_name = (request.args.get("name") or "").strip()
    messages = []
    partner = None

    if partner_name:
        partner = db.execute(
            "SELECT id, username FROM users WHERE username = ?",
            (partner_name,)
        ).fetchone()

        if partner:
            partner_id = partner["id"]
            chat_row = db.execute(
                """
                SELECT id FROM chat
                WHERE (user_1 = ? AND user_2 = ?)
                   OR (user_1 = ? AND user_2 = ?)
                """,
                (me_id, partner_id, partner_id, me_id)
            ).fetchone()

            if chat_row:
                chat_id = chat_row["id"]
                # CHANGED: load rows first
                rows = db.execute(
                    """
                    SELECT m.body, m.created_at, u.username AS sender
                    FROM messages m
                    JOIN users u ON u.id = m.sender_id
                    WHERE m.chat_id = ?
                    ORDER BY m.created_at ASC, m.id ASC
                    """,
                    (chat_id,)
                ).fetchall()
                # CHANGED: format created_at as day + time string
                messages = []
                for r in rows:
                    try:
                        dt = datetime.fromisoformat(r["created_at"])
                        created_at = dt.strftime("%Y-%m-%d %H:%M")
                    except (TypeError, ValueError):
                        created_at = r["created_at"]
                    messages.append(
                        {
                            "body": r["body"],
                            "created_at": created_at,
                            "sender": r["sender"],
                        }
                    )

    return render_template(
        "chat.html",
        me_username=me_username,
        partner_name=partner_name,
        messages=messages
    )

if __name__ == "__main__":
    app.run(debug=True)
