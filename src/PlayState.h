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
#include <random>
// Declaraciones anticipadas

class SceneObject;
class Frog;
class Collision;


class PlayState : public  GameState
{
public:
	using AnchorScene = std::list<SceneObject*>::iterator;
private:
	//atributos para los punteros del juego
	Frog* frog = nullptr;
	std::list<SceneObject*> Objects;
	std::mt19937_64 randomGenerator;
	
	bool baseOcupada[5] = { false,false,false,false,false };


public:
	PlayState(Game* game);
	~PlayState();
	void render() const;
	void update();
	void handleEvents();
	std::list<SceneObject*> sceneObjects;

	// Comprueba si hay algún objeto colocado en ese rectángulo
	Collision checkCollision(const SDL_FRect& rect) const;

	void loadMap(const std::string& filename);
	//generador de numeros aleatorios
	int getRandomRange(int min, int max);
	//comprueba si la rana ha llegado a casa
	bool CheckHomeCollision();
	//comprueba si se han completado todas las casas
	bool completeHome();
	//reinicia el estado del juego
	void reiniciarJuego();

};
#endif PLAYSTATE_H
