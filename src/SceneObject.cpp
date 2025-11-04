#include "SceneObject.h"

SceneObject::SceneObject(Game* game)
	: GameObject(game)
{
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

