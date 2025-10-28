#include "Log.h"


Log::Log(Game* game, std::istream& in)
	: game(game),
	texture(nullptr)
{

	float x, y, s;
	int t;

	if (in >> x >> y >> s >> t)
	{
		position = Point2D(x, y);
		speed = Vector2D<float>(s, 0.0f);

		//selecciona la textura según el tipo
		if (t == 0) texture = game->getTexture(Game::LOG1);
		else if (t == 1) texture = game->getTexture(Game::LOG2);
	}

}

Log::~Log()
{
	
}
void Log::render() const {
	int width = texture->getFrameWidth();
	int height = texture->getFrameHeight();
	texture->render(SDL_FRect{ position.getX(), position.getY() ,(float)width,(float)height });
}
void Log::update() {
	//regla de tres 
	position = position + speed * Game::FRAME_RATE * LOG_SPEED_FACTOR;

	//para que los troncos reaparezcan al salir de la pantalla
	if (position.getX() > Game::WINDOW_WIDTH + Game::SIDE_LIMIT) {
		position = Point2D(-Game::SIDE_LIMIT, position.getY());
	}
}
Collision Log::checkCollision(const SDL_FRect& other) const {	

	//rectángulo para comprobar colisión
	SDL_FRect rect = { position.getX(), position.getY(), 
		(float)texture->getFrameWidth(), (float)texture->getFrameHeight() };

	//si hay colisión, devuelve tipo plataforma y su velocidad
	if(SDL_HasRectIntersectionFloat(&rect, &other)) {
		return Collision(Collision::PLATFORM, speed);
	}
	return Collision(Collision::NONE);
	
}



