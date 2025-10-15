#ifndef VEHICLE_H
#define VEHICLE_H
#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include <SDL3/SDL.h>

class Game;
using Point2D = Vector2D<float>;
class Vehicle
{
	Game* game;
	Texture* texture;
	Point2D position;
	Vector2D<float> speed;
	//añado un atributo auxiliar para poder llamar a la constructora y que se 
	//construyan automáticamente al leer el mapa
	int type;

public:
	//constructora
	Vehicle(Game* game, const Point2D& position, const Vector2D<float>& speed, int type);
	//método de renderizado
	void render() const;
	//método de actualización de posición
	void update();
	//método para comprobar colisiones con un rectángulo dado
	bool checkCollision(const SDL_FRect& other) const;
};
#endif // VEHICLE_H
