#ifndef NEWGAMESTATE_H
#define NEWGAMESTATE_H


#include <SDL_rect.h>

#include "GameState.h"

class Texture;

class NewGameState : public GameState {
public:
	NewGameState();
	virtual ~NewGameState();
	void leave() override;
	void update() override;
	void enter() override;
	void receive(const Message& m) override {};
private:
	const Texture& _msg;
};


#endif // NEWGAMESTATE_H