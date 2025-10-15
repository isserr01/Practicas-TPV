#ifndef COLLISION_H
#define COLLISION_H

#include "Vector2D.h"
struct Collision {
	enum Type
	{
		NONE,
		ENEMY,
		PLATFORM,
	};

	Vector2D<float> speed; 
	Type type;

	Collision(Type t, Vector2D<float> speed = Vector2D<float>(0,0))
		: type(t), speed(speed) {
	}
	Collision()
		: type(NONE), speed(Vector2D<float>(0, 0)) {
	}

};;


#endif // COLLISION_H