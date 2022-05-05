#include "GameServer.h"
#include <iostream>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <NetMessage.h>
GameServer* GameServer::instance = 0;

GameServer::GameServer() {
	_contextWork = new io_context::work(_context);
	_endPoint = tcp::endpoint(ip::make_address("93.184.216.34", _currentErrorCode), 80);
	_socket = new tcp::socket(_context);
	_socket->connect(_endPoint, _currentErrorCode);
	_contextThread = std::thread([&]() {_context.run(); });
}

void GameServer::CreateServer() {
	instance = new GameServer();
}

void GameServer::StartServer() {

}

void GameServer::StopServer() {

}