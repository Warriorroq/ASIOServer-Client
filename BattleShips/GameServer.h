#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif 
#include <asio.hpp>
#include <thread>
#include "olc_net.h"

using namespace olc;
using namespace net;

class GameServer : public CommonServer<CustomMessages>{
public:
	static GameServer* instance;
	static void CreateServer();
	void StartServer();
	void StopServer();
protected:
	virtual bool OnClientConnect(std::shared_ptr<Connection<CustomMessages>> client)
	{
		Message<CustomMessages> msg;
		msg.header.id = CustomMessages::ServerAccept;
		client->Send(msg);
		return true;
	}

	virtual void OnClientDisconnect(std::shared_ptr<Connection<CustomMessages>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	virtual void OnMessage(std::shared_ptr<Connection<CustomMessages>> client, Message<CustomMessages>& msg)
	{
		switch (msg.header.id)
		{
		case CustomMessages::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";

			// Simply bounce message back to client
			client->Send(msg);
		}
		break;

		case CustomMessages::ClientMessage:
		{
			MessageAllClients(msg);
		}
		break;
		}
	}
private:
	bool _isActive;
	GameServer(uint16_t port) : CommonServer<CustomMessages>(port) {
		_isActive = false;
	};
};