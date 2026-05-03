// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include <string>
#include <functional>
#include "SDLNetUtils.h"
#include "messages.h"

class LittleWolf;
class Vector2D;

class Networking {
public:
    Networking();
    virtual ~Networking();

    bool init(const char* host, Uint16 port, const char* playerName);
    void disconnect();
    void update();

    Uint8 get_client_id() const { return _client_Id; }
    bool is_master() const { return _client_Id == _master_Id; }

    // Envío de mensajes
    void send_state(const Vector2D& pos, float theta, float velX, float velY);
    void send_info(Uint8 health, Uint8 state, Sint32 score);
    void send_shoot(Vector2D origin, Vector2D direction);
    void send_dead(Uint8 id, Uint8 shooter, Uint32 timestamp);
    void send_health(Uint8 health);
    void send_score(Sint32 score);
    void send_restart();
    void send_switch_player();
    void send_sound_event(Uint8 soundType, float sourceX, float sourceY);

    void set_game_callback(LittleWolf* game);

private:
    void handle_message(const SDLNetUtils::buff_t& buf);
    void handle_new_client(Uint8 id, Uint8 masterId);
    void handle_disconnect(Uint8 id, Uint8 masterId);
    void handle_player_state(const PlayerStateMsg& msg);
    void handle_player_info(const PlayerInfoMsg& msg);
    void handle_shoot(const ShootMsg& msg);
    void handle_dead(const DeadMsg& msg);
    void handle_health(const HealthMsg& msg);
    void handle_score(const ScoreMsg& msg);
    void handle_restart_countdown(const RestartCountdownMsg& msg);
    void handle_restart();
    void handle_master_changed(Uint8 newMaster);
    void handle_current_player(Uint8 playerId);
    void handle_sound_event(const SoundEventMsg& msg);

    NET_StreamSocket* _sock;
    Uint8 _client_Id;
    Uint8 _master_Id;
    std::string _playerName;
    LittleWolf* _game;

    // Control de spam
    Uint32 _lastStateSend;
    float _lastX, _lastY, _lastTheta;
};