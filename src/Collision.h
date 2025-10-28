#ifndef COLLISION_H
#define COLLISION_H

#include "vector2D.h"
struct Collision {
	//tipo de colisión
	enum Type
	{
		NONE,
		ENEMY,
		PLATFORM,
	};

	//velocidad (para las plataformas)
	Vector2D<float> speed; 
	//tipo de colisión basada en el enum
	Type type;

	//constructores
	Collision(Type t, Vector2D<float> speed = Vector2D<float>(0,0))
		: type(t), speed(speed) {
	}
	Collision()
		: type(NONE), speed(Vector2D<float>(0, 0)) {
	}

};;


#endif // COLLISION_H