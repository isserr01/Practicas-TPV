// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <SDL3_net/SDL_net.h>
#include <cstring>
#include <memory>
#include "game/TCPServer.h"
#include "game/LittleWolf.h"
#include "game/Networking.h"
#include "sdlutils/SDLUtils.h"
#include "sdlutils/InputHandler.h"

using namespace std;

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

void error() {
    cout << "Error: " << SDL_GetError() << endl;
    exit(1);
}

// ==================== SERVIDOR LITTLEWOLF ====================
// Esta clase extiende TCPServer para manejar la lógica del juego
class LittleWolfServer : public TCPServer {
private:
    // Datos de cada jugador
    struct PlayerGameData {
        float x, y, theta;
        Uint8 state;      // 0=NOT_USED, 1=ALIVE, 2=DEAD
        Uint8 health;
        Sint32 score;
        string name;
    };

    PlayerGameData _players[10];  // Máximo 10 jugadores
    Uint8 _masterId;
    bool _waitingRestart;
    int _restartTimer;
    Uint32 _restartStartTime;

public:
    LittleWolfServer(Uint8 maxClients) : TCPServer(maxClients), _masterId(0),
        _waitingRestart(false), _restartTimer(0) {
        // Inicializar datos de jugadores
        for (int i = 0; i < 10; i++) {
            _players[i].state = 0;  // NOT_USED
            _players[i].health = 100;
            _players[i].score = 0;
            _players[i].x = 3.5f + (rand() % 10);
            _players[i].y = 3.5f + (rand() % 10);
            _players[i].theta = 0;
        }
    }

    // Este método se llama cuando se recibe un mensaje
    // Necesitas modificar TCPServer para que llame a este método
    // O lo hacemos directamente en el main procesando los mensajes
};

// Por ahora, usaremos el TCPServer directamente y manejaremos los mensajes en el main

// ==================== MODO SERVIDOR ====================
void server(Uint16 port) {
    cout << "=== LITTLEWOLF SERVER ===" << endl;
    cout << "Starting server at port " << port << endl;

    TCPServer server(4);  // Máximo 4 jugadores

    if (!server.connect(port)) {
        cerr << "Failed to start server" << endl;
        error();
    }

    cout << "Server is running. Press Ctrl+C to stop." << endl;

    // El servidor ya tiene su propio bucle en server.listen()
    // Pero necesitamos procesar los mensajes específicos de LittleWolf
    // Como TCPServer ya hace broadcast de todo, ¡no necesitamos hacer nada más!
    // Los clientes recibirán todos los mensajes automáticamente

    server.listen();  // Esto se bloquea hasta que se cierra el servidor

    cout << "Server stopped" << endl;
}

// ==================== MODO CLIENTE ====================
class GameClient {
private:
    unique_ptr<LittleWolf> _wolf;
    unique_ptr<Networking> _net;
    bool _running;
    Uint32 _lastStateSend;
    float _lastSentX, _lastSentY, _lastSentTheta;
    string _playerName;

public:
    GameClient() : _running(true), _lastStateSend(0),
        _lastSentX(3.5f), _lastSentY(3.5f), _lastSentTheta(0) {
    }

    bool init(const char* serverIP, Uint16 port, const char* playerName) {
        _playerName = playerName;

        cout << "=== LITTLEWOLF CLIENT ===" << endl;
        cout << "Connecting to " << serverIP << ":" << port << endl;
        cout << "Player name: " << playerName << endl;

        // Inicializar SDL
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
            cerr << "SDL_Init failed: " << SDL_GetError() << endl;
            return false;
        }

        // Inicializar juego
        _wolf = make_unique<LittleWolf>();

        try {
            _wolf->load("../../../resources/maps/little_wolf/map_0.json");
            cout << "Map loaded successfully" << endl;
        }
        catch (const char* e) {
            cerr << "Failed to load map: " << e << endl;
            return false;
        }

        // Crear ventana
        SDL_Window* window = SDL_CreateWindow("LittleWolf Network - Client", 800, 600, 0);
        if (!window) {
            cerr << "Failed to create window: " << SDL_GetError() << endl;
            return false;
        }
        SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
        _wolf->init(window, renderer);
        cout << "Window created" << endl;

        // Inicializar red
        _net = make_unique<Networking>();
        if (!_net->init(serverIP, port, playerName)) {
            cerr << "Failed to connect to server" << endl;
            return false;
        }

        _net->set_game_callback(_wolf.get());
        _wolf->setMyPlayerId(_net->get_client_id());

        cout << "Connected with ID: " << (int)_net->get_client_id() << endl;
        cout << "Controls: W/A/S/D = move, H/L = rotate, SPACE = shoot, ESC = exit" << endl;

        return true;
    }

    void run() {
        while (_running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) {
                    _running = false;
                }
                else if (event.type == SDL_EVENT_KEY_DOWN) {
                    if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                        _running = false;
                    }
                    else if (event.key.scancode == SDL_SCANCODE_N) {
                        _net->send_switch_player();
                    }
                }
            }

            // Actualizar red (recibir mensajes)
            _net->update();

            // Actualizar juego (detecta inputs)
            _wolf->update();

            // Enviar estado al servidor (solo si estamos vivos)
            sendPlayerState();

            // Enviar disparo si es necesario
            checkAndSendShoot();

            // Renderizar
            _wolf->render();

            SDL_Delay(16);
        }
    }

    void sendPlayerState() {
        if (_wolf->getLocalState() != LittleWolf::ALIVE) return;

        Uint32 now = SDL_GetTicks();
        if (now - _lastStateSend < 50) return;

        float moveX = _wolf->getPendingMoveX();
        float moveY = _wolf->getPendingMoveY();
        float rotDelta = _wolf->getPendingRotDelta();

        float speed = 0.05f;
        float rotSpeed = 0.05f;

        float newTheta = _lastSentTheta + rotDelta * rotSpeed;
        float newX = _lastSentX + (moveY * cosf(_lastSentTheta) - moveX * sinf(_lastSentTheta)) * speed;
        float newY = _lastSentY + (moveY * sinf(_lastSentTheta) + moveX * cosf(_lastSentTheta)) * speed;

        if (fabs(newX - _lastSentX) > 0.01f || fabs(newY - _lastSentY) > 0.01f || fabs(newTheta - _lastSentTheta) > 0.01f) {
            _net->send_state(Vector2D(newX, newY), newTheta, 0, 0);
            _lastSentX = newX;
            _lastSentY = newY;
            _lastSentTheta = newTheta;
            _lastStateSend = now;
            _wolf->updatePlayerPosition(_net->get_client_id(), newX, newY, newTheta);
        }
    }

    void checkAndSendShoot() {
        if (_wolf->isShootRequested()) {
            LittleWolf::Point origin = _wolf->getLocalPosition();
            float theta = _wolf->getLocalTheta();
            _net->send_shoot(Vector2D(origin.x, origin.y), Vector2D(cosf(theta), sinf(theta)));
            _wolf->clearShootRequest();
        }
    }

    void shutdown() {
        if (_net) _net->disconnect();
    }
};

void client(const char* host, Uint16 port) {
    GameClient gameClient;
    if (!gameClient.init(host, port, "Player")) {
        error();
    }

    gameClient.run();
    gameClient.shutdown();
}

// ==================== MAIN ====================
int main(int argc, char** argv) {
    // Inicializar SDLNet
    if (NET_Init() < 0) {
        error();
    }

    if (argc == 3 && strcmp(argv[1], "server") == 0) {
        server(static_cast<Uint16>(atoi(argv[2])));
    }
    else if (argc == 4 && strcmp(argv[1], "client") == 0) {
        client(argv[2], static_cast<Uint16>(atoi(argv[3])));
    }
    else {
        cout << "Usage: " << endl;
        cout << "  " << argv[0] << " client host port " << endl;
        cout << "  " << argv[0] << " server port " << endl;
        cout << endl;
        cout << "Example:" << endl;
        cout << "  " << argv[0] << " server 2000" << endl;
        cout << "  " << argv[0] << " client localhost 2000" << endl;
    }

    // Finalizar SDLNet
    NET_Quit();

    return 0;
}