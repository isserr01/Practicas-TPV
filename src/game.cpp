#include "game.h"

#include <string>

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
		throw "window: "s + SDL_GetError();

	renderer = SDL_CreateRenderer(window, nullptr);

	if (renderer == nullptr)
		throw "renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		try {
			textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
		}
		catch(const string& e) {
			throw string("No se pudo cargar la textura ") + name + ": " + e;
		}
	}

	//llamamos al método de cargar el mapa
	loadMap(MAP_FILE);

	if(MAP_FILE == nullptr) {
		throw string("No se pudo cargar el mapa ") + MAP_FILE;
	}

	avispa = new wasp(this, textures[WASP], Point2D(0, 0));
	
	
	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
}

Game::~Game()
{
	//liberamos memoria
	for(auto tex : textures)
		delete tex;
	for(auto v : vehicles)
		delete v;
	for (auto l : logs)
		delete l;
	delete frog;
	delete avispa;
	for (auto hr : homedFrogs)
		delete hr;
	for (auto t : turtles)
		delete t;
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

	if (avispa) avispa->render();

	for (auto l : logs) {
		l->render();
	}
	for(auto v : vehicles) {
		v->render();
	}
	
	for(auto hr : homedFrogs) {
		hr->render();
	}
	for (auto t : turtles) {
		t->render();
	}
	if (frog) frog->render();


	SDL_RenderPresent(renderer);
}

void
Game::update()
{
	//actualización de los objetos del juego llamando a sus métodos de update
	for(auto v : vehicles) v->update();
	for (auto l : logs) l->update();
	for (auto t : turtles) t->update();
	if (avispa) avispa->update();
	if (frog) frog->update();
	
	int haySpawn = getRandomRange(0, 10);
	if (haySpawn == 3 && !avispa->isAlive()) {
		int pos = getRandomRange(0, 4);
		avispa->SetPosition(pos);

		if (!baseOcupada[avispa->getPosition()])
			avispa->spawn();
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
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;
		else if (frog) {
			//se delegan los eventos a la rana para comprobar el input
			//para el movimiento
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
				if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN 
					|| event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT)
				{
					frog->HandleEvents(event);
				}
			}	
		}

	}
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

	for (auto v : vehicles) {
		col = v->checkCollision(rect);
		if (col.type != Collision::NONE && col.type == Collision::ENEMY) {

			return col;
		}
	}
	for (auto l : logs) {
		col = l->checkCollision(rect);
		if (col.type != Collision::NONE && col.type == Collision::PLATFORM) {

			return col;
		}
	}
	for (auto t : turtles) {
		col = t->checkCollision(rect);
		if (col.type != Collision::NONE && col.type == Collision::PLATFORM) {
			return col;
		}
			
	}
	if (avispa) {
		col = avispa->checkCollision(rect);
		if (col.type != Collision::NONE && col.type == Collision::ENEMY) {

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
		throw string("No se pudo abrir el mapa" + filename);
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
		}
		//caso de los vehículos
		else if (id == 'V') {
			//llamada al constructor y añadimos el elemento al array dinámico de vehículos
			Vehicle* v = new Vehicle(this, f);
			vehicles.push_back(v);
		}
		//caso de los troncos
		else if (id == 'L') {
			//llamada al constructor y añadimos al array dinámico de troncos
			Log* l = new Log(this,f);
			logs.push_back(l);
		}
		else if (id == 'T') {
			TurtleGroup* t = new TurtleGroup(this, f);
			turtles.push_back(t);
		}

		//excepción si el identificador no existe
		else {
			cout << "ID de objeto desconocido: " << id << std::endl;
		}

	}

}
bool Game::CheckHomeCollision() {

	if (!frog) return false;
	std::vector<Point2D> homePositions = { { HOME_RECT[0],HOME_Y},
		{ HOME_RECT[1],HOME_Y},
		{ HOME_RECT[2],HOME_Y},
		{ HOME_RECT[3],HOME_Y},
		{ HOME_RECT[4],HOME_Y}
	};
	std::vector<SDL_FRect> homeRect = { 
		{ HOME_RECT[0],HOME_Y,HOME_RECT_X,HOME_RECT_X},
		{ HOME_RECT[5],HOME_Y ,HOME_RECT_X,HOME_RECT_X},
		{ HOME_RECT[6],HOME_Y ,HOME_RECT_X,HOME_RECT_X},
		{ HOME_RECT[7],HOME_Y ,HOME_RECT_X,HOME_RECT_X},
		{ HOME_RECT[4],HOME_Y ,HOME_RECT_X,HOME_RECT_X}
	};
	for (int i = 0; i < homePositions.size(); i++) {
		if (frog->isAtHome(homeRect[i]) && !baseOcupada[i]) {
			homedFrogs.push_back(new HomedFrog(this, textures[FROG],homePositions[i]));
			frog->resetPosition();
			baseOcupada[i] = true;
			return true;
			break;
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