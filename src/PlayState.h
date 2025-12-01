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
	std::list<SceneObject*> Objects;
	
	
	bool baseOcupada[5] = { false,false,false,false,false };


public:
	PlayState(Game* game, std::string& mapName);
	~PlayState();
	void render() const;
	void update();
	void handleEvents(SDL_Event event);
	std::list<SceneObject*> sceneObjects;

	// Comprueba si hay algún objeto colocado en ese rectángulo
	Collision checkCollision(const SDL_FRect& rect) const override;

	void loadMap(const std::string& filename);
	
	//comprueba si la rana ha llegado a casa
	bool CheckHomeCollision();
	//comprueba si se han completado todas las casas
	bool completeHome();
	//reinicia el estado del juego
	void reiniciarJuego();

};
#endif PLAYSTATE_H
