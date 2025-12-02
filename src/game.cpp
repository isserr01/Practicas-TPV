#include "game.h"

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include "PlayState.h"
#include "GameError.h"
#include "MainMenuState.h"
#include "texture.h"


using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Frogger 3.0";


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
	{"turtle.png",1,7},
	{"menuBackground.png"},
	{"texts/Avispado.png" },
	{"texts/CONTINUAR.png"},
	{"texts/ELIGE UN MAPA.png"},
	{"texts/GAME OVER.png"},
	{"texts/HAS GANADO.png"},
	{"texts/LEFT.png"},
	{"texts/RIGHT.png"},
	{"texts/Original.png"},
	{"texts/Practica1.png" },
	{"texts/REINICIAR.png"}, 
	{"texts/SALIR.png"},
	{"texts/Trivial.png"},
	{"texts/Veloz.png"},
	{"texts/VOLVER AL MENU.png"}
};

Game::Game()
	:
	exit(false),
	randomGenerator(std::random_device{}())
	
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
		textures[i] = new Texture(renderer, (std::string(imgBase) + name).c_str(), nrows, ncols);
		if (!textures[i])
			throw GameError("Error al cargar textura: " + std::string(name));
	}


	auto mainMenu = std::make_shared<MainMenuState>(this);
	pushState(mainMenu);
	
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

int Game::getRandomRange(int min, int max) {
	return uniform_int_distribution<int>(min, max)(randomGenerator);
}


void
Game::run()
{
	
	SDL_Event event;
	while (!exit) {
		uint32_t ini = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT)
				exit = true;
			else handleEvent(event);
		}
		
		update();
		render();

		//arreglo del framerate
		uint32_t t = SDL_GetTicks() - ini;

		if (t < FRAME_RATE)
			SDL_Delay(FRAME_RATE - t);
	}
	
}


