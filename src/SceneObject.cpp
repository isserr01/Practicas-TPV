#include "SceneObject.h"

SceneObject::SceneObject(Game* game)
	: GameObject(game)
{
	class Vehicle : public SceneObject{};
	class Log : public SceneObject {};
	class Frog : public SceneObject {};
	class Wasp : public SceneObject {};
	class TurtleGroup : public SceneObject {};
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
