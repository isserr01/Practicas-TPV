#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#pragma once
#include "GameState.h"
#include <list>

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include "vector2D.h"

// Declaraciones anticipadas

class SceneObject;
class Frog;
class Collision;


//constantes
constexpr const char* const MAP_FILE = "../assets/maps/";

class PlayState : public  GameState
{
public:
	using AnchorScene = std::list<SceneObject*>::iterator;
private:
	//atributos para los punteros del juego
	Frog* frog = nullptr;
	std::list<SceneObject*> sceneObjects;
	std::string currentMap;
	
	bool baseOcupada[5] = { false,false,false,false,false };


public:
	PlayState(Game* game, const std::string& mapName);
	~PlayState();
	void render() const override;
	void update() override;
	void handleEvent(const SDL_Event& event) override;

	
	const std::string getCurrentMap() const {
		return currentMap;
	}
	// Comprueba si hay algún objeto colocado en ese rectángulo
	Collision checkCollision(const SDL_FRect& rect) const override;

	void loadMap(const std::string& filename);
	
	//comprueba si la rana ha llegado a casa
	bool CheckHomeCollision();
	//comprueba si se han completado todas las casas
	bool completeHome();


};
#endif //PLAYSTATE_H
