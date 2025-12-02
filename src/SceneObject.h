#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H
#include "GameObject.h"
#include "vector2D.h"
#include "texture.h"
#include "Collision.h"
#include <vector>
#include <SDL3/SDL.h>

using Point2D = Vector2D<float>;
class SceneObject : public GameObject
{
	
public:
	SceneObject(GameState* state);
	virtual ~SceneObject() = default;
	virtual void render() const override;
	virtual	void update() override = 0;
	virtual Collision checkCollision(const SDL_FRect& rect);

	// Nuevo método para avispas
	virtual bool isWasp() const { return false; }
	virtual bool isAlive() const { return true; }
	

protected:
	Point2D position;
	Texture* texture;
	float dimY;
	float dimX;
	SDL_FRect getBoundingBox() const;
	Game* game;

};

#endif //SCENEOBJECT_H