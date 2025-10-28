#ifndef VEHICLE_H
#define VEHICLE_H
#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include <SDL3/SDL.h>
#include "Collision.h"	
#include <istream>

constexpr float VEHICLE_SPEED_FACTOR = 0.001f;


class Game;
using Point2D = Vector2D<float>;
class Vehicle
{
	Game* game;
	Texture* texture;
	Point2D position;
	Vector2D<float> speed;

public:
	//constructora
	Vehicle(Game* game, std::istream& in);
	~Vehicle();
	//método de renderizado
	void render() const;
	//método de actualización de posición
	void update();
	//método para comprobar colisiones con un rectángulo dado
	Collision checkCollision(const SDL_FRect& other) const;
};
#endif // VEHICLE_H
