#include "InfoBar.h"
#include "game.h"

InfoBar::InfoBar(float maxTime, int lives, Game* game):
	maxTime(maxTime),lives(lives),timeLeft(maxTime),frogT(game->getTexture(Game::FROG)),game(game)
{}

void InfoBar::update(int newLives, float newTime) {
	lives = newLives;
	timeLeft = newTime;
}

void InfoBar::render(SDL_Renderer* renderer) {
	for (int i = 0;i < lives;i++) {
		SDL_FRect r = { 10.0f + i * (frogT->getFrameWidth() + 5),
						440.0f,(float)frogT->getFrameWidth(),(float)frogT->getFrameHeight()
		};
		frogT->renderFrame(r,0,0);
	}

	float w = 200;
	SDL_FRect timeBar = { 230, 450, w * (timeLeft / maxTime), 20 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &timeBar);

	SDL_FRect border = { 230, 450, w, 20 };
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderRect(renderer, &border);
}