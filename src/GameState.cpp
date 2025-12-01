#include "GameState.h"
#include "GameObject.h"
#include "EventHandler.h"
#include "Game.h"

GameState::GameState(Game* game) : game(game) {}
GameState::~GameState() {
    for (auto o : gameObjects) delete o;
}

void GameState::addObject(GameObject* o) { gameObjects.push_back(o); }
void GameState::addEventListener(EventHandler* h) { eventHandler.push_back(h); }
void GameState::runLater(Callback& cb) { lateCallbacks.push_back(cb); }

void GameState::update() {
    for (auto o : gameObjects) o->update();
    for (auto& cb : lateCallbacks) cb();
    lateCallbacks.clear();
}

void GameState::render() const {
    for (auto o : gameObjects) o->render();
}

void GameState::handleEvent(const SDL_Event& e) {
    for (auto h : eventHandler) h->handleEvent(e);
}

Collision GameState::checkCollision(const SDL_FRect& rect) const {
    return Collision(Collision::NONE);
}
