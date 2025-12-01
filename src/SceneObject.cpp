#include "SceneObject.h"

SceneObject::SceneObject(GameState* state)
	: GameObject(state), game(state->getGame())
{
	
	class Crosser : public SceneObject {};
	class Frog : public SceneObject {};
	class Wasp : public SceneObject {};
	class homedFrog : public SceneObject {};
}


void SceneObject::render() const {

	texture->render(this->getBoundingBox());
}

SDL_FRect SceneObject::getBoundingBox() const {
	SDL_FRect rect;
	rect.x = position.getX();
	rect.y = position.getY();
	rect.w = dimX;
	rect.h = dimY;

	return rect;
}

Collision SceneObject::checkCollision(const SDL_FRect& rect)
{
	return Collision(Collision::NONE);
}
