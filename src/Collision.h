#ifndef COLLISION_H
#define COLLISION_H

#include "Vector2D.h"
enum class CollisionType
{
	NONE,
	VEHICLE,
	LOG,
};

struct Collision {
	CollisionType type;
	Vector2D<float> logSpeed; 

	Collision(CollisionType t = CollisionType::NONE, Vector2D<float> speed = Vector2D<float>(0,0))
		: type(t), logSpeed(speed) {
	}

};;

#endif // COLLISION_H