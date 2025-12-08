#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include "Button.h"
#include "texture.h"
#include "game.h"



class Label;

class MainMenuState : public GameState
{
private:
	Button* mapButton = nullptr;
	int index;
	Label* title;
	Button* leftArrow;
	Button* rightArrow;

	std::vector<Game::TextureName> buttonTextures{
		Game::AVISPADO,
		Game::ORIGINAL,
		Game::PRACTICA1,
		Game::TRIVIAL,
		Game::VELOZ
	};

	std::vector<std::string> mapName{
		"Avispado.txt",
		"Original.txt",
		"Práctica 1.txt",
		"Trivial.txt",
		"Veloz.txt"
	};
	
	void switchMap(int dir);
	void createButtons();
	void createMapButton();
	std::vector<std::string> getMaps();

public:
	MainMenuState(Game* game);
	~MainMenuState();

	void render() const override;
	void update() override;
	void handleEvent(const SDL_Event& event) override;
	

};
#endif // MAINMENUSTATE_H

