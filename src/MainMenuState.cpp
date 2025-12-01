#include "MainMenuState.h"
#include "Button.h"
#include "Label.h"
#include "game.h"
#include "PlayState.h"
#include <filesystem>
#include <iostream>
#include <fstream>

MainMenuState::MainMenuState(Game* game)
	:GameState(game)
{

	createButtons();
	title = new Label(this, game->getTexture(Game::ELIGE_UN_MAPA), 100.0f, 140.0f);

}

MainMenuState::~MainMenuState() {
	for (Button* b : buttons)
		delete b;

}

void MainMenuState::createButtons() {

	int startX = 25;
	int startY = 210;
	int diffX = 180;
	int diffY = 75;
	int cols = 2;
	int x = startX;
	int y = startY;

	for (int i = 0;i < mapName.size();i++) {
		Texture* t = game->getTexture(buttonTextures[i]);
		Button* b = new Button(this, t, x, y);
		

		b->connect([this, i]() {
			index = i;
			});

		buttons.push_back(b);
		addObject(b);

		if (i % cols == 0) {
			x = startX;
			y += diffY;
		}
		else x += diffX;	
	}
}

void MainMenuState::render() const {

	SDL_RenderClear(game->getRenderer());
	game->getTexture(Game::MAINMENU_BACKGROUND)->render();
	for (Button* b : buttons)
		b->render();
	title->render();
	SDL_RenderPresent(game->getRenderer());
}

void MainMenuState::update()
{
	for (Button* b : buttons)
		b->update();

	if (index != -1) {
		game->pushState(std::make_shared<PlayState>(game, mapName[index]));
		index = -1;
	}
}

void MainMenuState::handleEvent(const SDL_Event& e)
{
	GameState::handleEvent(e);
}