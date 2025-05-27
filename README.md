# 🚀 Asteroids Shooter Game

A 2D arcade-style Asteroids Shooter game built using **C++** and **OpenGL GLUT**. Control a spaceship, shoot down colorful asteroids, and survive wave after wave of cosmic chaos. Developed with love and a spark of retro gaming flair by **Azmain Sheikh Rubayed (41220200226)**.

---

## 🎮 Features

- 🔺 **Rotating Spaceship**: Navigate and aim using left/right arrow keys.
- 💥 **Bullet Firing**: Press `SPACE` to fire yellow bullets.
- 🌑 **Randomized Asteroids**: Each asteroid has a unique shape, color, movement pattern, and splits upon destruction.
- 🌟 **Blast Effects**: Yellow explosion effect on asteroid destruction.
- 🎯 **Collision Detection**: Bullet-asteroid and asteroid-ship collision logic.
- 💯 **Score Tracking**: Earn points for destroying asteroids.
- ❤️ **Lives System**: Start with 3 lives. Game ends when all are lost.
- ⏸️ **Pause/Resume**: Toggle pause with `P`.
- 🔁 **Restart Option**: Press `R` to restart after game over.
- 🕒 **Countdown Start**: 3-second countdown before the game starts.
- 🔢 **Wave System**: Displays wave number as difficulty increases.

---

## 🕹️ Controls

| Key            | Action                     |
|----------------|----------------------------|
| `LEFT / RIGHT` | Rotate spaceship           |
| `SPACE`        | Shoot bullet               |
| `P`            | Pause/Resume the game      |
| `ENTER`        | Start the game             |
| `R`            | Restart after game over    |

---

## 🧠 Game Logic Overview

- **Asteroid Spawning**: New asteroids appear at intervals (`spawnRate`).
- **Splitting**: Larger asteroids split into two smaller ones when hit.
- **Blast Effects**: Visual explosion added at the collision site.
- **Edge Cleaning**: Bullets and asteroids are removed when out of bounds.

---

## 📦 Requirements

- **C++ Compiler** with OpenGL support
- **GLUT** library

---

## 🛠️ How to Compile and Run

### Linux / macOS (with g++ and GLUT installed)

```bash
g++ main.cpp -o asteroids -lGL -lGLU -lglut
./asteroids
