#ifndef FROG_H
#define FROG_H
#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include <SDL3/SDL.h>
#include "Collision.h"
#include <istream>
#include "SceneObject.h"

constexpr float FROG_SPEED = 32.0;
constexpr int INITIAL_FRAME = 0;


class Game;
using Point2D = Vector2D<float>;
class Frog : public SceneObject
{

	Vector2D<float> speed;
	Vector2D<float> moveDir;
	int currentFrame;
	bool moving;
	int lives;
	Point2D initialPosition;

public:
	//constructor y destructor
	Frog(Game* game, std::istream& in);
	~Frog();
	//método de renderizado en pantalla
	void render() const override;
	//actualización de la posición y estado de la rana
	void update();
	//manejo de eventos de teclado
	void HandleEvents(const SDL_Event& event);
	//comprobar si está en casa
	bool isAtHome(const SDL_FRect& home);
	//manda a la rana a su posición inicial
	void resetPosition();
	//getter de vidas
	int getLives() const;

};
#endif // FROG_H
