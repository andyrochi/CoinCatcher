#pragma once
#include <iostream>
class ScoreManager {
public:
	ScoreManager() {
		this->score = 0;
	}
	void incScore(int val=50) {
		this->score += val;
		std::cout << "+" << val << std::endl;
		printScore();
	}
	void decScore(int val = 30) {
		this->score -= val;
		if (score <= 0) score = 0;
		std::cout << "-" << val << std::endl;
		printScore();
	}
	int getScore() {
		return score;
	}
	void printScore() {
		std::cout << "Current Score: " << score << std::endl;
	}

	void resetScore() {
		this->score = 0;
	}

private:
	int score;
};