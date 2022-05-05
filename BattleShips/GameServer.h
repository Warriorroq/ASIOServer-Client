#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif 
#include <asio.hpp>
#include <thread>
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
	io_context::work *_contextWork;
	std::thread _contextThread;
	tcp::endpoint _endPoint;
	tcp::socket *_socket;
	GameServer();
};