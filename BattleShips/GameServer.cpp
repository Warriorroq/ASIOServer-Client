#include "GameServer.h"
#include <iostream>
#include <chrono>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
using namespace std::chrono_literals;
GameServer* GameServer::instance = 0;

GameServer::GameServer() {
	_endPoint = tcp::endpoint(ip::make_address("93.184.216.34", _currentErrorCode), 80);
	_socket = new tcp::socket(_context);
	_socket->connect(_endPoint, _currentErrorCode);
	if (!_currentErrorCode)
	{
		std::cout << "connected" << std::endl;
	}
	else {
		std::cout << "error:" << _currentErrorCode.message() << std::endl;
	}
	if (_socket->is_open())
	{
		std::string req = 
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";
		_socket->write_some(buffer(req.data(), req.length()));
		std::this_thread::sleep_for(200ms);
		size_t bytes = _socket->available();
		std::cout << "bytes available:" << bytes <<std::endl;
		if (bytes > 0)
		{
			std::vector<char> vBuffer(bytes);
			_socket->read_some(buffer(vBuffer.data(), vBuffer.size()));
			for (auto c : vBuffer)
				std::cout << c;
		}
	}
}

void GameServer::CreateServer() {
	instance = new GameServer();
}

void GameServer::StartServer() {

}

void GameServer::StopServer() {

}