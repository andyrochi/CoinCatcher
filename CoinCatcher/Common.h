#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
enum GameStatus {PLAYING, PAUSED, GAMEOVER};
extern GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
extern GLfloat playerRadius, playerSpeed, playerFasterSpeed;
extern size_t GAME_TIME;
extern GameStatus GAME_STATUS;

void displayText(char ch[], GLfloat xpos, GLfloat ypos);
void displayScore(int number, GLfloat xpos, GLfloat ypos);
void displayTextBig(char ch[], GLfloat xpos, GLfloat ypos);