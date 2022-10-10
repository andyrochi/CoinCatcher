#pragma once
#include "displayOverlay.h"

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
    snprintf(time, 4, "%03d", GAME_TIME);
    displayText(time, xPos, yPos);
}

void displayMenu() {
    static int contCount = 0;
    static int mouthCount = 20;

    char text[] = "WELCOME TO COIN CATCHER!";
    displayTextBig(text, -1.0f, 0.4f);
    char text2[] = "TRY TO SCORE AS MUCH AS YOU CAN!";
    displayTextMedium(text2, -1.0f, 0.1f);
    if (contCount < 15) {
        char text3[] = "PRESS <SPACE> TO START";
        displayTextMedium(text3, -0.65f, -0.7f);
    }
    contCount++;
    contCount %= 30;
    char text4[] = "Created by Andy Chia-Hao Chang 2022";
    displayTextSmall(text4, -0.68f, -0.9f);

    // draw player
    GLfloat ballX = 0.0f, ballY = -0.3f, ballRadius = playerRadius;
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0f);  // Translate to (xPos, yPos)
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.8f, 0.0f);  // Yellow
    glVertex2f(0.0f, 0.0f);       // Center of circle
    int numSegments = 100;
    GLfloat angle;
    for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
        if (i > 20 - mouthCount) {
            angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
            glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
        }
    }
    glEnd();
    glPopMatrix();
    mouthCount += 1;
    mouthCount %= 20;
}

void displayHelp() {
    static int contCount = 0;
    char text[] = "INSTRUCTIONS";
    displayTextBig(text, -0.4f, 0.7f);
    char text2[] = "PRESS <LEFT> OR <RIGHT> TO MOVE";
    displayTextMedium(text2, -0.9f, 0.4f);
    char text3[] = "PRESS <F1> TO ENTER/QUIT FULL SCREEN";
    displayTextMedium(text3, -1.05f, -0.2f);
    char text4[] = "PRESS <SPACE> TO PAUSE GAME";
    displayTextMedium(text4, -0.8f, 0.1f);
    char text5[] = "PRESS <ESC> TO EXIT";
    displayTextMedium(text5, -0.53f, -0.5f);
    if (contCount < 15) {
        char text5[] = "PRESS <ENTER> TO START";
        displayTextMedium(text5, -0.63f, -0.8f);
    }
    contCount++;
    contCount %= 30;
}

void displayPauseScreen() {
    static int contCount = 0;
    char text[] = "PAUSED";
    displayTextBig(text, -0.25f, 0.2f);
    if (contCount < 15) {
        char text2[] = "PRESS <SPACE> TO CONTINUE";
        displayTextMedium(text2, -0.75f, -0.1f);
    }
    contCount++;
    contCount %= 30;
}

void displayGameOver(int score, bool isBest) {
    static int contCount = 0;
    char text[] = "TIME'S UP!";
    displayTextBig(text, -0.4f, 0.2f);
    char scoreText[30];
    snprintf(scoreText, 20, "SCORE: %d", score);
    displayTextBig(scoreText, -0.4f, -0.1f);
    if (isBest) {
        char newBest[] = "NEW BEST!!";
        displayTextBig(newBest, -0.4f, -0.4f);
    }
    if (contCount < 15) {
        snprintf(scoreText, 30, "PRESS <R> TO PLAY AGAIN!");
        displayTextBig(scoreText, -1.0f, -0.7f);
    }
    contCount++;
    contCount %= 30;
}