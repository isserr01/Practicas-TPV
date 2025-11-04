#include "Platform.h"


Collision Platform::checkCollision(const SDL_FRect& other){

	//rectángulo para comprobar colisión
	SDL_FRect rect = getBoundingBox();

	//si hay colisión, devuelve tipo plataforma y su velocidad
	if (SDL_HasRectIntersectionFloat(&rect, &other)) {
		return Collision(Collision::PLATFORM, speed);
	}
	return Collision(Collision::NONE);

}
