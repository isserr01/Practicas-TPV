#pragma once
#include "GameState.h"
class Button;
class PauseState : public GameState
{
private :
	Button* resumeButton = nullptr;
	Button* restartButton = nullptr;
	Button* mainMenuButton = nullptr;
public:
	PauseState(Game* game);
	void update() override;
};

