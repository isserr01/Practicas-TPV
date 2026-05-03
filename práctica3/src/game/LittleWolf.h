// This file is part of the course TPV2@UCM - Samir Genaim

/*
 * The content of this class is based on https://glouw.com/2018/03/11/littlewolf.html
 * Modified for network multiplayer
 */

#pragma once

#include <SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "../sdlutils/InputHandler.h"

class LittleWolf {
public:

    // a point in a 2D-plane
    struct Point {
        float x;
        float y;

        Point() : x(0), y(0) {}
        Point(float _x, float _y) : x(_x), y(_y) {}
    };

    // Used to represent a position where a ray hits, and which tile
    struct Hit {
        int tile;
        Point where;
    };

    // a line between two points
    struct Line {
        Point a;
        Point b;

        Line() : a(), b() {}
        Line(Point _a, Point _b) : a(_a), b(_b) {}
    };

    // The information on the window/renderer, the size of the window, and a texture
    struct Gpu {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        int xres;
        int yres;
    };

    // when we lock a texture, we get an array of pixels where we can draw
    struct Display {
        Uint32* pixels;
        int width;
        int pitch;
    };

    // representing a (project) wall
    struct Wall {
        int top;
        int bot;
        float size;
    };

    // the status of a player
    enum PlayerState : Uint8 {
        NOT_USED = 0,
        ALIVE = 1,
        DEAD = 2
    };

    // player information
    struct Player {
        Uint8 id;
        Line fov;
        Point where;
        Point velocity;
        float speed;
        float acceleration;
        float theta;
        PlayerState state;
    };

    // Representing a map
    struct Map {
        Uint8** user_walling;
        Uint8** walling;
        Uint16 user_walling_width;
        Uint16 user_walling_height;
        Uint16 walling_width;
        Uint16 walling_height;

        Map() : user_walling(nullptr), walling(nullptr),
            user_walling_width(0), user_walling_height(0),
            walling_width(0), walling_height(0) {
        }

        ~Map() {
            if (user_walling != nullptr) {
                for (auto i = 0u; i < user_walling_height; i++) {
                    delete[] user_walling[i];
                }
                delete[] user_walling;
            }
            if (walling != nullptr) {
                for (auto i = 0u; i < walling_height; i++) {
                    delete[] walling[i];
                }
                delete[] walling;
            }
        }
    };

    LittleWolf();
    virtual ~LittleWolf();

    // load a map from a file
    void load(std::string filename);

    // add a new player with identifier <id>
    bool addPlayer(Uint8 id);

    // initialize the SDL window information
    void init(SDL_Window* window, SDL_Renderer* render);

    // render the walls, etc
    void render();

    // update - solo detecta inputs para enviar al servidor
    void update();

    // Getters
    int get_xres() const { return _xres; }
    int get_yres() const { return _yres; }

    void bringAllToLife();

    // ========== MÉTODOS PARA NETWORKING ==========

    // Actualizar estado de un jugador remoto
    void updatePlayerRemote(Uint8 playerId, float x, float y, float theta,
        Uint8 state, Uint8 health, Sint32 score, const char* name);

    // Actualizar solo posición (para movimiento suave)
    void updatePlayerPosition(Uint8 playerId, float x, float y, float theta);

    // Actualizar salud
    void updatePlayerHealth(Uint8 playerId, Uint8 health);

    // Actualizar puntuación
    void updatePlayerScore(Uint8 playerId, Sint32 score);

    // Marcar jugador como muerto
    void onPlayerDied(Uint8 victimId, Uint8 killerId);

    // Reiniciar juego (revivir todos)
    void restartGame();

    // Mostrar mensaje de cuenta regresiva
    void showRestartCountdown(int seconds);

    // Establecer mi ID de jugador
    void setMyPlayerId(Uint8 id) { _myPlayerId = id; }
    Uint8 getMyPlayerId() const { return _myPlayerId; }

    // Establecer si soy el master
    void setMaster(bool isMaster) { _isMaster = isMaster; }
    bool isMaster() const { return _isMaster; }

    // Obtener posición del jugador local (para envío)
    Point getLocalPosition() const;
    float getLocalTheta() const;

    // Obtener inputs pendientes para enviar al servidor
    float getPendingMoveX() const { return _pendingMoveX; }
    float getPendingMoveY() const { return _pendingMoveY; }
    float getPendingRotDelta() const { return _pendingRotDelta; }
    bool isShootRequested() const { return _shootRequested; }
    void clearShootRequest() { _shootRequested = false; }

    // Estado del jugador local
    Uint8 getLocalHealth() const { return _myHealth; }
    Sint32 getLocalScore() const { return _myScore; }
    Uint8 getLocalState() const { return _myState; }

    // Obtener información de todos los jugadores para el servidor
    void getAllPlayersInfo(std::vector<Uint8>& ids, std::vector<Point>& positions,
        std::vector<float>& thetas, std::vector<Uint8>& states,
        std::vector<Uint8>& healths, std::vector<Sint32>& scores) const;

    // Verificar colisión para el master
    bool checkCollision(const Point& origin, float dirX, float dirY, Uint8 shooterId, Uint8& hitPlayerId) const;

private:
    // ========== MÉTODOS DE RENDERIZADO ==========
    void render_map(const Player& p);
    void render_upper_view();
    void render_players_info();

    // ========== MÉTODOS DE CASTING ==========
    Wall project(int xres, int yres, float focal, const Point& corrected);
    Hit cast(const Point& where, Point direction, Uint8** walling, bool ignore_players, bool ignore_deads);

    // ========== MÉTODOS AUXILIARES ==========
    void applyRemoteInterpolation();
    void switchToNextPlayer();
    void muteSound();

    // ========== FUNCIONES MATEMÁTICAS ==========
    inline Point turn(const Point& a, float t) const {
        return { a.x * cosf(t) - a.y * sinf(t), a.x * sinf(t) + a.y * cosf(t) };
    }

    inline Point rag(const Point& a) const { return { -a.y, a.x }; }
    inline Point sub(const Point& a, const Point& b) const { return { a.x - b.x, a.y - b.y }; }
    inline Point add(const Point& a, const Point& b) const { return { a.x + b.x, a.y + b.y }; }
    inline Point mul(const Point& a, float n) const { return { a.x * n, a.y * n }; }
    inline float mag(const Point& a) const { return sqrtf(a.x * a.x + a.y * a.y); }
    inline Point unit(const Point& a) const { return mul(a, 1.0f / mag(a)); }
    inline float slope(const Point& a) const { return a.y / a.x; }

    inline Line rotate(const Line& l, float t) const {
        return { turn(l.a, t), turn(l.b, t) };
    }

    inline Point lerp(const Line& l, float n) const {
        return add(l.a, mul(sub(l.b, l.a), n));
    }

    inline int fl(float x) const { return (int)x - (x < (int)x); }
    inline int cl(float x) const { return (int)x + (x > (int)x); }
    inline float dec(float x) const { return x - (int)x; }

    inline Point sh(const Point& a, const Point& b) const {
        float x = b.x > 0.0f ? fl(a.x + 1.0f) : cl(a.x - 1.0f);
        float y = slope(b) * (x - a.x) + a.y;
        return { x, y };
    }

    inline Point sv(const Point& a, const Point& b) const {
        float y = b.y > 0.0f ? fl(a.y + 1.0f) : cl(a.y - 1.0f);
        float x = (y - a.y) / slope(b) + a.x;
        return { x, y };
    }

    inline Uint8 tile(const Point& a, Uint8** tiles) const {
        return tiles[(int)a.y][(int)a.x];
    }

    inline Line viewport(float focal) const {
        return { { focal, -1.0f }, { focal, 1.0f } };
    }

    inline Display lock(const Gpu& gpu) {
        void* screen;
        int pitch;
        SDL_LockTexture(gpu.texture, NULL, &screen, &pitch);
        return { (Uint32*)screen, pitch / (int)sizeof(Uint32), pitch };
    }

    inline void put(const Display& display, int x, int y, Uint32 pixel) {
        display.pixels[y + x * display.width] = pixel;
    }

    inline void unlock(const Gpu& gpu) {
        SDL_UnlockTexture(gpu.texture);
    }

    inline Uint32 color_rgba(Uint8 tile) const {
        Uint32 c = color(tile);
        return (c << 8) | (c >> 24 & 0xff);
    }

    Uint32 color(Uint8 tile) const;

    inline Uint8 player_to_tile(Uint8 id) const { return id + 10; }
    inline Uint8 tile_to_player(Uint8 tile) const { return tile - 10; }

    // ========== MIEMBROS ==========

    bool _show_help;
    static constexpr Uint8 _max_player = 10;

    int _xres;
    int _yres;
    static constexpr Uint8 _walling_size_factor = 2;
    Uint16 _walling_width;
    Uint16 _walling_height;
    float _shoot_distance;

    Map _map;
    Player _players[_max_player];
    Uint8 _curr_player_id;
    Gpu _gpu;
    bool _mute;

    // ========== NETWORKING ==========
    Uint8 _myPlayerId;
    Uint8 _myHealth;
    Sint32 _myScore;
    Uint8 _myState;
    bool _isMaster;

    // Almacenamiento para jugadores remotos
    struct RemotePlayerInfo {
        float x, y, theta;
        Uint8 state;
        Uint8 health;
        Sint32 score;
        std::string name;
        bool active;
        float targetX, targetY, targetTheta;

        RemotePlayerInfo() : x(0), y(0), theta(0), state(NOT_USED),
            health(100), score(0), active(false),
            targetX(0), targetY(0), targetTheta(0) {
        }
    };

    RemotePlayerInfo _remotePlayers[_max_player];

    // Inputs pendientes para enviar al servidor
    float _pendingMoveX, _pendingMoveY, _pendingRotDelta;
    bool _shootRequested;

    // Control de interpolación
    Uint32 _lastInterpolationTime;

    // Mensaje de reinicio
    int _restartCountdown;
    bool _showRestartMessage;
};