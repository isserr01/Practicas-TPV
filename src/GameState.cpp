#include "GameState.h"

GameState::GameState(Game* game) : game(game) {}
GameState:: ~GameState() {
	for (GameObject* obj : gameObjects)
		delete obj;
}
void GameState:: render() const {
	for(GameObject* obj : gameObjects)
		obj->render();
}
void GameState::update() {
	for (GameObject* obj : gameObjects)
		obj->update();
}
void GameState::removeObject(GameObject* obj) {
	gameObjects.remove(obj);
	delete obj;
}
GameState::Anchor addObject(GameObject* obj) {
	gameObjects.push_back(obj);
	return gameObjects.end();
}
void GameState::handleEvent(const SDL_Event& event) {
	for (EventHandler* handler : eventHandler)
		handler->handleEvent(event);
}
void GameState::addEventListener(EventHandler* handler) {
	eventHandler.push_back(handler);
}
void GameState::runLater(Callback&& func) {
	lateCallbacks.push_back(func);
}
