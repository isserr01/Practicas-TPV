#include "PauseState.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include <iostream>
#include <fstream>

PauseState::PauseState(Game* game, PlayState* playState)
	: GameState(game), playState(playState)
{
	createButtons();
}


PauseState::~PauseState() {
	for (Button* b : buttons) {
		removeObject(b);
		removeEventListener(b);
		delete b;
	}
	buttons.clear();
		

}

void PauseState::createButtons() {

	int startX = 150;
	int startY = 150;
	int diffY = 75;
	int x = startX;
	int y = startY;

	for (int i = 0;i < buttonTextures.size();i++) {
		Texture* t = game->getTexture(buttonTextures[i]);
		Button* b = new Button(this, t, x, y);

		b->connect([this, i]() {
			index = i;
			});

		buttons.push_back(b);
		addObject(b);

		x = startX;
		y += diffY;
	}
}

void PauseState::render() const {
	

	SDL_SetRenderDrawBlendMode(game->getRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(game->getRenderer(), 0, 0, 0, 150); 
	SDL_FRect screenRect = { 0, 0,Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT }; 
	SDL_RenderFillRect(game->getRenderer(), &screenRect);

	for (Button* b : buttons)
		b->render();

	SDL_RenderPresent(game->getRenderer());
}


void PauseState::update()
{


	for (Button* b : buttons)
		b->update();

	if (index != -1) {
		switch (index)
		{
			case 0: //continuar
				game->popState(); //salir del estado de pausa
			return;

			case 1: //reiniciar
				game->replaceState(std::make_shared<PlayState>(game,mapName));
			return;

			case 2: //volver al menú
				game->replaceState(std::make_shared<MainMenuState>(game));
			return;

			case 3: //salir
				game->quit();
			return;
		default:
			break;
		}
		index = -1;
	}
}

void PauseState::handleEvent(const SDL_Event& e)
{
	GameState::handleEvent(e);
}
