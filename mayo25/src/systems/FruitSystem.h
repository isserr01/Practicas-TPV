// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/System.h"
#include <vector>

class FruitSystem: public ecs::System {
public:

	FruitSystem();
	virtual ~FruitSystem();
	void initSystem() override;
	void update() override;
	void addFruit(unsigned int n);
	void onFruitEaten(ecs::entity_t e);
	void clearAllFruit();
	void recieve(const Message &m) override;
	void resetTimers();
private:
	unsigned int _starsLimit;
	unsigned int _currNumOfStars;
	unsigned int _starPosX;
	unsigned int _starPosY;

	std::vector<float> _eatenFruitTimes; // Vector para almacenar los tiempos de las frutas comidas
};

