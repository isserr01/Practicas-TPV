#include "Frog.h"
#include "iostream"
#include <fstream>


Frog::Frog(Game* game, std::istream& in)
	: game(game),
	currentFrame(INITIAL_FRAME),
	moving(false)
{
	float x, y;

	if (in >> x >> y >> lives)
	{
		position = Point2D(x, y);
		speed = Vector2D<float>(FROG_SPEED, FROG_SPEED);

	}

	texture = game->getTexture(Game::FROG);
}

Frog::~Frog()
{

}

void Frog::render() const {
	int width = texture->getFrameWidth();
	int height = texture->getFrameHeight();
	texture->renderFrame(
		SDL_FRect
		{ position.getX(), position.getY() 
		,(float)width,(float)height }, 0, currentFrame);

	//se gira la rana según la dirección de movimiento
	double angle = 0;
	SDL_FlipMode flip = SDL_FLIP_NONE;
	if (moveDir.getX() < 0) {
		angle = 270;
		flip = SDL_FLIP_HORIZONTAL;
	}
	else if (moveDir.getX() > 0) {
		angle = 90;
		flip = SDL_FLIP_NONE;
	}
	else if (moveDir.getY() < 0) {
		angle = 0;
		flip = SDL_FLIP_NONE;
	}
	else if (moveDir.getY() > 0) {
		angle = 180;
		flip = SDL_FLIP_NONE;
	}

	texture->renderFrame(
		SDL_FRect{ position.getX(), position.getY() ,(float)width,(float)height }, 0, currentFrame, angle, nullptr, flip);
}

void Frog::update() {

	//actualización de la posición según la dirección de movimiento
	//utilizo un booleano para que la rana se mueva solo una casilla por pulsación
	if (moving) {
		position = Point2D(position.getX() + moveDir.getX() * speed.getX(),
			position.getY() + moveDir.getY() * speed.getY());
	}
	moving = false;
	currentFrame = 0;	
 
	//comprobación de colisiones
	Collision col = game->checkCollision(SDL_FRect{
	position.getX(),position.getY(),
	(float)texture->getFrameWidth(),
	(float)texture->getFrameHeight()
	});
	
	//si es un enemigo, restamos una vida y reseteamos posición
	if (col.type == Collision::ENEMY) {
		std::cout << "Colisión con enemigo" << std::endl;
		lives--;
		resetPosition();
	}
	//si es una plataforma, la rana se mueve con ella
	else if (col.type == Collision::PLATFORM) {		
		position = position + col.speed * Game::FRAME_RATE * 0.001;
	}
	//si no hay colisión y está en el río, pierde una vida
	else if (col.type== Collision::NONE) {
		if (position.getY() < Game::RIVER_LOW && !game->CheckHomeCollision()) {
			std::cout << "Cayó al agua" << std::endl;
			lives--;
			resetPosition();
		}
	}
	//si sale de los límites de la ventana, pierde una vida
	if (position.getX() + texture->getFrameWidth() < 0 || position.getX() + texture->getFrameWidth() > Game::WINDOW_WIDTH ||
		position.getY() < 0 || position.getY() + texture->getFrameHeight() > Game::WINDOW_HEIGHT) {
		std::cout << "Fuera de los límites" << std::endl;
		lives--;
		resetPosition();
	}
	
}

void Frog::HandleEvents(const SDL_Event& event) {
	//comprobación de las teclas de dirección
	if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0) {
		if (event.key.key == SDLK_UP) {
			moveDir = Vector2D<float>(0, -1);
			moving = true;
		}
		else if (event.key.key == SDLK_DOWN) {
			moveDir = Vector2D<float>(0, 1);
			moving = true;
		}
		else if (event.key.key == SDLK_LEFT) {
			moveDir = Vector2D<float>(-1, 0);
			moving = true;
		}
		else if (event.key.key == SDLK_RIGHT) {
			moveDir = Vector2D<float>(1, 0);
			moving = true;
		}

	}
}
bool Frog::isAtHome(const SDL_FRect& home) {

	SDL_FRect rect = { position.getX(),position.getY(),(float)texture->getFrameWidth(),(float)texture->getFrameHeight() };
	if (SDL_HasRectIntersectionFloat(&rect, &home)) {
		return true;
	}
	else {
		return false;
	}
}
void Frog::resetPosition() {
	position = Point2D((Game::WINDOW_WIDTH - texture->getFrameWidth()) / 2.0f,
		Game::WINDOW_HEIGHT - texture->getFrameHeight() - 10.0f);
}
int Frog::getLives() const {
	return lives;
}



