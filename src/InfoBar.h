#ifndef INFOBAR_H
#define INFOBAR_H

#include <SDL3/SDL.h>
#include "texture.h"
#include "game.h"

class InfoBar
{
	int lives;
	float timeLeft;
	float maxTime;
	Texture* frogT;
	Game* game;

public:
	InfoBar(float maxTime, int lives,Game* game);
	void update(int newLives, float newTime);
	void render(SDL_Renderer* renderer);

};

#endif //INFOBAR_H