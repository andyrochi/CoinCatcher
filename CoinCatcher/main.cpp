#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Player.h"
#include "Coins.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

// Global variables
char title[] = "CoinCatcher";  // Windowed mode's title
int windowWidth = 640;     // Windowed mode's width
int windowHeight = 480;     // Windowed mode's height
int windowPosX = 100;      // Windowed mode's top-left corner x
int windowPosY = 100;      // Windowed mode's top-left corner y

int refreshMillis = 30;      // Refresh period in milliseconds
int dispatchIntervals[] = { 200, 300, 400, 500, 600, 700 };

Player player;
CoinDispatcher coinDispatcher;
Score scoreCount;

// Projection clipping area
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

bool fullScreenMode = false; // Full-screen or windowed mode?
bool paused = false;         // Movement paused or resumed
GLfloat xSpeedSaved, ySpeedSaved;  // To support resume

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background (clear) color to black
}

/* Callback handler for window re-paint event */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
    glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
    glLoadIdentity();              // Reset model-view matrix

    player.drawPlayer();
    coinDispatcher.collisionCheck(player, scoreCount);
    coinDispatcher.draw();

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
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Pause/resume
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
    glutSpecialFunc(specialKeys); // Register callback handler for special-key event
    glutKeyboardFunc(keyboard);   // Register callback handler for special-key event
    glutMouseFunc(mouse);   // Register callback handler for mouse event
    initGL();                     // Our own OpenGL initialization
    glutMainLoop();               // Enter event-processing loop
    return 0;
}