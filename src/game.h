#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include "vector2D.h"
#include <random>
// Declaraciones anticipadas
class Texture;
class Vehicle;
class Log;
class Frog;
class Collision;
class wasp;
class HomedFrog;

/**
 * Clase principal del juego.
 */
class Game
{

public:
	// Se actualiza el juego cada tantos milisegundos
	static constexpr int FRAME_RATE = 30;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 448;
	static constexpr int WINDOW_HEIGHT = 484;
	// Extremo inferior del río
	static constexpr int RIVER_LOW = 210;

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
		NUM_TEXTURES
	};
	
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;
	bool exit;
	//atributos para los punteros del juego
	Frog* frog = nullptr;
	std::vector<Log*> logs;
	std::vector<Vehicle*> vehicles;
	wasp* avispa = nullptr;
	std::mt19937_64 randomGenerator;
	std::vector<HomedFrog*> homedFrogs;
	//métodos privados de la clase
	void render() const;
	void update();
	void handleEvents();
	bool baseOcupada [5]= { false,false,false,false,false };

public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	Collision checkCollision(const SDL_FRect& rect) const;

	void loadMap(const std::string& filename);
	//generador de numeros aleatorios
	int getRandomRange(int min, int max);
	void CheckHomeCollision();
	bool completeHome();
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
