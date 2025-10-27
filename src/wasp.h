#ifndef WASP_H
#define WASP_H

#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include "collision.h"
class game;
using Point2D= Vector2D <float>;

class wasp
{
	Game* game;
	Texture* textura;
	Point2D posicion;
	bool alive=false;
	int durationAle;
	int positionAle;

public:
	wasp(Game* game, Texture* textura, Point2D posicion);
	bool isAlive();
	void spawn();
	void render() const;
	void update();
	Collision checkCollision(const SDL_FRect& other) const;
};
#endif WASP_H
