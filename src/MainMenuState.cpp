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
	loadMap();
}
void MainMenuState::handleEvent(const SDL_Event& event) {

}
void MainMenuState::update() {

}