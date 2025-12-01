#pragma once
#include "GameState.h"

class Label;
class Button;
class EndState : public GameState
{
private:
	Label* titleLabel = nullptr;
	Button* mainMenuButt = nullptr;
	Button* exitButt = nullptr;

public:
	EndState(Game* game, bool win);
};

