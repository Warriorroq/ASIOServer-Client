#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif 
#define ASIO_STANDALONE
#include <asio.hpp>
using namespace asio;
using namespace ip;
class GameServer {
public:
	static GameServer* instance;
	static void CreateServer();
	void StartServer();
	void StopServer();
private:
	error_code _currentErrorCode;
	io_context _context;
	tcp::endpoint _endPoint;
	tcp::socket *_socket;
	GameServer();
};