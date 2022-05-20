#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif 
#include <asio.hpp>
#include <thread>
#include "olc_net.h"
#include "Player.h"
#include <ComfortableStream.h>
#include <memory>

class GameServer : public CommonServer<CustomMessages> {
public:
	static shared_ptr<GameServer> instance;
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
		Print("Removing client ");
		PrintLine(client->GetID());
		Player::players.erase(client->GetID());
	}

	virtual void OnMessage(std::shared_ptr<Connection<CustomMessages>> client, Message<CustomMessages>& msg)
	{
		switch (msg.header.id)
		{
		case CustomMessages::Disconnect: 
		{
			Message<CustomMessages> serverMessage;
			serverMessage.header.id = CustomMessages::PlayerAction;
			auto player = Player::players[client->GetID()];
			string disText = " disconnected";
			string text = "[Server] ";

			InputStringToMessage(serverMessage, disText);
			InputStringToMessage(serverMessage, player.nickName);
			InputStringToMessage(serverMessage, text);
			serverMessage << PlayerActions::SendedTextMessage;

			MessageAllClients(serverMessage);
		}
		break;
		case CustomMessages::ServerPing:
		{
			Print(client->GetID());
			PrintLine("Server Ping");
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
				msg << ']';
				InputStringToMessage(msg, player.nickName);
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