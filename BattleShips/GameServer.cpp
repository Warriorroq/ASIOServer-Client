#include "GameServer.h"
#include <iostream>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

GameServer* GameServer::instance = 0;

void GameServer::CreateServer() {
	uint16_t port;
	std::cin >> port;
	instance = new GameServer(port);
}

void GameServer::StartServer() {
	_isActive = true;
	instance->Start();
	while (_isActive) {
		Update();
	}
}

void GameServer::StopServer() {
	_isActive = false;
}