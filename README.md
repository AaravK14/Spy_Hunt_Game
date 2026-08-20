# 🕵️ Spy Hunt

A multiplayer social-deduction game built with a **C++ backend and HTML/CSS/JavaScript frontend**, where players receive secret words, give hints, discuss suspicious players, and vote to identify the hidden spy.

The game uses a custom C++ HTTP server to handle game logic and communicates with the web interface through API endpoints.

## 🎮 Overview

Spy Hunt is a multiplayer word-based deduction game.

At the beginning of each round, every player receives a word while the hidden **Spy** receives a different but related word.

Players then:

1. Receive their secret word.
2. Give a hint related to their word.
3. Discuss the hints and identify suspicious players.
4. Vote for the player they believe is the Spy.
5. Reveal the result and update game statistics.

The project combines **Object-Oriented Programming, networking, game logic, timers, and web development** into a single application.

## ✨ Features

* 🕵️ Random Spy selection
* 🎲 Random word-pair generation
* 🌍 Multiple themes including Animals, Food, Places and Objects
* ⏱️ Timed game phases
* 💡 Player hint system
* 💬 Discussion phase
* 🗳️ Voting system
* 📊 Game statistics
* 🏆 Spy and player win tracking
* 🌐 Browser-based interface
* 🔌 C++ HTTP server
* 🔄 Frontend-backend API communication
* 📱 Responsive UI using Tailwind CSS

## 🎯 Game Flow

```text
        Start Game
             ↓
      Select Players
             ↓
       Select Theme
             ↓
      Assign Word Pair
             ↓
      Random Spy Selected
             ↓
       Word Reveal Phase
             ↓
         Hint Phase
             ↓
      Discussion Phase
             ↓
        Voting Phase
             ↓
       Reveal Results
             ↓
      Update Statistics
```

## 🏗️ System Architecture

```text
┌─────────────────────────────┐
│       Web Frontend          │
│   HTML + JavaScript         │
│       Tailwind CSS          │
└──────────────┬──────────────┘
               │
               │ HTTP Requests
               │
               ▼
┌─────────────────────────────┐
│       C++ HTTP Server       │
│         Winsock             │
├─────────────────────────────┤
│     Spy Hunt Game Engine    │
├─────────────────────────────┤
│ Player / Spy Classes        │
│ Word Manager                │
│ Game Timer                  │
│ Voting System               │
│ Game Statistics             │
└─────────────────────────────┘
```

## 🧩 Object-Oriented Design

The backend uses several OOP concepts to structure the game engine.

### `GameEntity`

Base class representing a game entity and providing common properties and actions.

### `Player`

Extends `GameEntity` and manages:

* Player identity
* Assigned words
* Hints
* Votes
* Player actions

### `Spy`

Extends `Player` and adds Spy-specific functionality including:

* Spy word
* Suspicion tracking
* Identity reveal
* Specialized suspicion calculation

### `WordManager`

Maintains themed word pairs and randomly selects words for each game.

Available themes include:

* Animals
* Food
* Places
* Objects

### `GameTimer`

Handles timed phases of the game including:

* Word Reveal
* Hint
* Discussion
* Voting

### `SpyHuntGameEngine`

Acts as the central game controller and manages:

* Players
* Spy selection
* Game phases
* Word assignment
* Hints
* Voting
* Results
* Statistics

### `SimpleHTTPServer`

Provides the communication layer between the browser frontend and the C++ game engine using HTTP requests.

## 🌐 Frontend

The frontend is built using:

* HTML
* JavaScript
* Tailwind CSS

The browser communicates with the C++ backend through API endpoints running locally on port `8080`.

Example endpoints include:

```text
/api/start
/api/getWord
/api/submitHint
/api/startDiscussionPhase
/api/startVotingPhase
/api/vote
```

## 🛠️ Technologies Used

| Technology   | Purpose                                    |
| ------------ | ------------------------------------------ |
| C++          | Game engine and backend                    |
| HTML         | Web interface                              |
| JavaScript   | Frontend interaction and API communication |
| Tailwind CSS | UI styling                                 |
| Winsock      | C++ HTTP networking                        |
| HTTP         | Frontend-backend communication             |
| OOP          | Game architecture and entity management    |

## 📂 Project Structure

```text
Spy-Hunt-Game/
│
├── spy_hunt_server.cpp     # C++ backend and game engine
├── spy_hunt.html           # Web frontend
├── README.md
├── .gitignore
│
├── screenshots/
│   ├── main_menu.png
│   ├── game.png
│   ├── discussion.png
│   ├── voting.png
│   └── results.png
│
└── docs/
    └── architecture.png
```

## ⚙️ Requirements

* Windows
* C++ compiler with `g++`
* Modern web browser

The backend uses **Winsock**, so the current build instructions are designed primarily for Windows.

## 🚀 How to Run

### 1. Clone the repository

```bash
git clone YOUR_REPOSITORY_URL
cd Spy-Hunt-Game
```

### 2. Compile the C++ server

Using MinGW / g++:

```bash
g++ spy_hunt_server.cpp -o spy_hunt_server -lws2_32
```

### 3. Start the server

```bash
.\spy_hunt_server.exe
```

The server runs on:

```text
http://localhost:8080
```

### 4. Open the frontend

Open:

```text
spy_hunt.html
```

in a web browser.

Make sure the C++ server is running before starting the game.

## 📸 Screenshots

### Main Menu

![Main Menu](screenshots/main_menu.png)

### Gameplay

![Gameplay](screenshots/game.png)

### Discussion Phase

![Discussion](screenshots/discussion.png)

### Voting

![Voting](screenshots/voting.png)

### Results

![Results](screenshots/results.png)

## 🧠 Concepts Demonstrated

This project demonstrates practical implementation of:

* Object-Oriented Programming
* Inheritance
* Polymorphism
* Encapsulation
* Dynamic memory management
* STL containers
* Randomization
* State-based game logic
* Timers and asynchronous frontend polling
* Client-server communication
* HTTP request handling
* JSON-based API responses
* Web frontend development

## 🚀 Future Scope

* Multiplayer support over the internet
* WebSocket-based real-time communication
* Persistent player profiles and leaderboards
* More themes and word categories
* Room creation and joining
* Improved voting and scoring mechanics
* Cross-platform server support
* Database integration for player statistics

## 👨‍💻 Project

**Spy Hunt Game**

A C++ and web-based multiplayer social deduction game developed as an Object-Oriented Programming / systems programming project.
