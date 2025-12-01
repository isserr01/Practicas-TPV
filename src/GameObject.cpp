#include "GameObject.h"

GameObject::GameObject(GameState* state) 
	: state(state)
{
	class SceneObject : public GameObject {};
}

GameObject::~GameObject()
{
}

