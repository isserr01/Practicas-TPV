#include "PlayState.h"
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
#include "PauseState.h"

using namespace std;
;

PlayState::PlayState(Game* game, std::string& mapName)
	: GameState (game)
{
	//llamamos al método de cargar el mapa
	loadMap(MAP_FILE + mapName);
}

PlayState::~PlayState()
{
	//liberamos memoria
	for (SceneObject* objs : Objects)
		delete objs;
}

void
PlayState::render() const
{
	//renderizado de los troncos, los coches y la rana llamando a sus métodos de render
	game->getTexture(Game::BACKGROUND)->render();
	for (SceneObject* obj : Objects) {
		obj->render();
	}

}

void
PlayState::update()
{
	//actualización de los objetos del juego llamando al metodo update de sceneobject
	for (SceneObject* obj : Objects) obj->update();

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
	for (SceneObject* obj : Objects) {
		wasp* a = dynamic_cast<wasp*>(obj);
		if (a && a->isAlive()) {
			hayAvispaViva = true;
			break;
		}

	}

	//generamos un número aleatorio para decidir si spawneamos una avispa
	int haySpawn = game->getRandomRange(0, 10);
	if (haySpawn == 3 && !hayAvispaViva) {
		int pos = game->getRandomRange(0, 4);
		if (!baseOcupada[pos]) {
			wasp* nuevaAvispa = new wasp(this, Point2D(0, 0));
			nuevaAvispa->SetPosition(pos);
			nuevaAvispa->spawn();
			Objects.push_back(nuevaAvispa);
		}
	}

	CheckHomeCollision();

	if(frog&& frog->getLives() <=0 ) {
		game->quit();
	}
}




void
PlayState::handleEvents(SDL_Event event)
{
	
	const bool* keystate = SDL_GetKeyboardState(NULL);
	if (keystate == NULL) {
		throw string("No se pudo obtener el estado del teclado: ") + SDL_GetError();
		return;
	}
	
		if (keystate) {
			if (keystate[SDL_SCANCODE_LSHIFT] && keystate[SDL_SCANCODE_LCTRL]) {
				reiniciarJuego();
			}

			if (event.type == SDL_EVENT_KEY_DOWN) {
				if (frog) {
					game->pushState(std::make_shared<PauseState>(game));
					//se delegan los eventos a la rana para comprobar el input
					//para el movimiento
					if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN
						|| event.key.key == SDLK_LEFT || event.key.key == SDLK_RIGHT)
					{
						frog->HandleEvents(event);

					}

				}
			}
		
	}
	
	


}
void PlayState::reiniciarJuego() {
	//eliminamos todos los objetos de la escena
	for (SceneObject* obj : Objects) {
		delete obj;
		Objects.clear();
	}
	frog = nullptr;
	//reseteamos las bases ocupadas
	for (bool& ocupado : baseOcupada) {
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
PlayState::checkCollision(const SDL_FRect& rect) const
{
	Collision col;

	for (SceneObject* obj : Objects) {
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
void PlayState::loadMap(const std::string& filename)
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
			Log* l = new Log(this, f);
			Objects.push_back(l);
		}
		else if (id == 'T') {
			TurtleGroup* t = new TurtleGroup(this, f);
			Objects.push_back(t);
		}

		//excepción si el identificador no existe
		else {
			throw FileFormatError(filename, "Identificador de objeto desconocido" + id);
		}

	}

}
bool PlayState::CheckHomeCollision() {

	if (!frog) return false;
	std::vector<Point2D> homePositions = { { Game::HOME_X[0],Game::HOME_Y},
		{ Game::HOME_X[1],Game::HOME_Y},
		{Game::HOME_X[2],Game::HOME_Y},
		{ Game::HOME_X[3],Game::HOME_Y},
		{ Game::HOME_X[4],Game::HOME_Y}
	};
	std::vector<SDL_FRect> homeRect = { {Game::HOME_RECT_X[0],Game::HOME_RECT_Y ,Game::HOME_RECT_TAM,Game::HOME_RECT_TAM},
		{Game::HOME_RECT_X[1],Game::HOME_RECT_Y  ,Game::HOME_RECT_TAM,Game::HOME_RECT_TAM},
		{Game::HOME_RECT_X[2],Game::HOME_RECT_Y  ,Game::HOME_RECT_TAM,Game::HOME_RECT_TAM},
		{ Game::HOME_RECT_X[3],Game::HOME_RECT_Y  ,Game::HOME_RECT_TAM,Game::HOME_RECT_TAM},
		{ Game::HOME_RECT_X[4],Game::HOME_RECT_Y  ,Game::HOME_RECT_TAM,Game::HOME_RECT_TAM} };

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
bool PlayState::completeHome() {
	int i = 0;
	bool complete = true;
	while (i < sizeof(baseOcupada) && complete) {
		if (!baseOcupada[i]) {
			complete = false;
		}
		i++;
	}
	return complete;
}