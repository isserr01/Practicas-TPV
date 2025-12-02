#include "EndState.h"
#include "MainMenuState.h"
#include <iostream>
#include <fstream>

EndState::EndState(Game* game, bool partidaGanada)
	: GameState(game), partidaGanada(partidaGanada)
{
	createButtons();
	if (partidaGanada) {
		title = new Label(this, game->getTexture(Game::HAS_GANADO), 100.0f, 140.0f);
	}
	else {
		title = new Label(this, game->getTexture(Game::GAME_OVER), 100.0f, 140.0f);
	}
}


EndState::~EndState() {
	delete title;
	for (Button* b : buttons)
		delete b;

}

void EndState::createButtons() {

	int startX = 25;
	int startY = 210;
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

void EndState::render() const {

	SDL_RenderClear(game->getRenderer());
	game->getTexture(Game::MAINMENU_BACKGROUND)->render();
	for (Button* b : buttons)
		b->render();
	SDL_RenderPresent(game->getRenderer());
}

void EndState::update()
{
	for (Button* b : buttons)
		b->update();

	if (index != -1) {
		switch (index)
		{
		case 0: //volver al menú
			game->replaceState(std::make_shared<MainMenuState>(game));
			return;

		case 1: //salir
			game->quit();
			return;
		default:
			break;
		}
		index = -1;
	}
}

void EndState::handleEvent(const SDL_Event& e)
{
	GameState::handleEvent(e);
}
//playButton->connect([this](){ startGame(); });