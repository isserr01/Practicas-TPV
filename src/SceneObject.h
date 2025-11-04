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
	SceneObject(Game* game);
	virtual ~SceneObject() = default;
	virtual void render() const override;
	virtual Collision checkCollision(const SDL_FRect& rect) = 0;
	

protected:
	Point2D position;
	Texture* texture;
	float dimY;
	float dimX;
	SDL_FRect getBoundingBox() const;

};

#endif //SCENEOBJECT_H