// This file is part of the course TPV2@UCM - Samir Genaim

#include "TCPServer.h"

#include <cassert>
#include <csignal>
#include <thread>
#include <chrono>

#include "messages.h"

Uint16 TCPServer::shutDownTriggetPort = 0;
bool TCPServer::done = true;

TCPServer::TCPServer(Uint8 maxClients) :
	maxClients(maxClients), //
	serv(nullptr), //
	usedConn(0) {
	conn = new void* [maxClients + 1] {};
	clients = new NET_StreamSocket * [maxClients] {};
}

TCPServer::~TCPServer() {
	shutdown();
	delete[] conn;
	delete[] clients;
}

// Versión corregida - sin NET_WaitUntilConnected
void TCPServer::shutdownTrigger() {
	if (done)
		return;

	std::cout << std::endl << "Shutting down ..." << std::endl;
	done = true;

	if (shutDownTriggetPort != 0) {
		// Resolve the localhost to an IP address
		NET_Address* addr = NET_ResolveHostname("localhost");
		if (addr && NET_WaitUntilResolved(addr, 1000) == NET_SUCCESS) {
			// Intentar conectar - puede fallar, no importa
			NET_StreamSocket* conn = NET_CreateClient(addr, shutDownTriggetPort);
			if (conn) {
				// Pequeña espera para que se envíe el mensaje
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				NET_DestroyStreamSocket(conn);
			}
			NET_UnrefAddress(addr);
		}
	}
}

/* Signal Handler for SIGINT */
void TCPServer::handlerSIGINT(int) {
	TCPServer::shutdownTrigger();
}

bool TCPServer::connect(const Uint16 port) {
	TCPServer::shutDownTriggetPort = port;
	signal(SIGINT, TCPServer::handlerSIGINT);

	serv = NET_CreateServer(nullptr, port);
	if (!serv) {
#ifndef NDEBUG
		SDLNetUtils::print_SDLNet_error();
#endif
		return false;
	}

	conn[0] = serv;
	++usedConn;
	done = false;

	std::cout << "The server is up and kicking ..." << std::endl;
	return true;
}

void TCPServer::listen() {
	while (!done) {
		// Usar timeout pequeño para permitir shutdown
		int available = NET_WaitUntilInputAvailable(conn, usedConn, 100);

		if (available > 0) {
			if (done) continue;

			// Aceptar nuevos clientes
			NET_StreamSocket* client = nullptr;
			if (NET_AcceptClient(serv, &client) && client != nullptr) {
				if (usedConn - 1 < maxClients) {
					Uint8 j = 0;
					while (j < maxClients && clients[j] != nullptr)
						j++;

					assert(j < maxClients);

					conn[usedConn] = clients[j] = client;
					std::cout << "New client assigned id " << (int)j << std::endl;
					++usedConn;

					// Send acceptance message
					MsgWithMasterId m;
					m.type = _CONN_REQUEST_ACCEPTED;
					m.clientId = j;
					m.masterId = whoIsTheMaster();
					SDLNetUtils::serialized_send(m, client);

					// Tell all, except the new client, that there is a new client
					m.type = _CLIENT_CONNECTED;
					for (auto i = 1u; i < usedConn - 1; i++) {
						if (client != static_cast<NET_StreamSocket*>(conn[i]))
							SDLNetUtils::serialized_send(m,
								static_cast<NET_StreamSocket*>(conn[i]));
					}
				}
				else {
					// Reject connection request
					Msg m;
					m.type = _CONN_REQUEST_REJECTED;
					SDLNetUtils::serialized_send(m, client);
					NET_DestroyStreamSocket(client);
				}
			}

			// Check activity in the clients sockets
			for (auto i = 1u; i < usedConn; i++) {
				SDLNetUtils::buff_t buf = SDLNetUtils::receive(
					static_cast<NET_StreamSocket*>(conn[i]));

				if (!buf.error) {
					if (buf.size > 0) {
						// Broadcast a todos los clientes
						for (auto j = 1u; j < usedConn; j++) {
							if (j != i) { // No enviar al emisor
								SDLNetUtils::send(
									static_cast<NET_StreamSocket*>(conn[j]),
									buf.data, buf.size);
							}
						}
					}
				}
				else {
					// Client disconnected
					Uint8 j = 0;
					while (j < maxClients && clients[j] != static_cast<NET_StreamSocket*>(conn[i]))
						j++;

					assert(j < maxClients);

					std::cout << "Client " << (int)j << " disconnected!" << std::endl;

					clients[j] = nullptr;
					NET_DestroyStreamSocket(static_cast<NET_StreamSocket*>(conn[i]));
					conn[i] = conn[usedConn - 1];
					conn[usedConn - 1] = nullptr;
					--usedConn;

					// Tell all clients that client 'j' has disconnected
					MsgWithMasterId m;
					m.type = _CLIENT_DISCONNECTED;
					m.clientId = j;
					m.masterId = whoIsTheMaster();
					for (auto k = 1u; k < usedConn; k++) {
						SDLNetUtils::serialized_send(m,
							static_cast<NET_StreamSocket*>(conn[k]));
					}
					break; // Salir porque la lista cambió
				}
			}
		}
	}

	shutdown();
}

void TCPServer::shutdown() {
	if (serv == nullptr)
		return;

	Msg m;
	m.type = _SERVER_SHUTDOWN;

	// Dispose all client sockets
	for (auto i = 1u; i < usedConn; i++) {
		if (conn[i]) {
			SDLNetUtils::serialized_send(m, static_cast<NET_StreamSocket*>(conn[i]));
			NET_DestroyStreamSocket(static_cast<NET_StreamSocket*>(conn[i]));
		}
	}

	// Close the server's master socket
	NET_DestroyServer(serv);
	serv = nullptr;
	usedConn = 0;
}

int TCPServer::whoIsTheMaster() {
	for (auto i = 0u; i < maxClients; i++) {
		if (clients[i] != nullptr)
			return i;
	}
	return -1;
}