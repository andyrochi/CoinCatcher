#pragma once
#include "gameUtils.h"
#include <string>
#include <string.h>
#include <vector>
// A class to show floating text at an abritrary position
class TextObject {
public:

	TextObject():xpos(0.0f), ypos(0.0f), activated(false), frameCnt(15), color(WHITE) {}
	TextObject(GLfloat xpos, GLfloat ypos) {
		this->xpos = xpos;
		this->ypos = ypos;
		activated = false;
		frameCnt = 15;
	}
	void displayObject() {
		char* str = _strdup(content.c_str());
		displayTextSmall(str, xpos, ypos, color);
		ypos += 0.001f;
		frameCnt--;
		if (str != NULL) free(str);
		if (frameCnt <= 0) activated = false;
	};
	bool isActivated() { return activated; }
	void setText(GLfloat xpos, GLfloat ypos, char str[], TextColor newColor=WHITE) {
		this->xpos = xpos;
		this->ypos = ypos;
		content = str;
		activated = true;
		frameCnt = 25;
		color = newColor;
	}

private:
	GLfloat xpos;
	GLfloat ypos;
	size_t frameCnt;
	std::string content;
	bool activated;
	TextColor color;
};

class TextManager {
public:
	TextManager() {
		textPool.resize(20);
	}

	void addText(GLfloat xpos, GLfloat ypos, char str[], TextColor color=WHITE) {
		size_t i = 0;
		for (i = 0; i < textPool.size(); i++) {
			if (!textPool[i].isActivated()) { break; }
		}
		textPool[i].setText(xpos, ypos, str, color);
	}

	void displayText() {
		for (size_t i = 0; i < textPool.size(); i++) {
			if (textPool[i].isActivated()) {
				textPool[i].displayObject();
			}
		}
	}
private:
	std::vector<TextObject> textPool;
};