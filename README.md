# EndlessRunner


> A cyberpunk-themed 2D endless runner built in **C++** using **SFML**, developed as an Object-Oriented Programming (OOP) semester project.

---

## 📖 About

CyberRunner is a side-scrolling endless runner set in a neon-lit cyberpunk world. The player must dodge incoming obstacles, survive as long as possible, and beat their high score. The game is built from the ground up using core OOP principles — encapsulation, inheritance, polymorphism, and abstraction — making it a complete demonstration of semester-level C++ mastery.

---

## 🎮 Gameplay

- The player character runs automatically from left to right
- Obstacles (drones, barriers, etc.) approach from the right
- Press **Space / Up Arrow** to jump and avoid obstacles
- The game speed increases over time, raising the difficulty
- Your score increases the longer you survive
- A persistent **high score** is saved to `highscore.dat`

---

## ✨ Features

- Smooth 2D graphics powered by SFML
- Procedurally generated obstacles for endless replayability
- Increasing difficulty with game speed scaling
- Persistent high score system (file I/O)
- Cyberpunk-themed visual assets and atmosphere
- Clean OOP architecture separating game entities, rendering, and logic

---

## 🏗️ Project Structure

```
EndlessRunner/
├── src/                  # Source files (.cpp)
├── include/              # Header files (.h / .hpp)
├── assets/               # Sprites, fonts, sounds
├── CyberRunner.exe        # Prebuilt Windows executable
├── highscore.dat         # Persistent high score file
└── README.md
```

---

## 🧱 OOP Design

This project demonstrates the following OOP concepts:

| Concept | Application |
|---|---|
| **Classes & Objects** | `Player`, `Obstacle`, `Game`, `Background`, `ScoreManager`, etc. |
| **Encapsulation** | Private data members with public getter/setter methods |
| **Inheritance** | Game entities share a common base class (e.g., `Entity`) |
| **Polymorphism** | Virtual `update()` and `draw()` methods overridden per entity |
| **Abstraction** | Clean interfaces hiding internal rendering/physics logic |
| **File I/O** | High score persistence using file streams |

---

## 🛠️ Tech Stack

- **Language:** C++ (C++17)
- **Graphics Library:** [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)
- **Build System:** g++ / MinGW (Windows)
- **IDE:** Visual Studio Code / Code::Blocks / VS

---

## 🚀 Getting Started

### Prerequisites

- Windows OS
- SFML 2.x runtime DLLs (included or available from [sfml-dev.org](https://www.sfml-dev.org/download.php))

### Run the Prebuilt Executable

1. Clone or download this repository
2. Navigate to the project root
3. Double-click **`CyberRunner.exe`** or run from terminal:

```bash
./CyberRunner.exe
```

### Build from Source

Make sure you have g++ and SFML installed and linked.

```bash
g++ src/*.cpp -o CyberRunner -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -I include/
```

> Adjust SFML library paths as needed for your system.

---

## 🎮 Controls

| Key | Action |
|---|---|
| `Space` / `↑` | Jump |
| `Esc` | Quit / Return to Menu |
| `Enter` | Start / Restart Game |

---

## 📊 High Score

The game automatically saves your best score to `highscore.dat` in the project root. Delete this file to reset the leaderboard.

---



## 📚 Course Context

This project was submitted as a semester project demonstrating applied OOP concepts in C++, including class design, inheritance hierarchies, polymorphic behavior, and real-time game loop architecture.

---

## 📄 License

This project is for academic/educational purposes.
