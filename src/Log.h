#ifndef LOG_H
#define LOG_H
#include "game.h"
#include "vector2D.h"
#include "texture.h"
#include <SDL3/SDL.h>
#include "Collision.h"
#include <istream>
#include "Crosser.h"
#include "GameObject.h"
#include "Platform.h"


class Game;

class Log : public Platform
{

public:
	//constructora
	Log(Game* game, std::istream& in);
	virtual ~Log() = default;

};
#endif // LOG_H
