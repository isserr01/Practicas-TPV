#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include <vector>
#include <string>
#include <SDL3/SDL.h>
class Button;
class Label;
class MainMenuState : public GameState
{
private:
	std::vector <std::string> mapList;
	int selected = 0;

	Label* titleLabel = nullptr;
	Label* mapLabel = nullptr;
	Button* playButt = nullptr;
	Button* exitButt = nullptr;

	void loadMap();
	void updateMapLabel();

public:
	MainMenuState(Game* game);



};
#endif // MAINMENUSTATE_H

