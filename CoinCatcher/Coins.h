#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Math.h>
#include <vector>
#include <iostream>
#include "Player.h"
#include "Score.h"
#define PI 3.14159265f

class Coin {
public:
	static inline GLfloat posXMax = 1.0f, posXMin = -1.0f, posYMax = 1.0f, posYMin = -1.0f;
	static inline GLfloat radius = 0.05f;
	static void setBoundaries(GLdouble clipAreaXLeft, GLdouble clipAreaXRight, GLdouble clipAreaYBottom, GLdouble clipAreaYTop) {
		posXMin = clipAreaXLeft + radius;
		posXMax = clipAreaXRight - radius;
		posYMin = clipAreaYBottom + radius;
		posYMax = clipAreaYTop - radius;
	}

	Coin():posX(0.0f), posY(1.0f), ySpeed(-0.05f), activated(false) {}

	bool collisionCheck(GLfloat objPosX, GLfloat objPosY, GLfloat objRadius) {
		GLfloat xDis = objPosX - posX;
		GLfloat yDis = objPosY - posY;
		GLfloat dist2 = xDis * xDis + yDis * yDis;
		GLfloat rad = (objRadius + radius);
		GLfloat rad2 = rad * rad;
		return dist2 < rad2;
	}

	void draw() {
		if (!activated) return;

		if (posX > posXMax) {
			posX = posXMax;
		}
		else if (posX < posXMin) {
			posX = posXMin;
		}
		if (posY > posYMax) {
			posY = posYMax;
			ySpeed = -ySpeed;
		}
		else if (posY < posYMin) {
			posY = posYMin;
			this->deactivate();
		}

		// Draw 
		glPushMatrix();
		glTranslatef(posX, posY, 0.0f);  // Translate to (xPos, yPos)
		// Use triangular segments to form a circle
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);       // Center of circle
		int numSegments = 100;
		GLfloat angle;
		for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
			angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
			glVertex2f(cos(angle) * radius, sin(angle) * radius);
		}
		glEnd();
		glPopMatrix();

		posY -= ySpeed;
	}

	bool isActivated() {
		return activated;
	}

	void reset(GLfloat posX, GLfloat posY) {
		activated = true;
		this->posX = posX;
		this->posY = posY;
		ySpeed = ySpeed > 0 ? -ySpeed : ySpeed;
	}

	void deactivate() {
		activated = false;
	}

	GLfloat getPosX() {
		return posX;
	}

	GLfloat getPosY() {
		return posY;
	}

private:
	GLfloat posX, posY, ySpeed;
	bool activated;
};

class CoinDispatcher {
public:
	CoinDispatcher()
		:radius(0.1f), posX(0.0f), posY(1.0f), xSpeed(0.03f) {
		for (size_t i = 0; i < 100; i++) {
			coinList.emplace_back();
		}
	}

	void updateBoundaries(GLdouble clipAreaXLeft, GLdouble clipAreaXRight, GLdouble clipAreaYBottom, GLdouble clipAreaYTop) {
		posXMin = clipAreaXLeft + radius;
		posXMax = clipAreaXRight - radius;
		posYMin = clipAreaYBottom + radius;
		posYMax = clipAreaYTop - radius - 0.2f;
		Coin::setBoundaries(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
	}

	void draw() {
		if (posX > posXMax) {
			posX = posXMax;
			xSpeed = -xSpeed;
		}
		else if (posX < posXMin) {
			posX = posXMin;
			xSpeed = -xSpeed;
		}
		if (posY > posYMax) {
			posY = posYMax;
		}

		glPushMatrix();
		glTranslatef(posX, posY, 0.0f);  // Translate to (xPos, yPos)
		// Use triangular segments to form a circle
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0f, 1.0f, 1.0f);  // Blue
		glVertex2f(0.0f, 0.0f);       // Center of circle
		int numSegments = 100;
		GLfloat angle;
		for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
			angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
			glVertex2f(cos(angle) * radius, sin(angle) * radius);
		}
		glEnd();
		glPopMatrix();

		posX += xSpeed;

		// draw coins
		for (auto& c : coinList) {
			c.draw();
		}
	}

	void addCoin() {
		size_t newCoinIndex = 0;
		for (size_t i = 0; i < coinList.size(); i++) {
			// not activated
			if (!coinList[i].isActivated()) {
				newCoinIndex = i;
				break;
			}
		}
		coinList[newCoinIndex].reset(posX, posY);
	}

	void collisionCheck(Player& player, Score& score) {
		GLfloat playerRad = player.getRadius();
		GLfloat playerX = player.getPosX();
		GLfloat playerY = player.getPosY();

		for (auto& coin : coinList) {
			if (!coin.isActivated()) continue;
			bool status = coin.collisionCheck(playerX, playerY, playerRad);
			if (status) {
				std::cout << "Collided! At pos:(" << coin.getPosX() << ", " << coin.getPosY() << ")" << std::endl;
				score.incScore(50);
				coin.deactivate();
			}
		}
	}

private:
	GLfloat radius = 0.1f;   // Radius of the bouncing ball
	GLfloat posX = 0.0f;        // Ball's center (x, y) position
	GLfloat posY = -1.0f;
	GLfloat posXMax, posXMin, posYMax, posYMin; // Ball's center (x, y) bounds
	GLfloat xSpeed = 0.05f;      // Ball's speed in x and y directions
	std::vector<Coin> coinList;
};