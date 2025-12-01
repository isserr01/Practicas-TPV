#include "game.h"

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "PlayState.h"
#include "GameError.h"
#include "GameError.h"


using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Frogger 3.0";
constexpr const char* const MAP_FILE = "../assets/maps/default.txt";

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr const char* const imgBase = "../assets/images/";

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"frog.png", 1, 2},
	{"background.png"},
	{"car1.png"},
	{"car2.png"},
	{"car3.png"},
	{"car4.png"},
	{"car5.png"},
	{"log1.png"},
	{"log2.png"},
	{"wasp.png"},
	{"turtle.png",1,7}
};

Game::Game()
	: exit(false)
{
	// Carga SDL y sus bibliotecas auxiliares
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(WINDOW_TITLE,
	                          WINDOW_WIDTH,
	                          WINDOW_HEIGHT,
	                          0);

	if (window == nullptr)
		throw SDLError("Error al cargar ventana");

	renderer = SDL_CreateRenderer(window, nullptr);

	if (renderer == nullptr)
		throw SDLError("Error al cargar el renderizador");

	// Carga las texturas al inicio
	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		try {
			textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
		}
		catch(const GameError& e) {
			throw SDLError("Error al cargar la textura " + string(name));
		}
	}
	pushState(std::make_shared<PlayState>(this, MAP_FILE));
}

Game::~Game()
{
	//liberamos memoria
	for(Texture* tex : textures)
		delete tex;
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window)SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_RenderClear(renderer);
	//renderizado del fondo
	GameStateMachine::render();

	//renderizado de los troncos, los coches y la rana llamando a sus métodos de render
	SDL_RenderPresent(renderer);
}


void
Game::run()
{
	SDL_Event event;
	while (!exit) {
		while (SDL_PollEvent(&event)) {
			handleEvents(event);
		}
		
		update();
		render();	

		SDL_Delay(FRAME_RATE);
	}
	
}

void Game::update()
{
	GameStateMachine::update();
}
void Game::handleEvents(SDL_Event& event)
{
	GameStateMachine::handleEvent(event);
}