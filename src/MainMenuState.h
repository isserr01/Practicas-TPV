#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H
#include "GameState.h"
#include "gameStateMachine.h"
#include "Button.h"

class MainMenuState : public GameState
{
	GameStateMachine* gsm;
	SDL_Renderer* renderer;

	Button* playButton;
	Button* exitButton;

public:
	MainMenuState(GameStateMachine* g, SDL_Renderer* r);
	~MainMenuState();
	void update() override;
	void render() const override;
	void handleEvent(const SDL_Event& event) override;

};

#endif // MAINMENUSTATE_H