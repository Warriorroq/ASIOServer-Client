#pragma once
#include <net_client.h>
#include <olc_net.h>
#include <string.h>
#include <iostream>
using namespace olc;
using namespace net;

class GameClient : public CommonClient<CustomMessages> {
public:
	void PingServer() {
		Message<CustomMessages> msg;
		msg.header.id = CustomMessages::ServerPing;
		auto time = std::chrono::system_clock::now();
		msg << time;
		Send(msg);
	}
	void MessageAll()
	{
		Message<CustomMessages> msg;
		msg.header.id = CustomMessages::ClientMessage;
		std::string s;
		std::getline(std::cin, s);
		for (int i = s.size() - 1; i >= 0; i--) {
			msg << s[i];
		}
		Send(msg);
	}
};