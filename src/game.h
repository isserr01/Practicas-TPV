#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include "gameStateMachine.h"
#include <random>

class Texture;
class PlayState;
class MainMenuState;

/**
 * Clase principal del juego.
 */
class Game : public GameStateMachine
{
public:

	using GameStateMachine::pushState;
	using GameStateMachine::popState;
	using GameStateMachine::replaceState;

	// Se actualiza el juego cada tantos milisegundos
	static constexpr int FRAME_RATE = 30;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 448;
	static constexpr int WINDOW_HEIGHT = 484;

	// Extremo inferior del río
	static constexpr int RIVER_LOW = 210;
	//extremos invisibles
	static constexpr float SIDE_LIMIT = 150.0f;
	//posiciones de las casas
	static constexpr float HOME_Y = 20; //colocacion de Y (altura) igual en todas
	static constexpr float HOME_X[5] = { 15,111,207,304,400 }; //colocacion de X (ancho) de cada casa
	//posiciones de los rectángulos de colisión de las casas
	static constexpr float HOME_RECT_X[5] = {20,116,212,309,405 };//colocacion de X (ancho) de cada rectangulo de colisión
	static constexpr float HOME_RECT_Y = 15; //colocacion de Y (altura) de cada rectángulo de colisión
	static constexpr float HOME_RECT_TAM = 10; //tamaño altura y ancho del rectángulo de colisión

	enum TextureName
	{
		FROG = 0,
		BACKGROUND,
		VEHICLE1,
		VEHICLE2,
		VEHICLE3,
		VEHICLE4,
		VEHICLE5,
		LOG1,
		LOG2,
		WASP,
		TURTLE,
		MAINMENU_BACKGROUND,
		AVISPADO,
		CONTINUAR,
		ELIGE_UN_MAPA,
		GAME_OVER,
		HAS_GANADO,
		LEFT,
		RIGHT,
		ORIGINAL,
		PRACTICA1,
		REINICIAR,
		SALIR,
		TRIVIAL,
		VELOZ,
		VOLVER_AL_MENU,
		NUM_TEXTURES
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;
	bool exit = false;
	std::mt19937_64 randomGenerator;
	MainMenuState* mainMenu;

public:
	Game();
	~Game();


	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;
	SDL_Renderer* getRenderer() const;
	SDL_Window* getWindow() const;
	//generador de numeros aleatorios
	int getRandomRange(int min, int max);

	// Ejecuta el bucle principal del juego
	void run();
	void quit() {
		exit = true;
	}



};


inline Texture* Game::getTexture(TextureName name) const { return textures[name];}
inline SDL_Renderer* Game::getRenderer() const{ return renderer;}

inline 	SDL_Window* Game::getWindow() const { return window; }

#endif // GAME_H