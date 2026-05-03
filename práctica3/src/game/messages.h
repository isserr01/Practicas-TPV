// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include "SDLNetUtils.h"

// Tipos de mensajes base (del servidor)
enum MsgId : Uint8 {
    _CONN_REQUEST = 0,
    _CONN_REQUEST_ACCEPTED,
    _CONN_REQUEST_REJECTED,
    _CLIENT_CONNECTED,
    _CLIENT_DISCONNECTED,
    _SERVER_SHUTDOWN,

    // Mensajes del juego LittleWolf
    _PLAYER_STATE = 10,
    _PLAYER_INFO = 11,
    _PLAYER_SHOOT = 12,
    _PLAYER_DIED = 13,
    _PLAYER_HEALTH = 14,
    _PLAYER_SCORE = 15,
    _RESTART_COUNTDOWN = 16,
    _RESTART = 17,
    _MASTER_CHANGED = 18,
    _CURRENT_PLAYER = 19,
    _SOUND_EVENT = 20
};

// Mensaje base
struct Msg {
    Uint8 type;
    _IMPL_SERIALIZATION_(type)
};

// Mensaje con ID de cliente
struct MsgWithClientId : Msg {
    Uint8 clientId;
    _IMPL_SERIALIZATION_(*static_cast<Msg*>(this), clientId)
};

// Mensaje con ID de master
struct MsgWithMasterId : MsgWithClientId {
    Uint8 masterId;
    _IMPL_SERIALIZATION_(*static_cast<MsgWithClientId*>(this), masterId)
};

// ============ MENSAJES DE LITTLEWOLF ============

// Estado del jugador (posición, rotación)
struct PlayerStateMsg : Msg {
    Uint8 clientId;
    float x, y;
    float theta;
    float velX, velY;

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        clientId, x, y, theta, velX, velY
    )
};

// Información completa del jugador
struct PlayerInfoMsg : Msg {
    Uint8 clientId;
    float x, y;
    float theta;
    Uint8 state;      // 0=NOT_USED, 1=ALIVE, 2=DEAD
    Uint8 health;
    Sint32 score;
    char name[11];

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        clientId, x, y, theta, state, health, score, name
    )
};

// Mensaje de disparo
struct ShootMsg : Msg {
    Uint8 shooterId;
    float originX, originY;
    float dirX, dirY;
    Uint32 timestamp;

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        shooterId, originX, originY, dirX, dirY, timestamp
    )
};

// Mensaje de muerte
struct DeadMsg : Msg {
    Uint8 victimId;
    Uint8 killerId;
    Uint32 timestamp;

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        victimId, killerId, timestamp
    )
};

// Actualización de salud
struct HealthMsg : Msg {
    Uint8 clientId;
    Uint8 health;

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        clientId, health
    )
};

// Actualización de puntuación
struct ScoreMsg : Msg {
    Uint8 clientId;
    Sint32 score;

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        clientId, score
    )
};

// Cuenta regresiva para reinicio
struct RestartCountdownMsg : Msg {
    Sint32 seconds;

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        seconds
    )
};

// Cambio de jugador activo
struct CurrentPlayerMsg : Msg {
    Uint8 playerId;

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        playerId
    )
};

// Evento de sonido
struct SoundEventMsg : Msg {
    Uint8 sourceId;
    Uint8 soundType;  // 0=disparo, 1=muerte
    float sourceX, sourceY;

    _IMPL_SERIALIZATION_(
        *static_cast<Msg*>(this),
        sourceId, soundType, sourceX, sourceY
    )
};