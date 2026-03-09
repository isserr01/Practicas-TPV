// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
class FighterFacade;

class GameState {
public:
	GameState() : _fighterFacade(nullptr) {}
	virtual ~GameState() {
	}
	virtual void update() = 0;
	virtual void enter() = 0;
	virtual void leave() = 0;
protected:
	FighterFacade* _fighterFacade;
};

