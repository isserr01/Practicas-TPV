#ifndef CROSSER_H
#define CROSSER_H

#include "GameObject.h"
#include "SceneObject.h"
#include "GameState.h"	
#include "vector2D.h"
#include "game.h"

using Point2D = Vector2D<float>;
class Crosser : public SceneObject
{
protected:
	Vector2D<float> speed;

public:
	Crosser(GameState* state, Vector2D<float> s, Point2D position);
	void update();

};

#endif // CROSSER_H

