// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../utils/Singleton.h"
#include "../components/GameState.h"
#include "../components/NewGameState.h"
#include "../components/PausedState.h"
#include "../components/RunningState.h"
#include "../components/NewRoundState.h" 
#include "../ecs/EntityManager.h"
//#include "../components/GameOverState.h"

namespace ecs {
class EntityManager;
}

class Transform;
class GameState;

class Game : public Singleton<Game>{
	friend class Singleton<Game>;
public:
	enum State {
		NEWGAME, // just before starting a new game
		NEWROUND, // just before starting a new round
		PAUSED, // between rounds
		RUNNING, // playing
		GAMEOVER // game over
	};

	inline void setState(State s) {

		assert(_state != nullptr);
		_state->leave();

		_currentState = s;
		switch (s) {
		case NEWGAME:
			_state = _newgame_state;
			break;
		case NEWROUND:
			_state = _newround_state;
			break;
		case PAUSED:
			_state = _paused_state;
			break;
		case RUNNING:
			_state = _running_state;
			break;
		case GAMEOVER:
			_state = _gameover_state;
			break;
		default:
			break;
		}
		_state->enter();

	}

	inline void initGame() {
		_mngr = new ecs::EntityManager();

		_newgame_state = new NewGameState();
		_newround_state = new NewRoundState();
		_paused_state = new PausedState();
		_running_state = new RunningState();
		//_gameover_state = new GameOverState();

		_state = _newgame_state;
		_state->enter();
	}

	ecs::EntityManager* getMngr() {
		return _mngr;
	}

	State getCurrentState() const {
		return _currentState;
	}

	virtual ~Game();
	bool init();
	void start();
private:
	Game();
	void checkCollisions();
	ecs::EntityManager *_mngr;

	Transform* _fighter;
	std::vector<Transform*> _objs;

	GameState *_state;
	GameState* _paused_state;
	GameState* _running_state;
	GameState* _newgame_state;
	GameState* _newround_state;
	GameState* _gameover_state;
	State _currentState;

};

