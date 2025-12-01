#include "GameObject.h"

GameObject::GameObject(GameState* game) 
	: gameState(game)
{
	class SceneObject : public GameObject {};
}

GameObject::~GameObject()
{
}

