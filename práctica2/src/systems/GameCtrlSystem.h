// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"

class GameCtrlSystem: public ecs::System {
public:

	GameCtrlSystem();
	virtual ~GameCtrlSystem();

	inline auto getScore() {
		return _score;
	}

	inline void setScore(unsigned int score) {
		_score = score;
	}

	inline void incrScore(unsigned int n) {
		_score += n;
	}

	void initSystem() override;
	void update() override;
	void recieve(const Message &m) override;

private:
	unsigned int _score;
};

