#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.h"
#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include "vector2D.h"
#include <random>
#include <list>
#include "game.h"


class Texture;
class SceneObject;
class Vehicle;
class Log;
class Frog;
class Collision;
class wasp;
class HomedFrog;
class TurtleGroup;

class PlayState : public GameState
{

public:
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
	static constexpr float HOME_RECT_X[5] = { 20,116,212,309,405 };//colocacion de X (ancho) de cada rectangulo de colisión
	static constexpr float HOME_RECT_Y = 15; //colocacion de Y (altura) de cada rectángulo de colisión
	static constexpr float HOME_RECT_TAM = 10; //tamaño altura y ancho del rectángulo de colisión

	

private:
		SDL_Window* window;
		SDL_Renderer* renderer;
		std::array<Texture*, Game::NUM_TEXTURES> textures;
		bool exit;
		//atributos para los punteros del juego
		Frog* frog = nullptr;
		std::list<SceneObject*> Objects;
		std::mt19937_64 randomGenerator;

		//métodos privados de la clase
		void render() const override;
		void update() override;
		void handleEvent(const SDL_Event& event) override;
		bool baseOcupada[5] = { false,false,false,false,false };
		//reinicia el estado del juego
		void reiniciarJuego();

public:
		PlayState();
		~PlayState();

		// Ejecuta el bucle principal del juego
		void run();

		// Comprueba si hay algún objeto colocado en ese rectángulo
		Collision checkCollision(const SDL_FRect& rect) const;

		void loadMap(const std::string& filename);
		//generador de numeros aleatorios
		int getRandomRange(int min, int max);
		//comprueba si la rana ha llegado a casa
		bool CheckHomeCollision();
		//comprueba si se han completado todas las casas
		bool completeHome();

};

#endif // PLAYSTATE_H