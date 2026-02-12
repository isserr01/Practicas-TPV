#pragma once

#include "InputComponent.h"
#include <SDL.h>

class FighterCtrl : public InputComponent
{
public:
	FighterCtrl();
	virtual ~FighterCtrl();
	void handleInput(Container* o) override;

	inline void setKeys(SDL_Scancode right, SDL_Scancode left, SDL_Scancode up) {
		_left = left;
		_right = right;
		_up = up;
	}

private:
	SDL_Scancode _left;
	SDL_Scancode _right;
	SDL_Scancode _up;

};

