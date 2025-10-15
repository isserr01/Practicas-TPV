#include "Frog.h"


Frog::Frog(Game* game, const Point2D& position, const Vector2D<float>& speed, int lives)
	: game(game)
	, texture(game->getTexture(Game::FROG))
	, position(position)
	, speed(speed)
	, currentFrame(0)
	, moving(false)
	, lives(lives)
{
}
void Frog::render() const {
	int width = texture->getFrameWidth();
	int height = texture->getFrameHeight();
	texture->renderFrame(
		SDL_FRect{ position.getX(), position.getY() ,(float)width,(float)height }, 0, currentFrame);


	double angle = 0;
	SDL_FlipMode flip = SDL_FLIP_NONE;
	if (moveDir.getX() < 0) {
		angle = 90;
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
		if (moving) {
			position = Point2D(position.getX() + moveDir.getX() * speed.getX(),
				position.getY() + moveDir.getY() * speed.getY());
		}
		moving = false;
		currentFrame = 0;
}
void Frog::HandleEvents(const SDL_Event& event) {
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




