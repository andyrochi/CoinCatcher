#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Math.h>
#include <vector>
#include <iostream>
#include "Player.h"
#include "ScoreManager.h"
#define PI 3.14159265f

enum objectType { COIN, OBSTACLE, POWERUP };
Player* playerPtr = NULL;
size_t powerUpCnt = 0;
// Change player status from recover to normal
void recoverPlayer(int val) {
	if (playerPtr != NULL && playerPtr->getStatus() == RECOVER)
		playerPtr->setStatus(NORMAL);
}
void setPlayerNormal(int val) {
	if (playerPtr == NULL) return;
	powerUpCnt -= 1;
	if (powerUpCnt <= 0) {
		powerUpCnt = 0;
		playerPtr->setStatus(NORMAL);
	}
}

class Coin {
public:
	static inline GLfloat posXMax = 1.0f, posXMin = -1.0f, posYMax = 1.0f, posYMin = -1.0f;
	static inline GLfloat radius = 0.05f;
	static inline GLfloat rsin = 0.05f * sin(PI/6);
	static inline GLfloat rcos = 0.05f * cos(PI / 6);
	static void setBoundaries(GLdouble clipAreaXLeft, GLdouble clipAreaXRight, GLdouble clipAreaYBottom, GLdouble clipAreaYTop) {
		posXMin = clipAreaXLeft + radius;
		posXMax = clipAreaXRight - radius;
		posYMin = clipAreaYBottom + radius;
		posYMax = clipAreaYTop - radius;
	}

	Coin():posX(0.0f), posY(1.0f), ySpeed(-0.05f), activated(false), type(COIN) {}

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
		if (type == COIN) {
			glBegin(GL_TRIANGLE_FAN);
			// (233,173,3)
			glColor3f(0.914f, 0.678f, 0.012f);
			glVertex2f(0.0f, 0.0f);       // Center of circle
			int numSegments = 80;
			GLfloat angle;
			for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
				angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
				glVertex2f(cos(angle) * radius, sin(angle) * radius);
			}
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
			// (243,199,13)
			glColor3f(0.952f, 0.780f, 0.051f);
			glVertex2f(0.0f, 0.0f);       // Center of circle
			for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
				angle = i * 2.0f * PI / numSegments;  // 360 deg for all segments
				glVertex2f(cos(angle) * radius * 0.7, sin(angle) * radius * 0.7);
			}
			glEnd();
		}
		else if (type == OBSTACLE){
			glBegin(GL_POLYGON);
			glColor4f(0.85, 0.85, 0.85, 1);
			glVertex2f(-1.5*radius, radius);
			glVertex2f(-1.5*radius, -radius);
			glVertex2f(1.5*radius, -radius);
			glVertex2f(1.5*radius, radius);
			glEnd();
		}
		else {
			glBegin(GL_TRIANGLES);
			glColor3f(1, 0, 0);
			glVertex2f(-rcos, -rsin);
			glColor3f(1, 1, 0);
			glVertex2f(rcos, -rsin);
			glColor3f(0, 1, 0);
			glVertex2f(0.0f, radius);
			glEnd();
		}
		glPopMatrix();

		updatePosition();
	}

	void updatePosition() {
		if (GAME_STATUS != PLAYING) return;
		posY -= ySpeed;
	}

	bool isActivated() {
		return activated;
	}

	void reset(GLfloat posX, GLfloat posY, objectType newType = COIN) {
		activated = true;
		this->posX = posX;
		this->posY = posY;
		ySpeed = ySpeed > 0 ? -ySpeed : ySpeed;
		this->type = newType;
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

	objectType getType() {
		return type;
	}

private:
	GLfloat posX, posY, ySpeed;
	bool activated;
	objectType type;
};

class CoinDispatcher {
public:
	CoinDispatcher()
		:radius(0.1f), posX(0.0f), posY(1.0f), xSpeed(0.03f) {
		for (size_t i = 0; i < 100; i++) {
			objectPool.emplace_back();
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

		updatePosition();

		// draw coins
		for (auto& c : objectPool) {
			c.draw();
		}
	}

	void updatePosition() {
		if (GAME_STATUS != PLAYING) return;
		posX += xSpeed;
	}

	void tossObject() {
		if (GAME_STATUS != PLAYING) return;
		size_t newObjectIndex = 0;
		for (size_t i = 0; i < objectPool.size(); i++) {
			// not activated
			if (!objectPool[i].isActivated()) {
				newObjectIndex = i;
				break;
			}
		}
		objectType type;
		int rnum = rand() % 10;
		if (rnum == 0) {
			type = POWERUP;
		}
		else if (rnum > 0 && rnum <3){
			type = OBSTACLE;
		}
		else {
			type = COIN;
		}
		objectPool[newObjectIndex].reset(posX, posY, type);
	}

	void collisionCheck(Player& player, ScoreManager& score) {
		if (GAME_STATUS != PLAYING) return;
		GLfloat playerRad = player.getRadius();
		GLfloat playerX = player.getPosX();
		GLfloat playerY = player.getPosY();
		playerPtr = &player;

		for (auto& obj : objectPool) {
			if (!obj.isActivated()) continue;
			bool hit = obj.collisionCheck(playerX, playerY, playerRad);
			if (hit) {
				//std::cout << "Collided! At pos:(" << coin.getPosX() << ", " << coin.getPosY() << ")" << std::endl;
				switch (obj.getType()) {
				case COIN:
					std::cout << "COIN!" << std::endl;
					score.incScore(50);
					obj.deactivate();
					break;
				case OBSTACLE:
					if (player.getStatus() != NORMAL) {
						break;
					}
					std::cout << "OH NO!" << playerPtr << std::endl;
					player.setStatus(RECOVER);
					score.decScore(100);
					glutTimerFunc(2000, recoverPlayer, 0);
					obj.deactivate();
					break;
				case POWERUP:
					std::cout << "POWERUP: " << powerUpCnt << std::endl;
					powerUpCnt++;
					player.setStatus(INVINCIBLE);
					glutTimerFunc(4000, setPlayerNormal, 0);
					obj.deactivate();
					break;
				}
				
				//coin.deactivate();
			}
		}
	}

	void reset() {
		posX = 0.0f;
		posY = 1.0f;
		for (auto& coin : objectPool) {
			coin.deactivate();
		}
	}

private:
	GLfloat radius = 0.1f;   // Radius of the bouncing ball
	GLfloat posX = 0.0f;        // Ball's center (x, y) position
	GLfloat posY = -1.0f;
	GLfloat posXMax, posXMin, posYMax, posYMin; // Ball's center (x, y) bounds
	GLfloat xSpeed = 0.05f;      // Ball's speed in x and y directions
	std::vector<Coin> objectPool;
};