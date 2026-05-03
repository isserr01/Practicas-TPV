// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>
#include "../utils/Singleton.h"
#include "GameState.h"
#include "NewGameState.h"
#include "PausedState.h"
#include "RunningState.h"
#include "NewRoundState.h" 
#include "GameOverState.h"

#include "../ecs/ecs.h"

class Fighter;
class Bullets;
class Networking;

class Game : public Singleton<Game> {
	friend class Singleton<Game>;
public:

	enum State {
		NEWGAME, // just before starting a new game
		NEWROUND, // just before starting a new round
		PAUSED, // between rounds
		RUNNING, // playing
		GAMEOVER_LOST,
		GAMEOVER_WON
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
		case GAMEOVER_LOST:
			_state = _gameover_lost_state;
			break;
		case GAMEOVER_WON:
			_state = _gameover_won_state;
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
		_gameover_won_state = new GameOverState(true);
		_gameover_lost_state = new GameOverState(false);

		_state = _newgame_state;
		_state->enter();
	}

	ecs::EntityManager* getMngr() {
		return _mngr;
	}

	Fighter& get_fighters() {
		return *_fighters;
	}

	Bullets& get_bullets() {
		return *_bm;
	}

	inline Networking& get_networking() {
		return *_net;
	}

	void start();
	Game();
	virtual ~Game();
private:
	
	
	bool init();
	void check_collisions();

	//entitymanager and systems
	ecs::EntityManager* _mngr;

	//game states
	GameState* _state;
	GameState* _paused_state;
	GameState* _running_state;
	GameState* _newgame_state;
	GameState* _newround_state;
	GameState* _gameover_won_state;
	GameState* _gameover_lost_state;
	State _currentState;

	Bullets* _bm;
	Fighter* _fighters;
	Networking* _net;
};

