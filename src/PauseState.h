#pragma once
#include "GameState.h"
#include <vector>
#include "game.h"
#include <SDL3/SDL.h>
#include <string>
#include "Button.h"
#include "PlayState.h"


class PauseState : public GameState
{
private :
	std::vector<Button*> buttons;
	int index = -1;
	const std::string mapName;
	PlayState* playState;

	std::vector<Game::TextureName> buttonTextures{
		Game::CONTINUAR,
		Game::REINICIAR,
		Game::VOLVER_AL_MENU,
		Game::SALIR
	};

	void createButtons();

public:
	PauseState(Game* game, PlayState* playState);
	~PauseState();
	void update() override;
	void render() const override;
	void handleEvent(const SDL_Event& event) override;
};
