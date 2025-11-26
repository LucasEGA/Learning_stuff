Flask Chat Application
A lightweight and clean one-to-one chat application built with Flask, SQLite, and a simple modern UI.
Users can register, log in, and start private conversations with any other user.

🚀 Features
-User registration & login
-Secure password hashing
-One-to-one chat system
-Messages stored in SQLite
-Clean, modern UI (HTML + CSS)
-Flash messages for feedback
-Lightweight and easy to extend

📦 Installation
1. Install dependencies
   pip install flask werkzeug flask-login
2. Run the application
   python app.py
3. Open in browser
   http://127.0.0.1:5000

📁 Project Structure
app.py
data.db
/static/
    stylesheet.css
/templates/
    layout.html
    chat.html
    login.html
    register.html
    index.html

🛠️ Technologies Used
Flask – backend framework
SQLite – persistent message storage
Werkzeug – password hashing
Jinja2 – templating engine
CSS3 – custom styling

🔒 Security
Passwords securely hashed
Sessions handled by Flask
No plaintext passwords stored


🌱 Extendability
The project is intentionally simple and can be extended with:
Real-time chat (WebSockets)
User avatars
Typing indicators
Message reactions
Profile pages
Group chats
