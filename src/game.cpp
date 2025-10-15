#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "texture.h"
#include "Vehicle.h"
#include "Log.h"
#include "Frog.h"
#include "Collision.h"

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
	{"log2.png"}
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
		throw "window: "s + SDL_GetError();

	renderer = SDL_CreateRenderer(window, nullptr);

	if (renderer == nullptr)
		throw "renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	for (size_t i = 0; i < textures.size(); i++) {
		auto [name, nrows, ncols] = textureList[i];
		textures[i] = new Texture(renderer, (string(imgBase) + name).c_str(), nrows, ncols);
	}

	//llamamos al método de cargar el mapa
	loadMap(MAP_FILE);

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
	for (auto l : logs) {
		l->render();
	}
	for(auto v : vehicles) {
		v->render();
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
	if (frog) frog->update();

	//generamos el rectángulo de la rana para comprobar colisiones
	SDL_FRect frogRect{
		frog->getPosition().getX(),
		frog->getPosition().getY(),
		(float)frog->getTexture()->getFrameWidth(),
		(float)frog->getTexture()->getFrameHeight()
	};
	//comprobamos colisiones llamando a checkCollision, tengo un console.log para comprobar
	if (checkCollision(frogRect).type==CollisionType::VEHICLE)
		std::cout << "frog ha colisionado con un vehículo \n";
	else if (checkCollision(frogRect).type == CollisionType::LOG)
		std::cout << "frog está sobre un tronco \n";


}

void
Game::run()
{

	while (!exit) {
		handleEvents();
		update();
		render();	

		SDL_Delay(16);
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
			frog->HandleEvents(event);
			//se compara la colisión de la rana con otros objetos
				
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
	//se comprueba la colisión con cada vehículo actualizando su estado
	//y llamando al método que comprueba si colisiona con el rectángulo dado
	for (auto v : vehicles) {
		if(v->checkCollision(rect).type == CollisionType::VEHICLE) {
			return Collision(CollisionType::VEHICLE); 
		}
	}
	
	//se comprueba si la rana está sobre un tronco utilizando un booleano auxiliar
	//de la misma manera que se hace con los vehículos
	for (auto l : logs) {
		if(l->checkCollision(rect).type == CollisionType::LOG) {
			return Collision(CollisionType::LOG, l->checkCollision(rect).logSpeed);
		}
	}

	//se comprueba si la rana ha caído al río comparando su posición con la de comienzo
	//del mismo y si no está sobre un tronco
	if (frog->getPosition().getY() < RIVER_LOW) {
		std::cout << "¡La rana cayó al río!" << std::endl;
		return Collision(CollisionType::NONE);
	}

	// TODO: cambiar el tipo de retorno a Collision e implementar
	return Collision(CollisionType::NONE);
}

/// <summary>
/// Método auxiliar para cargar el mapa desde un archivo de texto
/// </summary>
/// <param name="filename">Ruta del .txt del mapa</param>
void Game::loadMap(const std::string& filename)
{
	std::ifstream f(filename);
	//lanzamos excepción si no se encuentra el archivo
	if (!f) {
		std::cerr << "No se pudo abrir el mapa" << std::endl;
	}

	//leemos char a char
	char id;
	while (f >> id) {
		//casuística para cada tipo de objeto
		//ignorar comentarios
		if (id == '#') {
			f.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore rest of line
			continue;
		}
		//variables auxiliares para leer la posición de los objetos
		float x, y;
		//caso de la rana
		if (id == 'F') {
			int lives;
			f >> x >> y >> lives;
			//llamada al constructor
			frog = new Frog(this, Point2D(x, y), Vector2D<float>(32.0f, 32.0f),lives);
		}
		//caso de los vehículos
		else if (id == 'V') {
			float vx;
			int type;
			f >> x >> y >> vx >> type;
			//llamada al constructor y añadimos el elemento al array dinámico de vehículos
			Vehicle* v = new Vehicle(this, Point2D(x, y), Vector2D<float>(vx/20, 0.0f), type);
			vehicles.push_back(v);
		}
		//caso de los troncos
		else if (id == 'L') {
			float vx;
			int type;
			f >> x >> y >> vx >> type;
			//llamada al constructor y añadimos al array dinámico de troncos
			Log* l = new Log(this, Point2D(x, y), Vector2D<float>(vx/20, 0.0f),type);
			logs.push_back(l);
		}
		//excepción para id desconocidos
		else {
			std::cerr << "ID de objeto desconocido: " << id << std::endl;
		}

	}
}