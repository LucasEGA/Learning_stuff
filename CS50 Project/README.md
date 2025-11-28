# Chatify — CS50 Final Project

### Video Demo  
https://youtu.be/__________________

---

## Description

**Chatify** is a lightweight, clean, and functional web-based chat application built with **Flask**, **Jinja templates**, **SQLite**, and **custom CSS**.  
It allows users to:

- Create an account  
- Log in securely  
- Open private chats with any other registered user  
- Exchange messages that are permanently stored in a relational database  

The primary aim was to build a real, functional messaging system that goes beyond basic CRUD operations and integrates multiple concepts taught in CS50: authentication, sessions, SQL, template inheritance, and stateful user interaction.

---

## Distinctiveness and Complexity

Chatify is distinct from the examples in the course because it implements a full **private 1-to-1 chat system** with:

- Secure user authentication (`generate_password_hash`, `check_password_hash`)
- Session-based login persistence
- Automatic creation of new chat rooms between user pairs
- Organized database structure across three related tables  
- Dynamic chat rendering (“me” vs “them” message bubbles)
- Safe message escaping (`|e`) to prevent injected HTML
- A fully custom frontend (layout, typography, nav bar, message boxes, responsive layout)

This project combines:

- Backend logic  
- Database design  
- Security considerations  
- User interface development  
- Template logic  

Overall, the system demonstrates architectural planning that goes beyond any previous problem set.

---

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


---

## Files

### **app.py**
Main Flask application file. Handles:

- Registering users  
- Logging in and logging out  
- Session management  
- Opening chats between two users  
- Creating a chat if it does not yet exist  
- Inserting new messages into the database  
- Reading and displaying messages in correct order  
- Passing variables to templates (`partner_name`, `messages`, `me_username`, etc.)

---

### **layout.html**
Base template extended by all pages.

Contains:

- Navigation bar  
- Flash message display  
- Link to stylesheet  
- A centered `<main>` container  
- Jinja `{% block content %}` for child page content

---

### **index.html**
Landing page.

- Left side: title and subtitle  
- Right side: image (`Photo_.png`)  
- Uses `app-card`, `app-row`, `text-side`, `image-side` classes for layout

---

### **register.html**
Form for creating a user account.

Fields:

- `username`
- `password`
- `confirmation`

Submits via POST to `/register`.

---

### **login.html**
Login form with:

- `username`
- `password`

Submits via POST to `/login`.

---

### **chat.html**
Main chat interface.

Includes:

1. **Open Chat Form**  
   Enter the username of the person you want to chat with.

2. **Conversation Display**  
   Messages are shown as:

   - `.msg.me` for messages sent by the logged-in user  
   - `.msg.them` for messages from the other user  

3. **Send Message Form**  
   Sends a POST request to insert a new message.

Messages are safely escaped (`{{ m['body']|e }}`), preventing HTML injection.

---

### **stylesheet.css**
Provides the styling for:

- Navigation bar  
- Centered layout (`page-wrapper`)  
- Buttons and forms  
- Flash messages  
- Responsive two-column layout  
- Message bubbles (`.msg.me`, `.msg.them`)  
- The index landing page layout  

---

## Database Design (`.db`)

Chatify stores all data in a single SQLite database file.

### **users**
Stores all registered user accounts.

| Field     | Type                  | Description                       |
|-----------|------------------------|-----------------------------------|
| id        | INTEGER PRIMARY KEY    | Unique user ID                    |
| username  | TEXT UNIQUE NOT NULL   | Account username                  |
| hash      | TEXT NOT NULL          | Password hash                     |

---

### **chats**
Represents a private chat between exactly two users.

| Field   | Type                  | Description                          |
|---------|------------------------|--------------------------------------|
| id      | INTEGER PRIMARY KEY    | Chat room ID                         |
| user_1  | INTEGER NOT NULL       | First participant                    |
| user_2  | INTEGER NOT NULL       | Second participant                   |

There is at most one row for each pair of users.

---

### **messages**
Stores all messages exchanged between users.

| Field      | Type                  | Description                                 |
|------------|------------------------|---------------------------------------------|
| id         | INTEGER PRIMARY KEY    | Message ID                                  |
| chat_id    | INTEGER NOT NULL       | Reference to `chats.id`                     |
| sender     | INTEGER NOT NULL       | ID of the user who sent the message         |
| body       | TEXT NOT NULL          | Message content                             |
| created_at | TEXT NOT NULL          | Timestamp (string)                          |

Messages are loaded and rendered in chronological order.

---

## How to Run

1. **Clone the repository**

```bash
git clone https://github.com/YOUR_USERNAME/YOUR_REPO.git
cd YOUR_REPO





