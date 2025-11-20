#include "game.h"

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>

#include "wasp.h"
#include "texture.h"
#include "Vehicle.h"
#include "Log.h"
#include "Frog.h"
#include "Collision.h"
#include "HomedFrog.h"
#include "TurtleGroup.h"
#include "GameError.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Frogger 1.0";
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
	: exit(false),
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
		try {
			textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
		}
		catch(const GameError& e) {
			throw SDLError("Error al cargar la textura " + string(name));
		}
	}

	//llamamos al método de cargar el mapa
	loadMap(MAP_FILE);
	
	
	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
}

Game::~Game()
{
	//liberamos memoria
	for(Texture* tex : textures)
		delete tex;
	for(SceneObject* objs : Objects)
		delete objs;
	if (renderer) SDL_DestroyRenderer(renderer);
	if (window)SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_RenderClear(renderer);
	//renderizado del fondo
	textures[BACKGROUND]->render();

	//renderizado de los troncos, los coches y la rana llamando a sus métodos de render

	for(SceneObject* obj : Objects) {
		obj->render();
	}

	SDL_RenderPresent(renderer);
}

void
Game::update()
{
	//actualización de los objetos del juego llamando al metodo update de sceneobject
	for(SceneObject* obj :Objects ) obj->update();
	
	//eliminamos las avispas que ya no están vivas
	std::erase_if(Objects, [](SceneObject* obj) {
		wasp* a = dynamic_cast<wasp*>(obj);
		if (a && !a->isAlive()) {
			delete obj;
			return true;
		}
		return false;
		});

	//lógica de spawn de avispas
	bool hayAvispaViva = false;
	for(SceneObject* obj : Objects) {
		wasp* a = dynamic_cast<wasp*>(obj);
		if (a&&a->isAlive()) {
			hayAvispaViva = true;
			break;
		}
		
	}

	//generamos un número aleatorio para decidir si spawneamos una avispa
	int haySpawn = getRandomRange(0, 10);
	if (haySpawn == 3 && !hayAvispaViva) {
		int pos = getRandomRange(0, 4);
		if (!baseOcupada[pos]) {
			wasp* nuevaAvispa = new wasp(this, Point2D(0, 0));
			nuevaAvispa->SetPosition(pos);
			nuevaAvispa->spawn();
			Objects.push_back(nuevaAvispa);
		}
	}
	
	
	CheckHomeCollision();
	
}

int Game::getRandomRange(int min, int max) {
	return uniform_int_distribution<int>(min, max)(randomGenerator);
}

void
Game::run()
{

	while (!exit && frog->getLives()>0 && !completeHome()) {
		handleEvents();
		update();
		render();	

		SDL_Delay(FRAME_RATE);
	}
	
}

void
Game::handleEvents()
{
	const bool* keystate = SDL_GetKeyboardState(NULL);
	if(keystate==NULL) {
		throw string("No se pudo obtener el estado del teclado: ") + SDL_GetError();
		return;
	}
	// Only quit is handled directly, everything else is delegated
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				exit = true;
			}
			
			else if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
				if (frog) {
					//se delegan los eventos a la rana para comprobar el input
					//para el movimiento
					if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN
						|| event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT)
					{
						frog->HandleEvents(event);
						
					}

				}
			}
			if (keystate[SDL_SCANCODE_LSHIFT] && keystate[SDL_SCANCODE_LCTRL] && keystate[SDL_SCANCODE_0]) {
				reiniciarJuego();
			}
		}
		
		
}
void Game:: reiniciarJuego() {
	//eliminamos todos los objetos de la escena
	for(SceneObject* obj : Objects) {
		delete obj;
		Objects.clear();
	}
	frog = nullptr;
	//reseteamos las bases ocupadas
	for(bool& ocupado : baseOcupada) {
		ocupado = false;
	}
	//recargamos el mapa
	loadMap(MAP_FILE);
}
/// <summary>
/// Método que comprueba si hay colisiones entre un objeto y el rectángulo generado por la rana
/// </summary>
/// <param name="rect">Rectángulo generado por el sprite de la rana</param>
/// <returns></returns>
Collision
Game::checkCollision(const SDL_FRect& rect) const
{
	Collision col;

	for (SceneObject* obj: Objects) {
		col = obj->checkCollision(rect);
		if (col.type != Collision::NONE) {

			return col;
		}
	}
	
	return col;
}

/// <summary>
/// Método para cargar el mapa desde un archivo de texto
/// </summary>
/// <param name="filename">Ruta del .txt del mapa</param>
void Game::loadMap(const std::string& filename)
{
	std::ifstream f(filename);
	//lanzamos excepción si no se encuentra el archivo
	if (!f) {
		throw FileNotFoundError(filename);
	}

	//leemos char a char
	char id;
	while (f >> id) {
		//casuística para cada tipo de objeto
		//ignorar comentarios
		if (id == '#') {
			f.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		//caso de la rana
		if (id == 'F') {
			//llamada al constructor
			frog = new Frog(this, f);
			Objects.push_back(frog);
		}
		//caso de los vehículos
		else if (id == 'V') {
			//llamada al constructor y añadimos el elemento al array dinámico de vehículos
			Vehicle* v = new Vehicle(this, f);
			Objects.push_back(v);
		}
		//caso de los troncos
		else if (id == 'L') {
			//llamada al constructor y añadimos al array dinámico de troncos
			Log* l = new Log(this,f);
			Objects.push_back(l);
		}
		else if (id == 'T') {
			TurtleGroup* t = new TurtleGroup(this, f);
			Objects.push_back(t);
		}

		//excepción si el identificador no existe
		else {
			throw FileFormatError(filename,"Identificador de objeto desconocido"+id);
		}

	}

}
bool Game::CheckHomeCollision() {

	if (!frog) return false;
	std::vector<Point2D> homePositions = { { HOME_X[0],HOME_Y},
		{ HOME_X[1],HOME_Y},
		{ HOME_X[2],HOME_Y},
		{ HOME_X[3],HOME_Y},
		{ HOME_X[4],HOME_Y}
	};
	std::vector<SDL_FRect> homeRect = { { HOME_RECT_X[0],HOME_RECT_Y ,HOME_RECT_TAM,HOME_RECT_TAM},
		{ HOME_RECT_X[1],HOME_RECT_Y  ,HOME_RECT_TAM,HOME_RECT_TAM},
		{ HOME_RECT_X[2],HOME_RECT_Y  ,HOME_RECT_TAM,HOME_RECT_TAM},
		{ HOME_RECT_X[3],HOME_RECT_Y  ,HOME_RECT_TAM,HOME_RECT_TAM},
		{ HOME_RECT_X[4],HOME_RECT_Y  ,HOME_RECT_TAM,HOME_RECT_TAM} };

	for (int i = 0; i < homePositions.size(); i++) {
		if (frog->isAtHome(homeRect[i]) && !baseOcupada[i]) {
			Objects.push_back(new HomedFrog(this, homePositions[i]));
			frog->resetPosition();
			baseOcupada[i] = true;
			return true;
		}
	}
	return false;
}
bool Game::completeHome(){
	int i = 0;
	bool complete = true;
	while(i< sizeof(baseOcupada) && complete) {
		if(!baseOcupada[i]) {
			complete = false;
		}
		i++;
	}
	return complete;
}