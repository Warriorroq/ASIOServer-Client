#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif 
#include <asio.hpp>
#include <thread>
#include "olc_net.h"
#include "Player.h"

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
		Message<CustomMessages> message;
		message.header.id = CustomMessages::PlayerAction;
		auto player = Player::players[client->GetID()];

		for (int i = player.nickName.size() - 1; i >= 0; i--) {
			message << player.nickName[i];
		}

		string s = " left the session";
		for (int i = s.size() - 1; i >= 0; i--) {
			message << s[i];
		}
		message << PlayerActions::SendedTextMessage;
		MessageAllClients(message);
		Player::players.erase(client->GetID());
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

		case CustomMessages::PlayerAction:
		{
			PlayerActions action;
			msg >> action;
			if (action == PlayerActions::SendedTextMessage)
			{
				auto player = Player::players[client->GetID()];
				msg << "] ";
				for (int i = player.nickName.size() - 1; i >= 0; i--) {
					msg << player.nickName[i];
				}
				msg << '[';
				msg << PlayerActions::SendedTextMessage;
				MessageAllClients(msg);
			}
		}
		case CustomMessages::CreatePlayer:
		{
			string nickName = string();
			char letter;
			while (!msg.IsEmpty()) 
			{
				msg >> letter;
				nickName.push_back(letter);
			}
			Player player = Player();
			player.nickName = nickName;
			Player::players.insert(pair<int, Player>(client->GetID(), player));
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