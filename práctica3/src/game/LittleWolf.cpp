// This file is part of the course TPV2@UCM - Samir Genaim

#include "LittleWolf.h"
#include "../json/JSONValue.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "Game.h"
#include <iostream>
#include <cstring>

// Para evitar warnings de strncpy en Windows
#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

LittleWolf::LittleWolf() :
    _show_help(true),
    _xres(0), _yres(0),
    _walling_width(0), _walling_height(0),
    _shoot_distance(0),
    _map(),
    _players(),
    _curr_player_id(0),
    _mute(false),
    _myPlayerId(0),
    _myHealth(100),
    _myScore(0),
    _myState(ALIVE),
    _isMaster(false),
    _pendingMoveX(0), _pendingMoveY(0), _pendingRotDelta(0),
    _shootRequested(false),
    _lastInterpolationTime(0),
    _restartCountdown(0),
    _showRestartMessage(false) {

    // Inicializar jugadores
    for (int i = 0; i < _max_player; i++) {
        _players[i].state = NOT_USED;
        _players[i].id = i;
    }
}

LittleWolf::~LittleWolf() {
}

void LittleWolf::load(std::string filename) {
    // Load JSON walling file.
    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile(filename));

    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw "Something went wrong while load/parsing the walling file'" + filename + "'";
    }

    JSONObject root = jValueRoot->AsObject();
    JSONValue* jValue = nullptr;

    Uint16 uh = 0, uw = 0, sf = 0;

    jValue = root["scaling_factor"];
    if (jValue != nullptr) {
        if (jValue->IsNumber()) {
            sf = static_cast<Uint16>(jValue->AsNumber());
        }
        else {
            throw "Incorrect value for scaling_factor";
        }
    }
    else {
        sf = 2;
    }

    if (sf % _walling_size_factor != 0)
        throw "Scaling factor should be divisible by '" + std::to_string(_walling_size_factor) + "': " + std::to_string(sf);

    jValue = root["walling"];
    JSONArray json_walling;

    if (jValue != nullptr) {
        if (jValue->IsArray()) {
            json_walling = jValue->AsArray();
            uh = json_walling.size();
            assert(uh > 0);
            uw = json_walling[0]->AsString().size();
        }
    }

    _xres = uw * sf;
    _yres = uh * sf;
    _walling_width = _xres / _walling_size_factor;
    _walling_height = _yres / _walling_size_factor;
    _shoot_distance = std::min(_walling_width, _walling_height) / 2;

    // Load user walling
    Uint8** walling = new Uint8 * [uh];
    for (auto i = 0u; i < uh; i++) {
        std::string row = json_walling[i]->AsString();
        const char* buffer = json_walling[i]->AsString().c_str();

        walling[i] = new Uint8[uw];
        for (auto j = 0u; j < uw; j++) {
            char tile = buffer[j];
            if (tile < 48 || tile > 57) {
                throw "Invalid value for tile (" + std::to_string(i) + "," + std::to_string(j) + ") :'" + tile + "'";
            }
            walling[i][j] = tile - '0';
        }
    }

    _map.user_walling = walling;
    _map.user_walling_width = uw;
    _map.user_walling_height = uh;

    // Check borders
    auto n = uh - 1;
    for (auto i = 0u; i < uw; i++) {
        if (walling[0][i] == 0 || walling[n][i] == 0) {
            throw "There is a zero valued-cell at border.";
        }
    }

    n = uw - 1;
    for (auto i = 0u; i < uh; i++) {
        if (walling[i][0] == 0 || walling[i][n] == 0) {
            throw "There is a zero valued-cell at border.";
        }
    }

    // Construct scaled walling
    _map.walling = new Uint8 * [_walling_height];
    _map.walling_height = _walling_height;
    _map.walling_width = _walling_width;
    for (auto i = 0u; i < _walling_height; i++) {
        _map.walling[i] = new Uint8[_walling_width];
        for (auto j = 0u; j < _walling_width; j++)
            _map.walling[i][j] = 1;
    }

    Uint16 cell_height = _walling_height / _map.user_walling_height;
    Uint16 cell_width = _walling_width / _map.user_walling_width;

    for (int i = 0; i < _map.user_walling_height; i++)
        for (int j = 0; j < _map.user_walling_width; j++)
            for (int k = 0; k < cell_height; k++)
                for (int l = 0; l < cell_width; l++) {
                    _map.walling[i * cell_height + k][j * cell_width + l] = _map.user_walling[i][j];
                }
}

void LittleWolf::init(SDL_Window* window, SDL_Renderer* render) {
    SDL_Texture* const texture = SDL_CreateTexture(sdlutils().renderer(),
        SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, _yres, _xres);
    _gpu = { window, render, texture, _xres, _yres };
}

bool LittleWolf::addPlayer(Uint8 id) {
    assert(id < _max_player);

    if (_players[id].state != NOT_USED)
        return false;

    auto& rand = sdlutils().rand();

    Uint16 orow = rand.nextInt(0, _map.walling_height);
    Uint16 ocol = rand.nextInt(0, _map.walling_width);

    Uint16 row = orow;
    Uint16 col = (ocol + 1) % _map.walling_width;
    while (!((orow == row) && (ocol == col)) && _map.walling[row][col] != 0) {
        col = (col + 1) % _map.walling_width;
        if (col == 0)
            row = (row + 1) % _map.walling_height;
    }

    if (row >= _map.walling_height)
        return false;

    Player p = {
        id,
        viewport(0.8f),
        { col + 0.5f, row + 0.5f },
        { 0.0f, 0.0f },
        2.0f,
        0.9f,
        0.0f,
        ALIVE
    };

    _map.walling[(int)p.where.y][(int)p.where.x] = player_to_tile(id);
    _players[id] = p;

    if (id == _myPlayerId) {
        _curr_player_id = id;
    }

    return true;
}

void LittleWolf::update() {
    auto& ihdlr = ih();

    // Procesar teclas de UI (no juego)
    if (ihdlr.keyDownEvent()) {
        if (ihdlr.isKeyDown(SDL_SCANCODE_T)) {
            _show_help = !_show_help;
        }
        else if (ihdlr.isKeyDown(SDL_SCANCODE_M)) {
            muteSound();
        }
    }

    // Si el jugador local está muerto, no puede moverse ni disparar
    if (_myState != ALIVE) {
        _pendingMoveX = _pendingMoveY = _pendingRotDelta = 0;
        _shootRequested = false;

        // Vista aérea automática (ya manejada en render())
        return;
    }

    // Detectar inputs para enviar al servidor
    _pendingMoveX = 0;
    _pendingMoveY = 0;
    _pendingRotDelta = 0;

    if (ihdlr.isKeyDown(SDL_SCANCODE_W)) _pendingMoveY += 1;
    if (ihdlr.isKeyDown(SDL_SCANCODE_S)) _pendingMoveY -= 1;
    if (ihdlr.isKeyDown(SDL_SCANCODE_D)) _pendingMoveX += 1;
    if (ihdlr.isKeyDown(SDL_SCANCODE_A)) _pendingMoveX -= 1;
    if (ihdlr.isKeyDown(SDL_SCANCODE_H)) _pendingRotDelta = -1;
    if (ihdlr.isKeyDown(SDL_SCANCODE_L)) _pendingRotDelta = 1;

    // Movimiento más lento con Shift
    if (ihdlr.isKeyDown(SDL_SCANCODE_LSHIFT)) {
        _pendingRotDelta *= 0.1f;
        _pendingMoveX *= 0.1f;
        _pendingMoveY *= 0.1f;
    }

    // Detectar disparo
    _shootRequested = (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_SPACE));

    // Aplicar interpolación para jugadores remotos
    applyRemoteInterpolation();

    // Actualizar el jugador actual (la cámara) - usar mi posición local
    if (_players[_myPlayerId].state == ALIVE) {
        _curr_player_id = _myPlayerId;
    }
    else {
        // Si estoy muerto, buscar cualquier jugador vivo para la cámara en vista aérea
        // Pero la vista aérea se maneja en render()
    }

}

void LittleWolf::applyRemoteInterpolation() {
    Uint32 now = SDL_GetTicks();
    float factor = 0.3f;  // Factor de interpolación

    for (int i = 0; i < _max_player; i++) {
        if (i == _myPlayerId) continue;

        RemotePlayerInfo& remote = _remotePlayers[i];
        if (remote.active && _players[i].state != NOT_USED) {
            // Interpolación suave hacia la posición objetivo
            _players[i].where.x = _players[i].where.x * (1 - factor) + remote.targetX * factor;
            _players[i].where.y = _players[i].where.y * (1 - factor) + remote.targetY * factor;
            _players[i].theta = _players[i].theta * (1 - factor) + remote.targetTheta * factor;
        }
    }

    _lastInterpolationTime = now;
}

void LittleWolf::render() {
    // Si el jugador local está muerto, mostrar vista aérea
    bool isLocalDead = (_myState == DEAD);

    if (isLocalDead) {
        render_upper_view();
    }
    else {
        // Buscar el jugador local en _players
        for (int i = 0; i < _max_player; i++) {
            if (_players[i].id == _myPlayerId && _players[i].state == ALIVE) {
                render_map(_players[i]);
                break;
            }
        }
    }

    render_players_info();

    // Mostrar ayuda
    if (_show_help) {
        int y = sdlutils().height();
        std::vector<const char*> helpLines = {
            "usage_1",
            "usage_2",
            "usage_3",
            "usage_4",
            "usage_5",
            "usage_6",
            "usage_7",
            "usage_8",
        };

        for (const char* line : helpLines) {
            auto& t = sdlutils().msgs().at(line);
            y = y - t.height() - 5;
            t.render(5, y);
        }
    }

    // Mostrar mensaje de reinicio si aplica
    if (_showRestartMessage && _restartCountdown > 0) {
        std::string msg = "Game restarts in " + std::to_string(_restartCountdown) + " seconds";
        auto& text = sdlutils().msgs().at(msg.c_str());
        int x = (sdlutils().width() - text.width()) / 2;
        int y = sdlutils().height() / 2;
        text.render(x, y);
    }
}

void LittleWolf::render_map(const Player& p) {
    const Display display = lock(_gpu);
    const Line camera = rotate(p.fov, p.theta);

    for (int x = 0; x < _gpu.xres; x++) {
        const Point direction = lerp(camera, x / (float)_gpu.xres);
        const Hit hit = cast(p.where, direction, _map.walling, true, false);
        const Point ray = sub(hit.where, p.where);
        const Point corrected = turn(ray, -p.theta);
        const Wall wall = project(_gpu.xres, _gpu.yres, p.fov.a.x, corrected);

        for (int y = 0; y < wall.bot; y++)
            put(display, x, y, 0x00000000);
        for (int y = wall.bot; y < wall.top; y++)
            put(display, x, y, color(hit.tile));
        for (int y = wall.top; y < _gpu.yres; y++)
            put(display, x, y, 0x00000000);

        // Dibujar jugadores
        const Point direction_p = lerp(camera, x / (float)_gpu.xres);
        const Hit hit_p = cast(p.where, direction_p, _map.walling, false, false);
        const Point ray_p = sub(hit_p.where, p.where);
        const Point corrected1 = turn(ray_p, -p.theta);
        const Wall wall_p = project(_gpu.xres, _gpu.yres, p.fov.a.x, corrected1);

        if (hit_p.tile >= 10) {
            int size_p = 0;
            Uint8 id = hit_p.tile - 10;

            if (_players[id].state == ALIVE)
                size_p = wall_p.top - (wall_p.top - wall_p.bot) / 3;
            else
                size_p = wall_p.bot + (wall_p.top - wall_p.bot) / 10;

            for (int y = wall_p.bot; y < size_p; y++)
                put(display, x, y, color(hit_p.tile));
        }
    }

    // Mira
    for (int i = -10; i < 10; i++) {
        put(display, _gpu.xres / 2, _gpu.yres / 2 + i, 0xAAAAAAAA);
        put(display, _gpu.xres / 2 + i, _gpu.yres / 2, 0xAAAAAAAA);
    }

    unlock(_gpu);

    const SDL_FRect dst = {
        (_gpu.xres - _gpu.yres) / 2.0f,
        (_gpu.yres - _gpu.xres) / 2.0f,
        (float)_gpu.yres,
        (float)_gpu.xres
    };
    SDL_RenderTextureRotated(_gpu.renderer, _gpu.texture, NULL, &dst, -90, NULL, SDL_FLIP_NONE);
}

void LittleWolf::render_upper_view() {
    const Display display = lock(_gpu);
    std::memset(display.pixels, 0, display.pitch * _xres);

    // Dibujar mapa
    for (auto x = 0u; x < _map.walling_height; x++) {
        for (auto y = 0u; y < _map.walling_width; y++) {
            if (_map.walling[x][y] != 0) {
                for (int i = 0; i < _walling_size_factor; i++) {
                    for (int j = 0; j < _walling_size_factor; j++) {
                        put(display, y * _walling_size_factor + i,
                            _gpu.yres - 1 - x * _walling_size_factor + j,
                            color(_map.walling[x][y]));
                    }
                }
            }
        }
    }

    unlock(_gpu);

    const SDL_FRect dst = {
        (_gpu.xres - _gpu.yres) / 2.0f,
        (_gpu.yres - _gpu.xres) / 2.0f,
        (float)_gpu.yres,
        (float)_gpu.xres
    };
    SDL_RenderTextureRotated(_gpu.renderer, _gpu.texture, NULL, &dst, -90, NULL, SDL_FLIP_NONE);

    // Dibujar nombres de jugadores
    for (int i = 0; i < _max_player; i++) {
        if (_players[i].state != NOT_USED) {
            std::string name = _remotePlayers[i].name.empty() ? "P" + std::to_string(i) : _remotePlayers[i].name;
            Texture info(sdlutils().renderer(), name,
                sdlutils().fonts().at("MFR12"),
                build_sdlcolor(color_rgba(i + 10)));

            int w = info.width();
            int h = info.height();
            SDL_FRect dest = {
                _players[i].where.x * 2 - w / 2,
                _players[i].where.y * 2 - h / 2,
                (float)w,
                (float)h
            };
            info.render(dest);
        }
    }
}

void LittleWolf::render_players_info() {
    uint_fast16_t y = 5;

    for (int i = 0; i < _max_player; i++) {
        if (_players[i].state != NOT_USED) {
            std::string name = _remotePlayers[i].name.empty() ? "P" + std::to_string(i) : _remotePlayers[i].name;
            std::string indicator = (i == _myPlayerId) ? "→ " : "  ";
            std::string stateStr = "";
            if (_players[i].state == DEAD) stateStr = " [DEAD]";

            std::string msg = indicator + name + stateStr +
                " HP:" + std::to_string(_remotePlayers[i].health) +
                " SCR:" + std::to_string(_remotePlayers[i].score);

            Texture info(sdlutils().renderer(), msg,
                sdlutils().fonts().at("MFR48"),
                build_sdlcolor(color_rgba(i + 10)));

            SDL_FRect dest = { 10.0f, (float)y, (float)info.width(), (float)info.height() };
            info.render(dest);
            y += info.height() + 5;
        }
    }
}

LittleWolf::Wall LittleWolf::project(int xres, int yres, float focal, const Point& corrected) {
    const float normal = corrected.x < 1e-2f ? 1e-2f : 0.05f * corrected.x;
    const float size = 0.5f * focal * xres / normal;
    const int top = (yres + size) / 2.0f;
    const int bot = (yres - size) / 2.0f;
    return { top > yres ? yres : top, bot < 0 ? 0 : bot, size };
}

LittleWolf::Hit LittleWolf::cast(const Point& where, Point direction, Uint8** walling, bool ignore_players, bool ignore_deads) {
    Point hor = sh(where, direction);
    Point ver = sv(where, direction);
    Point ray = mag(sub(hor, where)) < mag(sub(ver, where)) ? hor : ver;
    Point dc = mul(direction, 0.01f);
    Point dx = { dc.x, 0.0f };
    Point dy = { 0.0f, dc.y };
    Point test = add(ray, mag(sub(hor, ver)) < 1e-3f ? dc : dec(ray.x) == 0.0f ? dx : dy);
    Hit hit = { tile(test, walling), ray };

    if (hit.tile > 0 && hit.tile < 10) {
        return hit;
    }
    else if (hit.tile > 9 && !ignore_players && (!ignore_deads || _players[hit.tile - 10].state != DEAD)) {
        return hit;
    }
    else {
        return cast(ray, direction, walling, ignore_players, ignore_deads);
    }
}

Uint32 LittleWolf::color(Uint8 tile) const {
    switch (tile) {
    case 1: return 0xAAAA0000;
    case 2: return 0x0000AA00;
    case 3: return 0x000000AA;
    case 4: return 0x00FFFF00;
    case 5: return 0x0000FFFF;
    case 6: return 0x00FF00FF;
    case 7: return 0x00C0C0C0;
    case 8: return 0x00808080;
    case 9: return 0x00800000;
    case 10: return 0x00808000;
    case 11: return 0x00008000;
    case 12: return 0x00800080;
    case 13: return 0x00008080;
    case 14: return 0x00000080;
    default: return 0x00f260b0;
    }
}

void LittleWolf::muteSound() {
    _mute = !_mute;
    float gain = _mute ? 0.0f : 1.0f;
    SoundManager::Instance()->stop_all(0);
    SoundManager::Instance()->set_master_volume(gain);
}

// ========== MÉTODOS DE NETWORKING ==========

void LittleWolf::bringAllToLife() {
    // bring all dead players to life -- all stay in the same position
    for (auto i = 0u; i < _max_player; i++) {
        if (_players[i].state == DEAD) {
            _players[i].state = ALIVE;
        }
    }
}

void LittleWolf::updatePlayerRemote(Uint8 playerId, float x, float y, float theta,
    Uint8 state, Uint8 health, Sint32 score, const char* name) {
    if (playerId >= _max_player) return;

    // Si es mi ID, actualizar mi estado local
    if (playerId == _myPlayerId) {
        _myHealth = health;
        _myScore = score;
        _myState = state;

        if (_players[playerId].state == NOT_USED && state != NOT_USED) {
            addPlayer(playerId);
        }

        _players[playerId].state = static_cast<PlayerState>(state);
        _players[playerId].where = { x, y };
        _players[playerId].theta = theta;
        return;
    }

    // Actualizar jugador remoto
    RemotePlayerInfo& remote = _remotePlayers[playerId];
    remote.targetX = x;
    remote.targetY = y;
    remote.targetTheta = theta;
    remote.state = state;
    remote.health = health;
    remote.score = score;
    remote.name = std::string(name);
    remote.active = true;

    // Si el jugador no existe en el juego, crearlo
    if (_players[playerId].state == NOT_USED && state != NOT_USED) {
        addPlayer(playerId);
    }

    _players[playerId].state = static_cast<PlayerState>(state);
    // No actualizar posición directamente, usar interpolación
}

void LittleWolf::updatePlayerPosition(Uint8 playerId, float x, float y, float theta) {
    if (playerId >= _max_player || playerId == _myPlayerId) return;
    if (playerId == _myPlayerId) {
        // Actualizar mi posición local
        _players[playerId].where.x = x;
        _players[playerId].where.y = y;
        _players[playerId].theta = theta;
        std::cout << "MY POSITION UPDATED: " << x << ", " << y << std::endl;  // Log
    }
    else {
        // Actualizar posición remota
        _remotePlayers[playerId].targetX = x;
        _remotePlayers[playerId].targetY = y;
        _remotePlayers[playerId].targetTheta = theta;
    }
}

void LittleWolf::updatePlayerHealth(Uint8 playerId, Uint8 health) {
    if (playerId >= _max_player) return;

    if (playerId == _myPlayerId) {
        _myHealth = health;
        if (health == 0) _myState = DEAD;
    }
    else {
        _remotePlayers[playerId].health = health;
    }
}

void LittleWolf::updatePlayerScore(Uint8 playerId, Sint32 score) {
    if (playerId >= _max_player) return;

    if (playerId == _myPlayerId) {
        _myScore = score;
    }
    else {
        _remotePlayers[playerId].score = score;
    }
}

void LittleWolf::onPlayerDied(Uint8 victimId, Uint8 killerId) {
    if (victimId >= _max_player) return;

    if (victimId == _myPlayerId) {
        _myState = DEAD;
        _players[victimId].state = DEAD;
    }
    else {
        _remotePlayers[victimId].state = DEAD;
        _players[victimId].state = DEAD;
    }

    if (killerId == _myPlayerId) {
        _myScore += 10;
    }
}

void LittleWolf::restartGame() {
    _myState = ALIVE;
    _myHealth = 100;
    // El servidor reposicionará a los jugadores

    for (int i = 0; i < _max_player; i++) {
        if (_players[i].state != NOT_USED) {
            _players[i].state = ALIVE;
            _remotePlayers[i].state = ALIVE;
            _remotePlayers[i].health = 100;
        }
    }

    _showRestartMessage = false;
}

void LittleWolf::showRestartCountdown(int seconds) {
    _restartCountdown = seconds;
    _showRestartMessage = (seconds > 0);
}

void LittleWolf::setCurrentPlayer(Uint8 playerId)
{
}

LittleWolf::Point LittleWolf::getLocalPosition() const {
    return _players[_myPlayerId].where;
}

float LittleWolf::getLocalTheta() const {
    return _players[_myPlayerId].theta;
}

void LittleWolf::getAllPlayersInfo(std::vector<Uint8>& ids, std::vector<Point>& positions,
    std::vector<float>& thetas, std::vector<Uint8>& states,
    std::vector<Uint8>& healths, std::vector<Sint32>& scores) const {
    ids.clear();
    positions.clear();
    thetas.clear();
    states.clear();
    healths.clear();
    scores.clear();

    for (int i = 0; i < _max_player; i++) {
        if (_players[i].state != NOT_USED) {
            ids.push_back(i);
            positions.push_back(_players[i].where);
            thetas.push_back(_players[i].theta);
            states.push_back(_players[i].state);
            healths.push_back(_remotePlayers[i].health);
            scores.push_back(_remotePlayers[i].score);
        }
    }
}

bool LittleWolf::checkCollision(const Point& origin, float dirX, float dirY,
    Uint8 shooterId, Uint8& hitPlayerId) const {
    for (int i = 0; i < _max_player; i++) {
        if (i == shooterId) continue;
        if (_players[i].state != ALIVE) continue;

        const Point& target = _players[i].where;
        float dx = target.x - origin.x;
        float dy = target.y - origin.y;

        // Producto escalar para proyección
        float t = dx * dirX + dy * dirY;
        if (t > 0) {
            float hitX = origin.x + dirX * t;
            float hitY = origin.y + dirY * t;
            float distance = sqrtf((hitX - target.x) * (hitX - target.x) +
                (hitY - target.y) * (hitY - target.y));

            if (distance < 0.5f && t < _shoot_distance) {
                hitPlayerId = i;
                return true;
            }
        }
    }
    return false;
}

void LittleWolf::removePlayer(Uint8 id) {
    _players[id].state = LittleWolf::NOT_USED;
}

void LittleWolf::update_player_state(Uint8 id, float x, float y, float w, float h,
    float rot) {

    if (id == _myPlayerId) {
        // Actualizar mi posición (la que el servidor validó)
        _players[id].where.x = x;
        _players[id].where.y = y;
        _players[id].theta = rot;
    }
    else {
        // Actualizar posición de otro jugador
        updatePlayerPosition(id, x, y, rot);
    }

}

void LittleWolf::killPlayer(Uint8 id) {
    _players[id].state = LittleWolf::DEAD;
}

void LittleWolf::update_player_info(Uint8 id, float x, float y, float w, float h,
    float rot, Uint8 state) {
    Player& p = _players[id];

    p.where.x = x;
	p.where.y = y;
    p.id = id;
    p.theta = rot;
    p.state = static_cast<PlayerState>(state);
}

void LittleWolf::send_my_info() {
    Player& p = _players[_myPlayerId];

    Game::Instance()->sendPlayerState();
}

