// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>
#include <cstdint>
#include <cstring>

// Tipos de mensajes para LittleWolf
enum MessageType : Uint8 {
	_CONN_REQUEST_ACCEPTED = 0,
	_CONN_REQUEST_REJECTED = 1,
	_CLIENT_CONNECTED = 1,
	_CLIENT_DISCONNECTED = 2,
	_SERVER_SHUTDOWN = 4,

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

#pragma pack(push, 1)

struct Msg {
	Uint8 type;

	inline Uint8* serialize(Uint8* buf) const {
		*buf = type;
		return buf + sizeof(Uint8);
	}

	inline Uint8* deserialize(Uint8* buf) {
		type = *buf;
		return buf + sizeof(Uint8);
	}
};

struct MsgWithMasterId : Msg {
	Uint8 clientId;
	Uint8 masterId;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = clientId; buf++;
		*buf = masterId; buf++;
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		clientId = *buf; buf++;
		masterId = *buf; buf++;
		return buf;
	}
};

struct PlayerStateMsg : Msg {
	Uint8 clientId;
	float x, y;
	float theta;
	float velX, velY;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = clientId; buf++;
		memcpy(buf, &x, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &y, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &theta, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &velX, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &velY, sizeof(float)); buf += sizeof(float);
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		clientId = *buf; buf++;
		memcpy(&x, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&y, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&theta, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&velX, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&velY, buf, sizeof(float)); buf += sizeof(float);
		return buf;
	}
};

struct PlayerInfoMsg : Msg {
	Uint8 clientId;
	float x, y;
	float theta;
	Uint8 state;
	Uint8 health;
	Sint32 score;
	char name[11];

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = clientId; buf++;
		memcpy(buf, &x, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &y, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &theta, sizeof(float)); buf += sizeof(float);
		*buf = state; buf++;
		*buf = health; buf++;
		Sint32 netScore = SDL_Swap32BE(score);
		memcpy(buf, &netScore, sizeof(Sint32)); buf += sizeof(Sint32);
		memcpy(buf, name, 11); buf += 11;
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		clientId = *buf; buf++;
		memcpy(&x, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&y, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&theta, buf, sizeof(float)); buf += sizeof(float);
		state = *buf; buf++;
		health = *buf; buf++;
		Sint32 netScore;
		memcpy(&netScore, buf, sizeof(Sint32)); buf += sizeof(Sint32);
		score = SDL_Swap32BE(netScore);
		memcpy(name, buf, 11); buf += 11;
		return buf;
	}
};

struct ShootMsg : Msg {
	Uint8 shooterId;
	float originX, originY;
	float dirX, dirY;
	Uint32 timestamp;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = shooterId; buf++;
		memcpy(buf, &originX, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &originY, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &dirX, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &dirY, sizeof(float)); buf += sizeof(float);
		Uint32 netTime = SDL_Swap32BE(timestamp);
		memcpy(buf, &netTime, sizeof(Uint32)); buf += sizeof(Uint32);
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		shooterId = *buf; buf++;
		memcpy(&originX, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&originY, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&dirX, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&dirY, buf, sizeof(float)); buf += sizeof(float);
		Uint32 netTime;
		memcpy(&netTime, buf, sizeof(Uint32)); buf += sizeof(Uint32);
		timestamp = SDL_Swap32BE(netTime);
		return buf;
	}
};

struct DeadMsg : Msg {
	Uint8 victimId;
	Uint8 killerId;
	Uint32 timestamp;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = victimId; buf++;
		*buf = killerId; buf++;
		Uint32 netTime = SDL_Swap32BE(timestamp);
		memcpy(buf, &netTime, sizeof(Uint32)); buf += sizeof(Uint32);
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		victimId = *buf; buf++;
		killerId = *buf; buf++;
		Uint32 netTime;
		memcpy(&netTime, buf, sizeof(Uint32)); buf += sizeof(Uint32);
		timestamp = SDL_Swap32BE(netTime);
		return buf;
	}
};

struct HealthMsg : Msg {
	Uint8 clientId;
	Uint8 health;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = clientId; buf++;
		*buf = health; buf++;
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		clientId = *buf; buf++;
		health = *buf; buf++;
		return buf;
	}
};

struct ScoreMsg : Msg {
	Uint8 clientId;
	Sint32 score;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = clientId; buf++;
		Sint32 netScore = SDL_Swap32BE(score);
		memcpy(buf, &netScore, sizeof(Sint32)); buf += sizeof(Sint32);
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		clientId = *buf; buf++;
		Sint32 netScore;
		memcpy(&netScore, buf, sizeof(Sint32)); buf += sizeof(Sint32);
		score = SDL_Swap32BE(netScore);
		return buf;
	}
};

struct RestartCountdownMsg : Msg {
	Sint32 seconds;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		Sint32 netSecs = SDL_Swap32BE(seconds);
		memcpy(buf, &netSecs, sizeof(Sint32)); buf += sizeof(Sint32);
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		Sint32 netSecs;
		memcpy(&netSecs, buf, sizeof(Sint32)); buf += sizeof(Sint32);
		seconds = SDL_Swap32BE(netSecs);
		return buf;
	}
};

struct CurrentPlayerMsg : Msg {
	Uint8 playerId;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = playerId; buf++;
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		playerId = *buf; buf++;
		return buf;
	}
};

struct SoundEventMsg : Msg {
	Uint8 sourceId;
	Uint8 soundType;
	float sourceX, sourceY;

	inline Uint8* serialize(Uint8* buf) const {
		buf = Msg::serialize(buf);
		*buf = sourceId; buf++;
		*buf = soundType; buf++;
		memcpy(buf, &sourceX, sizeof(float)); buf += sizeof(float);
		memcpy(buf, &sourceY, sizeof(float)); buf += sizeof(float);
		return buf;
	}

	inline Uint8* deserialize(Uint8* buf) {
		buf = Msg::deserialize(buf);
		sourceId = *buf; buf++;
		soundType = *buf; buf++;
		memcpy(&sourceX, buf, sizeof(float)); buf += sizeof(float);
		memcpy(&sourceY, buf, sizeof(float)); buf += sizeof(float);
		return buf;
	}
};

#pragma pack(pop)