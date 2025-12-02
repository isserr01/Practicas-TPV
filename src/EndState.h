#pragma once
#include "GameState.h"
#include <vector>
#include "game.h"
#include <SDL3/SDL.h>
#include <string>
#include "Button.h"
#include "Label.h"


class EndState : public GameState
{
private:
	std::vector<Button*> buttons;
	int index = -1;
	Label* title;
	bool partidaGanada;
	std::vector<Game::TextureName> buttonTextures{
		Game::VOLVER_AL_MENU,
		Game::SALIR
	};

	void createButtons();

public:
	EndState(Game* game,bool partidaGanada);
	~EndState();
	void update() override;
	void render() const override;
	void handleEvent(const SDL_Event& event) override;
};

