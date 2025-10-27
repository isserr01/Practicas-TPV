#ifndef HOMED_FROG_H
#define HOMED_FROG_H

#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include "collision.h"
class game;
using Point2D = Vector2D <float>;

class HomedFrog
{
	Game* game;
	Texture* textura;
	Point2D posicion;

public:
	HomedFrog(Game* game, Texture* textura, Point2D posicion);
	void render() const;

};
#endif HOMED_FROG_H

