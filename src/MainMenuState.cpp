#include "MainMenuState.h"
#include "Button.h"
#include "Label.h"
#include "game.h"
#include "PlayState.h"

#include <filesystem>
#include <iostream>
#include <fstream>

MainMenuState::MainMenuState(Game* game)
	:GameState(game), selected(0), titleLabel(nullptr), mapLabel(nullptr), playButt(nullptr), exitButt(nullptr)
{
	Button* btn = new Button(this, game->getTexture(Game::BACKGROUND), 0, 0);
	btn->connect([game]() {
		game->pushState(new PlayState(game));
		});
	addObject(btn);
	addEventListener(btn);

}
