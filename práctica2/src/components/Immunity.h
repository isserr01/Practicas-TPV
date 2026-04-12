#pragma once
#include "../ecs/Component.h"
class Immunity :public ecs::Component
{
public:
	bool active=false;
	float timerStart = 0.0f;
};

