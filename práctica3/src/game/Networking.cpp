// This file is part of the course TPV2@UCM - Samir Genaim

#include "Networking.h"
#include "LittleWolf.h"
#include "../sdlutils/SDLUtils.h"
#include <iostream>
#include <cstring>

// Para evitar warnings de strncpy en Windows
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

Networking::Networking()
	: sock(nullptr), _client_Id(0), _master_Id(0), _game(nullptr),
	_lastStateSend(0), _lastX(0), _lastY(0), _lastTheta(0) {
}

Networking::~Networking() {
	disconnect();
}

bool Networking::init(const char* host, Uint16 port, const char* playerName) {
	_playerName = playerName;

	// Inicializar SDL_net
	if (NET_Init() < 0) {
		std::cerr << "NET_Init failed: " << SDL_GetError() << std::endl;
		return false;
	}

	// Resolver dirección del servidor
	NET_Address* addr = NET_ResolveHostname(host);
	if (!addr) {
		std::cerr << "Failed to resolve hostname: " << host << std::endl;
		return false;
	}

	// Esperar a que se resuelva la dirección
	if (NET_WaitUntilResolved(addr, 5000) != NET_SUCCESS) {
		std::cerr << "Failed to resolve address" << std::endl;
		NET_UnrefAddress(addr);
		return false;
	}

	// Crear socket y conectar
	sock = NET_CreateClient(addr, port);
	if (!sock) {
		std::cerr << "Failed to create client socket" << std::endl;
		NET_UnrefAddress(addr);
		return false;
	}

	// Esperar a que la conexión se establezca
	// NOTA: NET_WaitUntilResolved no funciona con NET_StreamSocket*
	// En su lugar, usamos un array de sockets para esperar actividad
	void* conn[1] = { sock };
	if (NET_WaitUntilInputAvailable(conn, 1, 5000) <= 0) {
		std::cerr << "Connection timeout" << std::endl;
		NET_DestroyStreamSocket(sock);
		sock = nullptr;
		NET_UnrefAddress(addr);
		return false;
	}

	NET_UnrefAddress(addr);

	// Recibir mensaje de aceptación del servidor
	Uint8 buffer[256];
	int received = NET_ReadFromStreamSocket(sock, buffer, sizeof(buffer));

	if (received > 0) {
		MsgWithMasterId msg;
		Uint8* p = buffer;
		p = msg.deserialize(p);

		if (msg.type == _CONN_REQUEST_ACCEPTED) {
			_client_Id = msg.clientId;
			_master_Id = msg.masterId;
			std::cout << "Connected with id " << (int)_client_Id
				<< " (master is client " << (int)_master_Id << ")" << std::endl;

			// Enviar información del jugador al servidor
			send_info(100, 1, 0);  // health=100, state=ALIVE, score=0

			return true;
		}
		else if (msg.type == _CONN_REQUEST_REJECTED) {
			std::cerr << "Connection request rejected by server" << std::endl;
		}
		else if (msg.type == _SERVER_SHUTDOWN) {
			std::cerr << "Server is shutting down" << std::endl;
		}
	}

	std::cerr << "Connection timeout or rejected" << std::endl;
	NET_DestroyStreamSocket(sock);
	sock = nullptr;
	return false;
}

void Networking::disconnect() {
	if (sock) {
		Msg msg;
		msg.type = _CLIENT_DISCONNECTED;
		send_message(msg);

		NET_DestroyStreamSocket(sock);
		sock = nullptr;
	}
	NET_Quit();
}

bool Networking::send_message(const Msg& msg, const void* extraData, size_t extraSize) {
	if (!sock) return false;

	// Calcular tamaño del mensaje base
	Uint8 tempBuffer[256];
	Uint8* p = const_cast<Msg&>(msg).serialize(tempBuffer);
	size_t msgSize = p - tempBuffer;

	// Crear buffer final
	std::vector<Uint8> buffer(msgSize + extraSize);
	memcpy(buffer.data(), tempBuffer, msgSize);
	if (extraData && extraSize > 0) {
		memcpy(buffer.data() + msgSize, extraData, extraSize);
	}

	return NET_WriteToStreamSocket(sock, buffer.data(), static_cast<int>(buffer.size()));
}

void Networking::send_state(const Vector2D& pos, float theta, float velX, float velY) {
	// Limitar frecuencia (50ms)
	Uint32 now = SDL_GetTicks();
	if (now - _lastStateSend < 50) return;

	// Solo enviar si hubo cambio significativo
	float dx = pos.getX() - _lastX;
	float dy = pos.getY() - _lastY;
	float dtheta = theta - _lastTheta;
	if (fabs(dx) < 0.01f && fabs(dy) < 0.01f && fabs(dtheta) < 0.01f) return;

	PlayerStateMsg msg;
	msg.type = _PLAYER_STATE;
	msg.clientId = _client_Id;
	msg.x = pos.getX();
	msg.y = pos.getY();
	msg.theta = theta;
	msg.velX = velX;
	msg.velY = velY;

	if (send_message(msg)) {
		_lastX = pos.getX();
		_lastY = pos.getY();
		_lastTheta = theta;
		_lastStateSend = now;
	}
}

void Networking::send_info(Uint8 health, Uint8 state, Sint32 score) {
	PlayerInfoMsg msg;
	msg.type = _PLAYER_INFO;
	msg.clientId = _client_Id;
	msg.x = 0;
	msg.y = 0;
	msg.theta = 0;
	msg.state = state;
	msg.health = health;
	msg.score = score;
	// Copiar nombre de forma segura
	strncpy(msg.name, _playerName.c_str(), 10);
	msg.name[10] = '\0';
	send_message(msg);
}

void Networking::send_shoot(Vector2D origin, Vector2D direction) {
	ShootMsg msg;
	msg.type = _PLAYER_SHOOT;
	msg.shooterId = _client_Id;
	msg.originX = origin.getX();
	msg.originY = origin.getY();
	msg.dirX = direction.getX();
	msg.dirY = direction.getY();
	msg.timestamp = SDL_GetTicks();
	send_message(msg);
}

void Networking::send_dead(Uint8 id, Uint8 shooter, Uint32 timestamp) {
	DeadMsg msg;
	msg.type = _PLAYER_DIED;
	msg.victimId = id;
	msg.killerId = shooter;
	msg.timestamp = timestamp;
	send_message(msg);
}

void Networking::send_health(Uint8 health) {
	HealthMsg msg;
	msg.type = _PLAYER_HEALTH;
	msg.clientId = _client_Id;
	msg.health = health;
	send_message(msg);
}

void Networking::send_score(Sint32 score) {
	ScoreMsg msg;
	msg.type = _PLAYER_SCORE;
	msg.clientId = _client_Id;
	msg.score = score;
	send_message(msg);
}

void Networking::send_restart() {
	Msg msg;
	msg.type = _RESTART;
	send_message(msg);
}

void Networking::send_switch_player() {
	CurrentPlayerMsg msg;
	msg.type = _CURRENT_PLAYER;
	msg.playerId = _client_Id;
	send_message(msg);
}

void Networking::send_sound_event(Uint8 soundType, float sourceX, float sourceY) {
	SoundEventMsg msg;
	msg.type = _SOUND_EVENT;
	msg.sourceId = _client_Id;
	msg.soundType = soundType;
	msg.sourceX = sourceX;
	msg.sourceY = sourceY;
	send_message(msg);
}

void Networking::set_game_callback(LittleWolf* game) {
	_game = game;
}

void Networking::update() {
	if (!sock) return;

	while (true) {
		// Verificar si hay datos disponibles
		void* conn[1] = { sock };
		if (NET_WaitUntilInputAvailable(conn, 1, 0) <= 0) {
			break;  // No hay datos
		}

		// Leer header
		Uint8 headerBuffer[2];
		int received = NET_ReadFromStreamSocket(sock, headerBuffer, sizeof(headerBuffer));

		if (received == -1) {
			std::cerr << "Connection error" << std::endl;
			disconnect();
			return;
		}

		if (received == 0) break;

		Uint8 type = headerBuffer[0];

		// Procesar según el tipo
		switch (type) {
		case _CLIENT_DISCONNECTED: {
			MsgWithMasterId msg;
			Uint8 moreData[2];
			if (NET_ReadFromStreamSocket(sock, moreData, sizeof(moreData)) != sizeof(moreData)) {
				return;
			}
			Uint8* p = moreData;
			p = msg.deserialize(p);
			if (type == _CLIENT_CONNECTED) {
				handle_new_client(msg.clientId, msg.masterId);
			}
			else if (type == _CLIENT_DISCONNECTED) {
				handle_disconnect(msg.clientId, msg.masterId);
			}
			break;
		}

		case _PLAYER_STATE: {
			PlayerStateMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(PlayerStateMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_player_state(msg);
			break;
		}

		case _PLAYER_INFO: {
			PlayerInfoMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(PlayerInfoMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_player_info(msg);
			break;
		}

		case _PLAYER_SHOOT: {
			ShootMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(ShootMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_shoot(msg);
			break;
		}

		case _PLAYER_DIED: {
			DeadMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(DeadMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_dead(msg);
			break;
		}

		case _PLAYER_HEALTH: {
			HealthMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(HealthMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_health(msg);
			break;
		}

		case _PLAYER_SCORE: {
			ScoreMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(ScoreMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_score(msg);
			break;
		}

		case _RESTART_COUNTDOWN: {
			RestartCountdownMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(RestartCountdownMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_restart_countdown(msg);
			break;
		}

		case _RESTART:
			handle_restart();
			break;

		case _MASTER_CHANGED: {
			Uint8 newMaster;
			if (NET_ReadFromStreamSocket(sock, &newMaster, 1) != 1) {
				return;
			}
			handle_master_changed(newMaster);
			break;
		}

		case _CURRENT_PLAYER: {
			CurrentPlayerMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(CurrentPlayerMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_current_player(msg.playerId);
			break;
		}

		case _SOUND_EVENT: {
			SoundEventMsg msg;
			Uint8* p = reinterpret_cast<Uint8*>(&msg);
			int remaining = sizeof(SoundEventMsg) - 1;
			if (NET_ReadFromStreamSocket(sock, p + 1, remaining) != remaining) {
				return;
			}
			handle_sound_event(msg);
			break;
		}

		case _CONN_REQUEST_REJECTED:
			std::cerr << "Connection rejected by server" << std::endl;
			disconnect();
			return;

		case _SERVER_SHUTDOWN:
			std::cerr << "Server is shutting down" << std::endl;
			disconnect();
			return;

		default:
			std::cerr << "Unknown message type: " << (int)type << std::endl;
			break;
		}
	}
}

void Networking::handle_new_client(Uint8 id, Uint8 masterId) {
	_master_Id = masterId;
	std::cout << "New client connected: " << (int)id << std::endl;
}

void Networking::handle_disconnect(Uint8 id, Uint8 masterId) {
	_master_Id = masterId;
	std::cout << "Client disconnected: " << (int)id << std::endl;
}

void Networking::handle_player_state(const PlayerStateMsg& msg) {
	if (msg.clientId != _client_Id && _game) {
		// Actualizar posición del jugador remoto
		Vector2D pos(msg.x, msg.y);
		Vector2D vel(msg.velX, msg.velY);
		// _game->updatePlayerPosition(msg.clientId, pos, msg.theta, vel);
	}
}

void Networking::handle_player_info(const PlayerInfoMsg& msg) {
	if (msg.clientId != _client_Id && _game) {
		// Añadir o actualizar información del jugador
		Vector2D pos(msg.x, msg.y);
		// _game->updatePlayerInfo(msg.clientId, pos, msg.theta,
		//                         msg.state, msg.health, msg.score, msg.name);
	}
}

void Networking::handle_shoot(const ShootMsg& msg) {
	if (_game) {
		// Crear evento de sonido para el disparo
		SoundEventMsg soundMsg;
		soundMsg.type = _SOUND_EVENT;
		soundMsg.sourceId = msg.shooterId;
		soundMsg.soundType = 0;  // disparo
		soundMsg.sourceX = msg.originX;
		soundMsg.sourceY = msg.originY;
		handle_sound_event(soundMsg);
	}
}

void Networking::handle_dead(const DeadMsg& msg) {
	if (_game) {
		// Marcar jugador como muerto
		// _game->onPlayerDied(msg.victimId, msg.killerId);

		// Crear evento de sonido para la muerte
		if (msg.victimId != _client_Id) {
			SoundEventMsg soundMsg;
			soundMsg.type = _SOUND_EVENT;
			soundMsg.sourceId = msg.victimId;
			soundMsg.soundType = 1;  // muerte
			soundMsg.sourceX = 0;
			soundMsg.sourceY = 0;
			handle_sound_event(soundMsg);
		}
	}
}

void Networking::handle_health(const HealthMsg& msg) {
	if (msg.clientId != _client_Id && _game) {
		// _game->updatePlayerHealth(msg.clientId, msg.health);
	}
}

void Networking::handle_score(const ScoreMsg& msg) {
	if (msg.clientId != _client_Id && _game) {
		// _game->updatePlayerScore(msg.clientId, msg.score);
	}
}

void Networking::handle_restart_countdown(const RestartCountdownMsg& msg) {
	std::cout << "Game restarts in " << msg.seconds << " seconds" << std::endl;
	if (_game) {
		// _game->onRestartCountdown(msg.seconds);
	}
}

void Networking::handle_restart() {
	std::cout << "Game restarting!" << std::endl;
	if (_game) {
		// _game->bringAllToLife();
	}
}

void Networking::handle_master_changed(Uint8 newMaster) {
	_master_Id = newMaster;
	std::cout << "New master: Player " << (int)newMaster << std::endl;
}

void Networking::handle_current_player(Uint8 playerId) {
	if (_game) {
		// _game->setCurrentPlayer(playerId);
	}
}

void Networking::handle_sound_event(const SoundEventMsg& msg) {
	// Calcular volumen basado en distancia
	float volume = 1.0f;

	if (_game) {
		// Obtener posición del jugador local y calcular distancia
		// Vector2D sourcePos(msg.sourceX, msg.sourceY);
		// Vector2D localPos = _game->getCurrentPlayerPosition();
		// float distance = sourcePos.distance(localPos);
		// volume = std::max(0.0f, std::min(1.0f, 1.0f - distance / 20.0f));
	}

	const char* soundName = (msg.soundType == 0) ? "gunshot" : "pain";
	// sdlutils().soundEffects().at(soundName).play("se", volume);
}