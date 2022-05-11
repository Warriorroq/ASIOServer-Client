#include "GameServer.h"
#include <iostream>
#include <thread>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

GameServer* GameServer::instance = 0;

void GameServer::CreateServer() {
	instance = new GameServer(60000);
}

void GameServer::StartServer() {
	_isActive = true;
	instance->Start();
	while (_isActive) {
		Update(1);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000/30));
	}
}

void GameServer::StopServer() {
	_isActive = false;
	Stop();
}