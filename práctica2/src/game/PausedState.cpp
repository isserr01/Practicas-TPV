#include "PausedState.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../game/Game.h"
#include <iostream>

void PausedState::leave() {
	sdlutils().virtualTimer().resume();
}

void PausedState::enter() {
	sdlutils().virtualTimer().pause();
	std::cout << "paused state" << std::endl;
}

void PausedState::update() {

	sdlutils().clearRenderer();

	if (ih().keyDownEvent()) {
		//Game::Instance()->setState(Game::RUNNING);
	}

	auto& msg = sdlutils().msgs().at("continue");

	msg.render(
		(sdlutils().width() - msg.width()) / 2,
		sdlutils().height() / 2 + msg.height() * 2
	);

	//std::string score = "Score: " + std::to_string(Game::Instance()->getScore());
	//auto text = sdlutils().fonts().at("ARIAL24").renderText(score, {0,0,0,255});
	//SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlutils().renderer(), text);

	//SDL_FRect dst = { 50,50,text->w,text->h };
	//SDL_RenderTexture(sdlutils().renderer(), tex, nullptr, &dst);

	//SDL_DestroySurface(text);
	//SDL_DestroyTexture(tex);


	sdlutils().presentRenderer();
}
