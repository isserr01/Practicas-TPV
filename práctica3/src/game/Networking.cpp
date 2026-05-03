// This file is part of the course TPV2@UCM - Samir Genaim

#include "Networking.h"
#include "LittleWolf.h"
#include "../sdlutils/SDLUtils.h"
#include <iostream>
#include <cstring>

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

Networking::Networking()
    : _sock(nullptr), _client_Id(0), _master_Id(0), _game(nullptr),
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

    // Resolver dirección
    NET_Address* addr = NET_ResolveHostname(host);
    if (!addr) {
        std::cerr << "Failed to resolve hostname: " << host << std::endl;
        return false;
    }

    if (NET_WaitUntilResolved(addr, 5000) != NET_SUCCESS) {
        std::cerr << "Failed to resolve address" << std::endl;
        NET_UnrefAddress(addr);
        return false;
    }

    // Conectar
    _sock = NET_CreateClient(addr, port);
    if (!_sock) {
        std::cerr << "Failed to create client socket" << std::endl;
        NET_UnrefAddress(addr);
        return false;
    }

    NET_UnrefAddress(addr);

    // Esperar conexión
    void* conn[1] = { _sock };
    if (NET_WaitUntilInputAvailable(conn, 1, 5000) <= 0) {
        std::cerr << "Connection timeout" << std::endl;
        NET_DestroyStreamSocket(_sock);
        _sock = nullptr;
        return false;
    }

    // Recibir mensaje de aceptación
    SDLNetUtils::buff_t buf = SDLNetUtils::receive(_sock);
    if (!buf.error && buf.size > 0) {
        MsgWithMasterId msg;
        msg.deserialize(buf.data);

        if (msg.type == _CONN_REQUEST_ACCEPTED) {
            _client_Id = msg.clientId;
            _master_Id = msg.masterId;
            std::cout << "Connected with id " << (int)_client_Id
                << " (master is client " << (int)_master_Id << ")" << std::endl;

            send_info(100, 1, 0);
            return true;
        }
    }

    std::cerr << "Connection rejected" << std::endl;
    NET_DestroyStreamSocket(_sock);
    _sock = nullptr;
    return false;
}

void Networking::disconnect() {
    if (_sock) {
        Msg msg;
        msg.type = _CLIENT_DISCONNECTED;
        SDLNetUtils::serialized_send(msg, _sock);

        NET_DestroyStreamSocket(_sock);
        _sock = nullptr;
    }
    NET_Quit();
}

void Networking::update() {
    if (!_sock) return;

    while (true) {
        SDLNetUtils::buff_t buf = SDLNetUtils::receive(_sock);

        if (buf.error) {
            std::cerr << "Connection error" << std::endl;
            disconnect();
            return;
        }

        if (buf.size == 0) break;

        handle_message(buf);
    }
}

void Networking::handle_message(const SDLNetUtils::buff_t& buf) {
    Msg base;
    base.deserialize(const_cast<Uint8*>(buf.data));

    switch (base.type) {
    case _CLIENT_CONNECTED: {
        MsgWithMasterId msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_new_client(msg.clientId, msg.masterId);
        break;
    }
    case _CLIENT_DISCONNECTED: {
        MsgWithMasterId msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_disconnect(msg.clientId, msg.masterId);
        break;
    }
    case _PLAYER_STATE: {
        PlayerStateMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_player_state(msg);
        break;
    }
    case _PLAYER_INFO: {
        PlayerInfoMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_player_info(msg);
        break;
    }
    case _PLAYER_SHOOT: {
        ShootMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_shoot(msg);
        break;
    }
    case _PLAYER_DIED: {
        DeadMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_dead(msg);
        break;
    }
    case _PLAYER_HEALTH: {
        HealthMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_health(msg);
        break;
    }
    case _PLAYER_SCORE: {
        ScoreMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_score(msg);
        break;
    }
    case _RESTART_COUNTDOWN: {
        RestartCountdownMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_restart_countdown(msg);
        break;
    }
    case _RESTART:
        handle_restart();
        break;
    case _MASTER_CHANGED: {
        Uint8 newMaster;
        memcpy(&newMaster, buf.data + 1, 1);
        handle_master_changed(newMaster);
        break;
    }
    case _CURRENT_PLAYER: {
        CurrentPlayerMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_current_player(msg.playerId);
        break;
    }
    case _SOUND_EVENT: {
        SoundEventMsg msg;
        msg.deserialize(const_cast<Uint8*>(buf.data));
        handle_sound_event(msg);
        break;
    }
    default:
        std::cerr << "Unknown message type: " << (int)base.type << std::endl;
        break;
    }
}

// ============ ENVÍO DE MENSAJES ============

void Networking::send_state(const Vector2D& pos, float theta, float velX, float velY) {
    Uint32 now = SDL_GetTicks();
    if (now - _lastStateSend < 50) return;

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

    SDLNetUtils::serialized_send(msg, _sock);

    _lastX = pos.getX();
    _lastY = pos.getY();
    _lastTheta = theta;
    _lastStateSend = now;
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
    strncpy(msg.name, _playerName.c_str(), 10);
    msg.name[10] = '\0';

    SDLNetUtils::serialized_send(msg, _sock);
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

    SDLNetUtils::serialized_send(msg, _sock);
}

void Networking::send_dead(Uint8 id, Uint8 shooter, Uint32 timestamp) {
    DeadMsg msg;
    msg.type = _PLAYER_DIED;
    msg.victimId = id;
    msg.killerId = shooter;
    msg.timestamp = timestamp;

    SDLNetUtils::serialized_send(msg, _sock);
}

void Networking::send_health(Uint8 health) {
    HealthMsg msg;
    msg.type = _PLAYER_HEALTH;
    msg.clientId = _client_Id;
    msg.health = health;

    SDLNetUtils::serialized_send(msg, _sock);
}

void Networking::send_score(Sint32 score) {
    ScoreMsg msg;
    msg.type = _PLAYER_SCORE;
    msg.clientId = _client_Id;
    msg.score = score;

    SDLNetUtils::serialized_send(msg, _sock);
}

void Networking::send_restart() {
    Msg msg;
    msg.type = _RESTART;
    SDLNetUtils::serialized_send(msg, _sock);
}

void Networking::send_switch_player() {
    CurrentPlayerMsg msg;
    msg.type = _CURRENT_PLAYER;
    msg.playerId = _client_Id;
    SDLNetUtils::serialized_send(msg, _sock);
}

void Networking::send_sound_event(Uint8 soundType, float sourceX, float sourceY) {
    SoundEventMsg msg;
    msg.type = _SOUND_EVENT;
    msg.sourceId = _client_Id;
    msg.soundType = soundType;
    msg.sourceX = sourceX;
    msg.sourceY = sourceY;

    SDLNetUtils::serialized_send(msg, _sock);
}

void Networking::set_game_callback(LittleWolf* game) {
    _game = game;
}

// ============ HANDLERS ============

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
        _game->updatePlayerPosition(msg.clientId, msg.x, msg.y, msg.theta);
    }
}

void Networking::handle_player_info(const PlayerInfoMsg& msg) {
    if (msg.clientId != _client_Id && _game) {
        _game->updatePlayerRemote(msg.clientId, msg.x, msg.y, msg.theta,
            msg.state, msg.health, msg.score, msg.name);
    }
}

void Networking::handle_shoot(const ShootMsg& msg) {
    if (_game) {
        std::cout << "Shot from player " << (int)msg.shooterId << std::endl;
        // Reproducir sonido aquí
    }
}

void Networking::handle_dead(const DeadMsg& msg) {
    if (_game) {
        _game->onPlayerDied(msg.victimId, msg.killerId);
    }
}

void Networking::handle_health(const HealthMsg& msg) {
    if (msg.clientId != _client_Id && _game) {
        _game->updatePlayerHealth(msg.clientId, msg.health);
    }
}

void Networking::handle_score(const ScoreMsg& msg) {
    if (msg.clientId != _client_Id && _game) {
        _game->updatePlayerScore(msg.clientId, msg.score);
    }
}

void Networking::handle_restart_countdown(const RestartCountdownMsg& msg) {
    std::cout << "Game restarts in " << msg.seconds << " seconds" << std::endl;
    if (_game) {
        _game->showRestartCountdown(msg.seconds);
    }
}

void Networking::handle_restart() {
    std::cout << "Game restarting!" << std::endl;
    if (_game) {
        _game->restartGame();
    }
}

void Networking::handle_master_changed(Uint8 newMaster) {
    _master_Id = newMaster;
    std::cout << "New master: Player " << (int)newMaster << std::endl;
    if (_game) {
        _game->setMaster(_master_Id == _client_Id);
    }
}

void Networking::handle_current_player(Uint8 playerId) {
    if (_game) {
       // _game->setCurrentPlayer(playerId);
    }
}

void Networking::handle_sound_event(const SoundEventMsg& msg) {
    float volume = 1.0f;
    if (_game) {
        LittleWolf::Point localPos = _game->getLocalPosition();
        float dx = msg.sourceX - localPos.x;
        float dy = msg.sourceY - localPos.y;
        float distance = sqrtf(dx * dx + dy * dy);
        volume = std::max(0.0f, std::min(1.0f, 1.0f - distance / 20.0f));
    }

    if (volume > 0.05f) {
        const char* soundName = (msg.soundType == 0) ? "gunshot" : "pain";
        std::cout << "Sound: " << soundName << " volume: " << volume << std::endl;
    }
}