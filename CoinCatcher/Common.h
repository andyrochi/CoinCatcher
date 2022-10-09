#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
extern GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
extern GLfloat playerRadius, playerSpeed, playerFasterSpeed;
void displayText(char ch[], GLfloat xpos, GLfloat ypos);
void displayScore(int number, GLfloat xpos, GLfloat ypos);