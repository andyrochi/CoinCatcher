#include "Common.h"
#include <cstdio>

void displayText(char ch[], GLfloat xpos, GLfloat ypos) {
    int numofchar = strlen(ch);
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