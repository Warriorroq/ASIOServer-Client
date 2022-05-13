#include <iostream>
#include "GameServer.h"
using namespace std;
thread serverThread;
void SetUpServer() {
    GameServer::CreateServer();
    serverThread = thread([]() { GameServer::instance->StartServer(); });
}
int main()
{
    SetUpServer();
    while (true) {
        char command;
        cin >> command;
        if (command == 'p')
            for (auto p : Player::players)
                cout << p.first << " " << p.second.nickName << endl;
    }
}