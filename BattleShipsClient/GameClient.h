#pragma once
#include <net_client.h>
#include <olc_net.h>
#include <string.h>
#include <iostream>
#include <ComfortableStream.h>

using namespace olc;
using namespace net;
using namespace std;

class GameClient : public CommonClient<CustomMessages> {
public:
	void PingServer() {
		Message<CustomMessages> msg;
		msg.header.id = CustomMessages::ServerPing;

		auto time = chrono::system_clock::now();
		msg << time;

		Send(msg);
	}

	void MessageAll()
	{
		Message<CustomMessages> msg;
		msg.header.id = CustomMessages::PlayerAction;

		string s;
		InputLine("", s);
		InputStringToMessage(msg, s);
		msg << PlayerActions::SendedTextMessage;

		Send(msg);
	}

	void CreatePlayerOnServer() {
		Message<CustomMessages> msg;
		msg.header.id = CustomMessages::CreatePlayer;

		PrintLine("enter nickname:");
		string s;
		InputLine("", s);
		InputStringToMessage(msg, s);

		Send(msg);
	}
	void RecieveMessage() {
		if (IsConnected())
		{
			if (!Incoming().IsEmpty())
			{
				auto msg = Incoming().PopFront().msg;

				switch (msg.header.id)
				{
				case CustomMessages::ServerAccept:
				{	
					PrintLine("Server Accepted Connection");
				}
				break;


				case CustomMessages::ServerPing:
				{
					chrono::system_clock::time_point timeNow = chrono::system_clock::now();
					chrono::system_clock::time_point timeThen;
					msg >> timeThen;

					Print("Ping:");
					PrintLine(chrono::duration<double>(timeNow - timeThen).count());
				}
				break;

				case CustomMessages::PlayerAction:
				{
					PlayerActions action;
					msg >> action;

					if (action != PlayerActions::SendedTextMessage)
						return;

					char letter;
					while (!msg.IsEmpty())
					{
						msg >> letter;
						Print(letter);
					}

					PrintLine("");
				}
				break;
				}
			}
		}
		else
		{
			PrintLine("Server Down");
		}
	}
};