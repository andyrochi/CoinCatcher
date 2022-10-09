#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Math.h>
#include <vector>
#include <iostream>
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

	Coin():posX(0.0f), posY(1.0f), ySpeed(0.05f), activated(false) {}

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
		}
		else if (posY < posYMin) {
			posY = posYMin;
			activated = false;
		}

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

	bool getStatus() {
		return activated;
	}

	void reset(GLfloat posX, GLfloat posY) {
		activated = true;
		this->posX = posX;
		this->posY = posY;
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
		posYMax = clipAreaYTop - radius;
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
		//std::cout << "Coin added at" << posX << " " << posY << std::endl;
		//coinList.emplace_back( posX, posY );
		size_t newCoinIndex = 0;
		for (size_t i = 0; i < coinList.size(); i++) {
			// not activated
			if (!coinList[i].getStatus()) {
				newCoinIndex = i;
				break;
			}
		}
		coinList[newCoinIndex].reset(posX, posY);
	}

private:
	GLfloat radius = 0.1f;   // Radius of the bouncing ball
	GLfloat posX = 0.0f;        // Ball's center (x, y) position
	GLfloat posY = -1.0f;
	GLfloat posXMax, posXMin, posYMax, posYMin; // Ball's center (x, y) bounds
	GLfloat xSpeed = 0.05f;      // Ball's speed in x and y directions
	std::vector<Coin> coinList;
};