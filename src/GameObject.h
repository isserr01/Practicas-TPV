#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "game.h"


class GameObject
{
	
protected:
	Game* game;
	GameObject(Game* game);

public:
	virtual ~GameObject();
	//método de renderizado en pantalla
	virtual void render() const = 0;
	//actualización de la posición y estado
	virtual void update() = 0;

};

#endif // GAMEOBJECT_H