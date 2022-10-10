#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
enum GameStatus {PLAYING, PAUSED, GAMEOVER, STARTMENU};
extern GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;
extern GLfloat playerRadius, playerSpeed, playerFasterSpeed;
extern size_t GAME_TIME;
extern GameStatus GAME_STATUS;

void setGameTime(size_t value = 90);
void decGameTime(size_t value = 5);

void displayText(char ch[], GLfloat xpos, GLfloat ypos);
void displayScore(int number, GLfloat xpos, GLfloat ypos);
void displayTextBig(char ch[], GLfloat xpos, GLfloat ypos);
void displayTextMedium(char ch[], GLfloat xpos, GLfloat ypos);
void displayTextSmall(char ch[], GLfloat xpos, GLfloat ypos);