#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "GameState.h"
#include "EventHandler.h"


class GameState;
class GameObject
{
	
protected:
	GameState* state;
	
public:
	GameObject(GameState* state);
	virtual ~GameObject();
	//método de renderizado en pantalla
	virtual void render() const = 0;
	//actualización de la posición y estado
	virtual void update() = 0;

};

#endif // GAMEOBJECT_H