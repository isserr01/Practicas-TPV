#pragma once
#include "GameObject.h"
#include <list>
#include <SDL3/SDL.h>
class Game;

class GameState{
public:
	using Callback = std::is_function<void()>;
	using Anchor = std::list<class GameObject*>::iterator;
protected:

	std::list<GameObject*> gameObjects;
	std::list<EventHandler*> eventHandler;
	std::list<Callback> lateCallbacks;
	Game* game;
public:
	GameState(Game* game);
	~GameState();
	Game* getGame() { return game; }
	void render() const;
	void update();
	void handleEvent(const SDL_Event& event);
	void removeObject(GameObject* obj);
	Anchor addObject(GameObject* obj);
	void addEventListener(EventHandler* handler);
	void runLater(Callback&& func);
};

