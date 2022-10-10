#include "Common.h"
#include <cstdio>

void setGameTime(int val) {
    GAME_TIME = val;
}

void decGameTime(int val) {
    GAME_TIME -= val;
    if (GAME_TIME <= 0) {
        GAME_TIME = 0;
        GAME_STATUS = GAMEOVER;
    }
}

void displayText(char ch[], GLfloat xpos, GLfloat ypos) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(xpos, ypos, 0.0f);  // Translate to (xPos, yPos)
    glScalef(0.0008, 0.0008, 1);
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)ch);
    glPopMatrix();
}

void displayScore(int number, GLfloat xpos, GLfloat ypos) {
    char buffer[12];
    snprintf(buffer, 12, "%06d", number);
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(xpos, ypos, 0.0f);  // Translate to (xPos, yPos)
    glScalef(0.0008, 0.0008, 1);
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)buffer);
    glPopMatrix();
}

void displayTextBig(char ch[], GLfloat xpos, GLfloat ypos) {
    int numofchar = strlen(ch);
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(xpos, ypos, 0.0f);  // Translate to (xPos, yPos)
    glScalef(0.001, 0.001, 1);
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)ch);
    glPopMatrix();
}

void displayTextMedium(char ch[], GLfloat xpos, GLfloat ypos) {
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(xpos, ypos, 0.0f);  // Translate to (xPos, yPos)
    glScalef(0.0007, 0.0007, 1);
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)ch);
    glPopMatrix();
}

void displayTextSmall(char ch[], GLfloat xpos, GLfloat ypos, TextColor color) {
    glPushMatrix();
    if (color == YELLOW)
        glColor3f(0.952f, 0.780f, 0.051f);
    else if (color == RED)
        glColor3f(0.749, 0.0f, 0.0f);
    else
        glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(xpos, ypos, 0.0f);  // Translate to (xPos, yPos)
    glScalef(0.0005, 0.0005, 1);
    glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)ch);
    glPopMatrix();
}