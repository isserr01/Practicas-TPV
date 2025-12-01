#ifndef VEHICLE_H
#define VEHICLE_H
#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include <SDL3/SDL.h>
#include "Collision.h"	
#include <istream>
#include "Crosser.h"


class Game;
class GameState;
using Point2D = Vector2D<float>;
class Vehicle : public Crosser
{

	Game* game;
public:
	//constructora
	Vehicle(GameState* state, std::istream& in);
	//método para comprobar colisiones con un rectángulo dado
	Collision checkCollision(const SDL_FRect& other) override;
};
#endif // VEHICLE_H
