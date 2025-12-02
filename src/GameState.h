#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GameObject.h"
#include <list>
#include <functional>
#include "EventHandler.h"
#include <SDL3/SDL.h>
#include "Collision.h"

class Game;
class GameObject;

class GameState{
public:
	using Callback = std::function<void()>;

	GameState(Game* game);
	virtual ~GameState();
	virtual void render() const;
	virtual void update();
	virtual void handleEvent(const SDL_Event& event);
	void removeObject(GameObject* obj);
	void addObject(GameObject* obj);
	void addEventListener(EventHandler* handler);
	void runLater(const Callback& func);
	virtual Collision checkCollision(const SDL_FRect& rect) const;
	Game* getGame() const { return game; }

protected:

	std::list<GameObject*> gameObjects;
	std::list<EventHandler*> eventHandler;
	std::list<Callback> lateCallbacks;
	Game* game;
public:
	
};

#endif // GAMESTATE_H