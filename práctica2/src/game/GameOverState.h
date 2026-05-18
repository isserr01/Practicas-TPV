#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState.h"
#include <SDL_rect.h>

class Texture;

class GameOverState : public GameState {
public:
	GameOverState(bool won);
	virtual ~GameOverState() {};
	void leave() override {};
	void update() override;
	void enter() override;
private:
	bool _won;

};

#endif // GAMEOVERSTATE_H


