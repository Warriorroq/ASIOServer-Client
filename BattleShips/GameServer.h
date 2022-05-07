#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif 
#include <asio.hpp>
#include <thread>
#include "olc_net.h"

using namespace olc;
using namespace net;

enum class CustomMessages : uint32_t {
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage
};

class GameServer : public CommonServer<CustomMessages>{
public:
	static GameServer* instance;
	static void CreateServer();
	void StartServer();
	void StopServer();
protected:
	virtual bool OnClientConnect(std::shared_ptr<Connection<CustomMessages>> client)
	{
		return false;
	}

	virtual void OnClientDisconnect(std::shared_ptr<Connection<CustomMessages>> client)
	{

	}

	virtual void OnMessage(std::shared_ptr<Connection<CustomMessages>> client, Message<CustomMessages>& msg)
	{

	}
private:
	bool _isActive;
	GameServer(uint16_t port) : CommonServer<CustomMessages>(port) {
		_isActive = false;
	};
};