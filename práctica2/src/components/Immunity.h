#pragma once
#include "../ecs/Component.h"

struct Immunity :public ecs::Component
{
	bool active=false;
	float timerStart = 0.0f;
};

