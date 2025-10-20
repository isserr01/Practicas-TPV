#ifndef FROG_H
#define FROG_H
#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include <SDL3/SDL.h>
#include "Collision.h"

class Game;
using Point2D = Vector2D<float>;
class Frog
{
	Game* game;
	Texture* texture;
	Point2D position;
	Vector2D<float> speed;
	Vector2D<float> moveDir;
	int currentFrame;
	bool moving;
	int lives;

public:
	Frog(Game* game, const Point2D& position, const Vector2D<float>& speed, int lives);
	~Frog();
	void render() const;
	void update();
	void HandleEvents(const SDL_Event& event);


};
#endif // FROG_H
