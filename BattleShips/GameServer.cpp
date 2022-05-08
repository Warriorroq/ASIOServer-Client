#include "GameServer.h"
#include <iostream>
#include <thread>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

GameServer* GameServer::instance = 0;

void GameServer::CreateServer() {
	instance = new GameServer(8000);
}

void GameServer::StartServer() {
	_isActive = true;
	instance->Start();
	while (_isActive) {
		Update(1);
		std::cout << "updated"<<std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

void GameServer::StopServer() {
	_isActive = false;
	Stop();
}