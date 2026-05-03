// main.cpp - versión que mantiene compatibilidad
#include <iostream>
#include <cstring>
#include "game/Game.h"
#include "game/TCPServer.h"

int main(int argc, char** argv) {

    // Si hay argumento "server", ejecutar servidor
    if (argc > 1 && strcmp(argv[1], "server") == 0) {
        int port = (argc > 2) ? atoi(argv[2]) : 2000;
        std::cout << "LittleWolf Server on port " << port << std::endl;
        TCPServer server(10);
        if (server.connect(port)) {
            server.listen();
        }
        return 0;
    }

    // Si no, ejecutar cliente (como antes)
    try {
        if (Game::Init()) {
            Game::Instance()->init_game("localhost", 2000, "Player");
            Game::Instance()->start();
        }
    }
    catch (const std::string& e) {
        std::cerr << e << std::endl;
    }
    catch (const char* e) {
        std::cerr << e << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
    }
    catch (...) {
        std::cerr << "Caught an exception of unknown type ...";
    }

    return 0;
}