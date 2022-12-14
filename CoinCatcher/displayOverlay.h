#pragma once
#include "gameUtils.h"

void displayScoreText(GLfloat xPos, GLfloat yPos);
void displayBestScoreText(GLfloat xPos, GLfloat yPos);
void displayTimer(GLfloat xPos, GLfloat yPos);
void displayMenu();
void displayHelp();
void displayPauseScreen();
void displayGameOver(int score, bool isBest);