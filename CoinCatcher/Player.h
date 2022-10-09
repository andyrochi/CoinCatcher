#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Math.h>

#define PI 3.14159265f
#pragma once
class Player {
public:
	Player()
		:ballRadius(0.1f), ballX(0.0f), ballY(-1.0f), xSpeed(0.03f), ySpeed(0.05f){}

	Player(GLfloat ballRadius, GLfloat ballX, GLfloat ballY, GLfloat xSpeed, GLfloat ySpeed)
		:ballRadius(ballRadius), ballX(ballX), ballY(ballY), xSpeed(xSpeed), ySpeed(ySpeed){}

	void updateBoundaries(GLdouble clipAreaXLeft, GLdouble clipAreaXRight, GLdouble clipAreaYBottom, GLdouble clipAreaYTop) {
		ballXMin = clipAreaXLeft + ballRadius;
		ballXMax = clipAreaXRight - ballRadius;
		ballYMin = clipAreaYBottom + ballRadius;
		ballYMax = clipAreaYTop - ballRadius;
	}

	void drawPlayer() {
		// Detect boundary
		if (ballX > ballXMax) {
			ballX = ballXMax;
		}
		else if (ballX < ballXMin) {
			ballX = ballXMin;
		}
		if (ballY > ballYMax) {
			ballY = ballYMax;
		}
		else if (ballY < ballYMin) {
			ballY = ballYMin;
		}
		glPushMatrix();
		glTranslatef(ballX, ballY, 0.0f);  // Translate to (xPos, yPos)
		// Use triangular segments to form a circle
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0f, 0.0f, 1.0f);  // Blue
		glVertex2f(0.0f, 0.0f);       // Center of circle
		int numSegments = 100;
		GLfloat angle;
		for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
			angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
			glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
		}
		glEnd();
		glPopMatrix();
	}

	void moveRight() {
		ballX += xSpeed;
	}
	void moveLeft() {
		ballX -= xSpeed;
	}
	void moveUp() {
		ballY += ySpeed;
	}
	void moveDown() {
		ballY -= ySpeed;
	}

	void scaleLarge(GLdouble clipAreaXLeft, GLdouble clipAreaXRight, GLdouble clipAreaYBottom, GLdouble clipAreaYTop) {
		ballRadius *= 1.05f;
		ballXMin = clipAreaXLeft + ballRadius;
		ballXMax = clipAreaXRight - ballRadius;
		ballYMin = clipAreaYBottom + ballRadius;
		ballYMax = clipAreaYTop - ballRadius;
	}

	void scaleSmall(GLdouble clipAreaXLeft, GLdouble clipAreaXRight, GLdouble clipAreaYBottom, GLdouble clipAreaYTop) {
		ballRadius *= 0.95f;
		ballXMin = clipAreaXLeft + ballRadius;
		ballXMax = clipAreaXRight - ballRadius;
		ballYMin = clipAreaYBottom + ballRadius;
		ballYMax = clipAreaYTop - ballRadius;
	}
private:
	GLfloat ballRadius;   // Radius of the bouncing ball
	GLfloat ballX;        // Ball's center (x, y) position
	GLfloat ballY;
	GLfloat ballXMax, ballXMin, ballYMax, ballYMin; // Ball's center (x, y) bounds
	GLfloat xSpeed;      // Ball's speed in x and y directions
	GLfloat ySpeed;
};