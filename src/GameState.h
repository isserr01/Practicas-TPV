#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "GameObject.h"
#include "EventHandler.h"
#include "game.h"
#include <list>

class GameState
{
	Game* game;
protected:
	std::list<GameObject*> gameObjects;
	std::list<EventHandler*> eventHandlers;
	

public:
	virtual void update() = 0;
	virtual void render() const;
	virtual void handleEvent(const SDL_Event& event) = 0;
	void addObject(GameObject* obj);
	void addEventListener(EventHandler* handler);

};

#endif // GAMESTATE_H
