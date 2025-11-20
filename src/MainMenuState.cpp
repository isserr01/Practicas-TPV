#include "MainMenuState.h"
#include "PlayState.h"

MainMenuState::MainMenuState(GameStateMachine* g, SDL_Renderer* r)
	: gsm(g), renderer(r)
{
	playButton = new Button(renderer, 200, 200, 200, 50, [this]() {
		
	});
	exitButton = new Button(renderer, 150, 250, 150, 50, [this]() {
		SDL_Event quit;
		quit.type = SDL_EVENT_QUIT;
		SDL_PushEvent(&quit);
	});
}

MainMenuState::~MainMenuState()
{
	delete playButton;
	delete exitButton;
}

void MainMenuState::render() const
{
	playButton->render();
	exitButton->render();
}

void MainMenuState::handleEvent(const SDL_Event& event)
{
	playButton->handleEvent(event);
	exitButton->handleEvent(event);
}