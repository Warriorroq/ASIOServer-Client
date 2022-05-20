#include "GameServer.h"
#include <iostream>
#include <thread>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

shared_ptr<GameServer> GameServer::instance = 0;

void GameServer::CreateServer() {
	instance = shared_ptr<GameServer>(new GameServer(60000));
}

void GameServer::StartServer() {
	_isActive = true;
	instance->Start();
	while (_isActive) {
		Update(1);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
	}
}

void GameServer::StopServer() {
	_isActive = false;
	Stop();
}