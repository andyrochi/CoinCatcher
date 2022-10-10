#include "Common.h"
#include "Player.h"
#include "Coins.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

// Global variables
char title[] = "CoinCatcher";  // Windowed mode's title
int windowWidth = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX = 300;      // Windowed mode's top-left corner x
int windowPosY = 300;      // Windowed mode's top-left corner y

int refreshMillis = 30;      // Refresh period in milliseconds
int dispatchIntervals[] = { 200, 300, 400, 500, 600, 700 };
size_t GAME_TIME = 90;

// Player stats
GLfloat playerRadius = 0.1f, playerSpeed = 0.05f, playerFasterSpeed = 0.08f;
GameStatus GAME_STATUS = PAUSED;

Player player;
CoinDispatcher coinDispatcher;
ScoreManager scoreCount;

// Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

bool fullScreenMode = false; // Full-screen or windowed mode?
bool paused = false;         // Movement paused or resumed
GLfloat xSpeedSaved, ySpeedSaved;  // To support resume

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}

void displayScoreText(GLfloat xPos, GLfloat yPos) {
    char text[] = "SCORE:";
    displayText(text, xPos, yPos);
}

void displayBestScoreText(GLfloat xPos, GLfloat yPos) {
    char text[] = "BEST:";
    displayText(text, xPos, yPos);
}

void displayTimer(GLfloat xPos, GLfloat yPos) {
    char time[4];
    snprintf(time, 4, "%03ud", GAME_TIME);
    displayText(time, xPos, yPos);
}

void displayMenu() {
    char text[] = "WELCOME TO COIN CATCHER!";
    displayTextBig(text, -1.0f, 0.4f);
    char text3[] = "TRY TO SCORE AS MUCH AS YOU CAN!";
    displayTextMedium(text3, -1.0f, -0.2f);
    char text2[] = "PRESS <ENTER> TO START GAME";
    displayTextMedium(text2, -0.8f, 0.1f);
    char text4[] = "PRESS <H> FOR MORE INFO";
    displayTextMedium(text4, -0.7f, -0.5f);
}

void displayPauseScreen() {
    char text[] = "PAUSED";
    displayTextBig(text, -0.25f, 0.2f);
    char text2[] = "PRESS <SPACE> TO CONTINUE";
    displayTextMedium(text2, -0.75f, -0.1f);
}

void resetGameTime() {
    setGameTime(90);
}


/* Callback handler for window re-paint event */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
    glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
    glLoadIdentity();              // Reset model-view matrix


    if (GAME_STATUS == STARTMENU) {
        displayMenu();
    }

    if (GAME_STATUS != STARTMENU) {
        displayBestScoreText(clipAreaXLeft * 0.98f, 0.9f);
        displayScore(scoreCount.getBestScore(), clipAreaXLeft * 0.7f, 0.9f);
        displayScoreText(clipAreaXRight * 0.4f, 0.9f);
        displayScore(scoreCount.getScore(), clipAreaXRight * 0.7, 0.9f);
        displayTimer(-0.1f, 0.9f);

        player.drawPlayer();
        coinDispatcher.collisionCheck(player, scoreCount);
        coinDispatcher.draw();
    }

    if (GAME_STATUS == PAUSED) {
        displayPauseScreen();
    }
    
    if (GAME_STATUS == GAMEOVER) {
        char text[] = "TIME'S UP!";
        displayTextBig(text, -0.4f, 0.2f);
        int score = scoreCount.getScore();
        char scoreText[30];
        snprintf(scoreText, 20, "Score: %d", score);
        displayTextBig(scoreText, -0.4f, -0.1f);
        snprintf(scoreText, 30, "Press <R> to Play Again!");
        displayTextBig(scoreText, -0.9f, -0.4f);
    }

    glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
}

/* Call back when the windows is re-sized */
void reshape(GLsizei width, GLsizei height) {
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    if (width >= height) {
        clipAreaXLeft = -1.0 * aspect;
        clipAreaXRight = 1.0 * aspect;
        clipAreaYBottom = -1.0;
        clipAreaYTop = 1.0;
    }
    else {
        clipAreaXLeft = -1.0;
        clipAreaXRight = 1.0;
        clipAreaYBottom = -1.0 / aspect;
        clipAreaYTop = 1.0 / aspect;
    }
    gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
    player.updateBoundaries(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
    coinDispatcher.updateBoundaries(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
}

int getRandomInterval() {
    size_t length = sizeof(dispatchIntervals) / sizeof(dispatchIntervals[0]);
    size_t index = rand() % length;
    return dispatchIntervals[index];
}

void gameTimer(int value) {
    if (GAME_STATUS == PLAYING) {
        decGameTime(1);
    }
    glutTimerFunc(1000, gameTimer, 0);
}

void restartGame() {
    scoreCount.resetScore();
    player.reset();
    coinDispatcher.reset();
    resetGameTime();
}

/* Called back when the timer expired, used to rerender display */
void Timer(int value) {
    glutPostRedisplay();    // Post a paint request to activate display()
    glutTimerFunc(refreshMillis, Timer, 0); // subsequent timer call at milliseconds
}

void dispatchcoin(int value) {
    int nextInterval = getRandomInterval();
    glutTimerFunc(nextInterval, dispatchcoin, 0); // subsequent timer call at milliseconds
    coinDispatcher.tossObject();
}

/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:     // ESC key
        exit(0);
        break;
    case 32:
        if (GAME_STATUS == PLAYING) GAME_STATUS = PAUSED;
        else if (GAME_STATUS == PAUSED)
            GAME_STATUS = PLAYING;
        break;
    case 48: // 0
        GAME_STATUS = GAMEOVER;
        break;
    case 114:
    case 82:
        if (GAME_STATUS == GAMEOVER) {
            restartGame();
            GAME_STATUS = PLAYING;
        }
        break;
    case 13:
        if (GAME_STATUS == STARTMENU) {
            GAME_STATUS = PLAYING;
        }
        break;
    }
        
}

/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
        fullScreenMode = !fullScreenMode;         // Toggle state
        if (fullScreenMode) {                     // Full-screen mode
            windowPosX = glutGet(GLUT_WINDOW_X); // Save parameters for restoring later
            windowPosY = glutGet(GLUT_WINDOW_Y);
            windowWidth = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
        }
        else {                                         // Windowed mode
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
        }
        break;
    case GLUT_KEY_RIGHT:    // Right: Move player right
        player.moveRight();
        break;
    case GLUT_KEY_LEFT:     // Left: Move player left
        player.moveLeft();
        break;
    case GLUT_KEY_UP:       // Up: Move player up
        player.moveUp();
        break;
    case GLUT_KEY_DOWN:     // Down: Move player down
        player.moveDown();
        break;
    case GLUT_KEY_PAGE_UP:  // Page-Up: increase ball's radius
        player.scaleLarge(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
        break;
    case GLUT_KEY_PAGE_DOWN: // Page-Down: decrease ball's radius
        player.scaleSmall(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
        break;
    }
}

/* Callback handler for mouse event */
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        std::cout << "mouse clicked!" << std::endl;
    }
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {

    // initialize random seed
    srand(time(NULL));

    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // Enable double buffered mode
    glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
    glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
    glutCreateWindow(title);      // Create window with given title
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glutDisplayFunc(display);     // Register callback handler for window re-paint
    glutReshapeFunc(reshape);     // Register callback handler for window re-shape
    glutTimerFunc(0, Timer, 0);   // Refresh rate
    glutTimerFunc(1000, dispatchcoin, 0); // Dispatch timer
    glutTimerFunc(1000, gameTimer, 0);
    glutSpecialFunc(specialKeys); // Register callback handler for special-key event
    glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
    glutMouseFunc(mouse);   // Register callback handler for mouse event
    initGL();                     // Our own OpenGL initialization
    GAME_STATUS = STARTMENU;
    glutMainLoop();               // Enter event-processing loop
    return 0;
}