#include "GameObject.h"

GameObject::GameObject(Game* game) 
	: game(game)
{
	class SceneObject : public GameObject {};
}

GameObject::~GameObject()
{
}

