#include "GameClient.h"
#include <string.h>
#include <thread>
#include <olc_net.h>
#include <ComfortableStream.h>

GameClient client;
thread clientChatThread;

void GetMessages() {
	while (client.isActive)
	{
		client.RecieveMessage();
		WaitMiliseconds(1000 / 60);
	}
}

void StartClientChat() {
	client.CreatePlayerOnServer();

	while (client.isActive)
	{
		char command;
		cin >> command;

		if (command == 's')
			client.MessageAll();
		else if (command == 'e')
			client.ClientDisconnect();
		else if (command == 'p')
			client.PingServer();
		WaitMiliseconds(1000/60);
	}

	client.Disconnect();
}

int main() {
	string ip; 
	InputLine("enter ip:", ip);
	client.Connect(ip, 60000);

	clientChatThread = std::thread([]() {GetMessages();});
	WaitMiliseconds(100);

	StartClientChat();

	clientChatThread.join();
}