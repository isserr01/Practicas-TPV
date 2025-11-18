#ifndef WASP_H
#define WASP_H

#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include "Collision.h"
#include "SceneObject.h"
class game;
using Point2D= Vector2D <float>;

constexpr int WASP_SPAWN = 25;

class wasp : public SceneObject
{
	
	int durationAle;
	int positionAle;

public:
	bool alive = false;
	wasp(Game* game, Point2D posicion);
	bool isAlive() const;
	void spawn();
	void update() override;
	Collision checkCollision(const SDL_FRect& other) override;
	int getPosition() const;
	void SetPosition(const int& index);
};
#endif //WASP_H
