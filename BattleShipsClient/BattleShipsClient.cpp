#include "GameClient.h"
#include <string.h>
#include <thread>
#include <olc_net.h>
GameClient c;
std::thread clientChatThread;
bool bQuit = false;
void GetMessages() {
	while (!bQuit)
	{
		if (c.IsConnected())
		{
			if (!c.Incoming().IsEmpty())
			{
				auto msg = c.Incoming().PopFront().msg;

				switch (msg.header.id)
				{
				case CustomMessages::ServerAccept:
				{
					// Server has responded to a ping request				
					std::cout << "Server Accepted Connection\n";
				}
				break;


				case CustomMessages::ServerPing:
				{
					// Server has responded to a ping request
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
				}
				break;

				case CustomMessages::ClientMessage:
				{
					char letter;
					while(!msg.IsEmpty())
					{
						msg >> letter;
						std::cout << letter;
					}
					std::cout << '\n';
				}
				break;
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			bQuit = true;
		}

	}
}
void StartClientChat() {

	while (!bQuit)
	{
		std::cout << "command ";
		char command;
		std::cin >> command;
		if (command == 's')
			c.MessageAll();
		else if (command == 'e')
			bQuit = true;
		else if (command == 'p')
			c.PingServer();
	}
}
int main() {
	c.Connect("127.0.0.1", 60000);
	clientChatThread = std::thread([]() {GetMessages();});
	StartClientChat();
	clientChatThread.join();
}