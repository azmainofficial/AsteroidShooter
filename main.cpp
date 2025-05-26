#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>

#define PI 3.1415926535

// Structures
struct Bullet {
    float x, y, angle;
};

struct Asteroid {
    float x, y, dx, dy, size;
    std::vector<float> offsets;
    float color[3];
};

struct Blast {
    float x, y;
    float size;
    int timer;
};

float shipX = 0.0f, shipY = -0.8f, shipAngle = 90.0f;
std::vector<Bullet> bullets;
std::vector<Asteroid> asteroids;
std::vector<Blast> blasts;

int score = 0, lives = 3;
bool isPaused = false, gameOver = false, gameStarted = false;
int countdown = 3, wave = 1;

float asteroidSpeed = 0.0015f;
int spawnRate = 50;
int frameCounter = 0;

// Function Prototypes
void drawShip(), spawnAsteroid(), restartGame(), drawUI();
void drawBlast(const Blast& b), drawAsteroid(const Asteroid& a);
void drawString(float x, float y, const std::string& str);

// Draw Functions
void drawShip() {
    glPushMatrix();
    glTranslatef(shipX, shipY, 0);
    glRotatef(shipAngle, 0, 0, 1);
    glBegin(GL_TRIANGLES);
    glColor3f(0.7f, 0.9f, 1.0f);
    glVertex2f(0.04f, 0.0f);
    glVertex2f(-0.02f, -0.03f);
    glVertex2f(-0.02f, 0.03f);
    glEnd();
    glPopMatrix();
}

void drawBullet(float x, float y) {
    glColor3f(1, 1, 0);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void drawAsteroid(const Asteroid& a) {
    glColor3f(a.color[0], a.color[1], a.color[2]);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 8; ++i) {
        float angle = i * 2 * PI / 8;
        float r = a.size * a.offsets[i];
        glVertex2f(a.x + r * cos(angle), a.y + r * sin(angle));
    }
    glEnd();
}

void drawBlast(const Blast& b) {
    glColor4f(1.0f, 1.0f, 0.0f, (float)b.timer / 20.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(b.x, b.y);
    for (int i = 0; i <= 20; ++i) {
        float angle = i * 2 * PI / 20;
        glVertex2f(b.x + b.size * cos(angle), b.y + b.size * sin(angle));
    }
    glEnd();
}

void drawString(float x, float y, const std::string& str) {
    glRasterPos2f(x, y);
    for (char c : str)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

// Game Logic
void spawnAsteroid() {
    Asteroid a;
    a.x = ((rand() % 200) - 100) / 100.0f;
    a.y = 1.2f;
    a.dx = ((rand() % 100) - 50) / 10000.0f;
    a.dy = -((rand() % 30) + 10) * asteroidSpeed;
    a.size = ((rand() % 20) + 10) / 100.0f;
    for (int i = 0; i < 8; ++i)
        a.offsets.push_back(0.8f + static_cast<float>(rand() % 40) / 100.0f);
    a.color[0] = (rand() % 100) / 100.0f;
    a.color[1] = (rand() % 100) / 100.0f;
    a.color[2] = (rand() % 100) / 100.0f;
    asteroids.push_back(a);
}

void update(int) {
    if (gameOver || !gameStarted || isPaused) {
        glutTimerFunc(16, update, 0);
        return;
    }

    if (countdown > 0) {
        glutTimerFunc(1000, update, 0);
        countdown--;
        return;
    }

    for (auto& b : bullets) {
        b.x += 0.02f * cos(b.angle * PI / 180);
        b.y += 0.02f * sin(b.angle * PI / 180);
    }

    for (auto& a : asteroids) {
        a.x += a.dx;
        a.y += a.dy;
    }

    for (auto& b : blasts) b.timer--;

    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [](Bullet& b) { return b.x < -1 || b.x > 1 || b.y < -1 || b.y > 1; }), bullets.end());

    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
        [](Asteroid& a) { return a.y < -1.2f; }), asteroids.end());

    blasts.erase(std::remove_if(blasts.begin(), blasts.end(),
        [](Blast& b) { return b.timer <= 0; }), blasts.end());

    for (int i = 0; i < bullets.size(); ++i) {
        for (int j = 0; j < asteroids.size(); ++j) {
            float dx = bullets[i].x - asteroids[j].x;
            float dy = bullets[i].y - asteroids[j].y;
            float dist = sqrt(dx * dx + dy * dy);
            if (dist < asteroids[j].size) {
                if (asteroids[j].size > 0.03f) {
                    for (int k = 0; k < 2; ++k) {
                        Asteroid split = asteroids[j];
                        split.size /= 1.5f;
                        split.dx = -split.dx + ((rand() % 20) - 10) / 10000.0f;
                        split.dy = -split.dy + ((rand() % 20) - 10) / 10000.0f;
                        split.offsets.clear();
                        for (int i = 0; i < 8; ++i)
                            split.offsets.push_back(0.8f + static_cast<float>(rand() % 40) / 100.0f);
                        asteroids.push_back(split);
                    }
                }

                blasts.push_back({asteroids[j].x, asteroids[j].y, asteroids[j].size * 1.5f, 20});
                bullets.erase(bullets.begin() + i);
                asteroids.erase(asteroids.begin() + j);
                score += 10;
                goto skip;
            }
        }
    skip:;
    }

    for (int i = 0; i < asteroids.size(); ++i) {
        float dx = shipX - asteroids[i].x;
        float dy = shipY - asteroids[i].y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < asteroids[i].size) {
            lives--;
            blasts.push_back({asteroids[i].x, asteroids[i].y, asteroids[i].size * 2.0f, 20});
            asteroids.erase(asteroids.begin() + i);
            if (lives <= 0) gameOver = true;
            break;
        }
    }

    frameCounter++;
    if (frameCounter % spawnRate == 0)
        spawnAsteroid();

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void drawUI() {
    glColor3f(1, 1, 1);
    drawString(-0.95f, 0.9f, "Score: " + std::to_string(score));
    drawString(0.7f, 0.9f, "Lives: " + std::to_string(lives));
    drawString(-0.2f, 0.9f, "Wave: " + std::to_string(wave));

    if (!gameStarted)
        drawString(-0.2f, 0.0f, "Press ENTER to Start");

    if (countdown > 0)
        drawString(-0.05f, 0.1f, std::to_string(countdown));

    if (isPaused)
        drawString(-0.2f, -0.2f, "Game Paused (P to Resume)");

    if (gameOver)
        drawString(-0.3f, 0.0f, "Game Over - Press R to Restart");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawShip();
    for (auto& b : bullets) drawBullet(b.x, b.y);
    for (auto& a : asteroids) drawAsteroid(a);
    for (auto& b : blasts) drawBlast(b);
    drawUI();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
    if (key == ' ') {
        Bullet b;
        b.x = shipX + 0.05f * cos(shipAngle * PI / 180);
        b.y = shipY + 0.05f * sin(shipAngle * PI / 180);
        b.angle = shipAngle;
        bullets.push_back(b);
    } else if (key == 'p' || key == 'P') {
        isPaused = !isPaused;
    } else if (key == 'r' || key == 'R') {
        restartGame();
    } else if (key == 13 && !gameStarted) {
        gameStarted = true;
        countdown = 3;
    }
}

void specialKeys(int key, int, int) {
    if (key == GLUT_KEY_LEFT)
        shipAngle += 10;
    else if (key == GLUT_KEY_RIGHT)
        shipAngle -= 10;
}

void restartGame() {
    lives = 3;
    score = 0;
    gameOver = false;
    gameStarted = false;
    bullets.clear();
    asteroids.clear();
    blasts.clear();
    shipAngle = 90.0f;
    countdown = 3;
}

void init() {
    glClearColor(0, 0, 0.1f, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1, 1, -1, 1);
    srand(time(0));
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Asteroids Shooter - Azmain 41220200226");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
