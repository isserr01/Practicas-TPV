#ifndef LOG_H
#define LOG_H
#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include <SDL3/SDL.h>

class Game;
using Point2D = Vector2D<float>;
class Log
{
	Game* game;
	Texture* texture;
	Point2D position;
	Vector2D<float> speed;

public:
	Log(Game* game, const Point2D& position, const Vector2D<float>& speed,int type);
	
	void render() const;
	void update();
	bool checkCollision(const SDL_FRect& other) const;



};
#endif // LOG_H
