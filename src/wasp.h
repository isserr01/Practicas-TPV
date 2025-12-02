#ifndef WASP_H
#define WASP_H

#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include "Collision.h"
#include "SceneObject.h"
class Game;
class GameState;
using Point2D= Vector2D <float>;

constexpr int WASP_SPAWN = 25;

class wasp : public SceneObject
{
	
	int durationAle;
	int positionAle;
	bool alive = false;
	Game* game;

public:
	
	wasp(GameState* state, Point2D posicion);
	bool isAlive() const override;
	void spawn();
	void update() override;
	Collision checkCollision(const SDL_FRect& other) override;
	int getPosition() const;
	void SetPosition(int index);
	bool isWasp() const override { return true; }
	
};
#endif //WASP_H
