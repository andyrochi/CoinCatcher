#include "Common.h"
#include <Math.h>
#include <iostream>

#define PI 3.14159265f
#pragma once

enum Status { NORMAL, RECOVER, INVINCIBLE };
int i = 0;

class Player {
public:
	Player()
		:ballRadius(playerRadius), ballX(0.0f), ballY(-1.0f), xSpeed(playerSpeed), ySpeed(0.05f), status(NORMAL){}

	Player(GLfloat ballRadius, GLfloat ballX, GLfloat ballY, GLfloat xSpeed, GLfloat ySpeed)
		:ballRadius(ballRadius), ballX(ballX), ballY(ballY), xSpeed(xSpeed), ySpeed(ySpeed), status(NORMAL){}

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
		if (status == NORMAL) {
			glColor3f(0.0f, 0.0f, 1.0f);  // Blue
		}
		else if (status == RECOVER) {
			// Recover flashing effect
			i = (i + 1) % 4;
			GLfloat alphas[] = { 0.25f, 0.5f, 0.75f, 1.0f };
			GLfloat colors[][3] = {
				{0.545f, 0.0f, 1.0f },
				{0.545f, 0.0f, 1.0f },
				{0.545f, 0.0f, 1.0f },
				{0.0f, 0.0f, 1.0f }
			};
			glColor4f(colors[i][0], colors[i][1], colors[i][2], alphas[i]);  // Green
		}
		else {
			glColor3f(1.0f, 0.0f, 0.0f);  // Red
		}
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
		if (GAME_STATUS != PLAYING) return;
		ballX += xSpeed;
	}
	void moveLeft() {
		if (GAME_STATUS != PLAYING) return;
		ballX -= xSpeed;
	}
	void moveUp() {
		if (GAME_STATUS != PLAYING) return;
		ballY += ySpeed;
	}
	void moveDown() {
		if (GAME_STATUS != PLAYING) return;
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

	void setStatus(Status newStatus=NORMAL) {
		std::cout << "set status:" << newStatus << std::endl;
		status = newStatus;
		switch (newStatus) {
		case INVINCIBLE:
			ballRadius = 2.0f * playerRadius;
			xSpeed = playerFasterSpeed;
			updateBoundaries(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
			break;
		default:
			ballRadius = playerRadius;
			xSpeed = playerSpeed;
			updateBoundaries(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
			ballY = ballYMin;
			break;
		}
	}

	Status getStatus() {
		return status;
	}

	GLfloat getPosX() { return ballX; }
	GLfloat getPosY() { return ballY; }
	GLfloat getRadius() { return ballRadius; }

	void reset() {
		ballX = 0.0f;
		ballY = (-1.0f);
		xSpeed = (playerSpeed);
		ySpeed = (0.05f);
		status = (NORMAL);
	}
private:
	GLfloat ballRadius;   // Radius of the bouncing ball
	GLfloat ballX;        // Ball's center (x, y) position
	GLfloat ballY;
	GLfloat ballXMax, ballXMin, ballYMax, ballYMin; // Ball's center (x, y) bounds
	GLfloat xSpeed;      // Ball's speed in x and y directions
	GLfloat ySpeed;
	Status status;
};