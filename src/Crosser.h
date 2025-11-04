#ifndef CROSSER_H
#define CROSSER_H

#include "GameObject.h"
#include "SceneObject.h"
#include "vector2D.h"

using Point2D = Vector2D<float>;
class Crosser : public SceneObject
{
protected:
	Vector2D<float> speed;

public:
	Crosser(Game* game, Vector2D<float> s, Point2D position);
	void update();

};

#endif // CROSSER_H

