#include <iostream>
#include "GameServer.h"
using namespace std;
thread serverThread;
void SetUpServer() {
    char answer;
    cout << "Set up server? y/n" <<endl;
    cin >> answer;
    if (answer != 'y')
        return;
    GameServer::CreateServer();
    serverThread = thread([]() { GameServer::instance->StartServer(); });
}
int main()
{
    SetUpServer();
    this_thread::sleep_for(chrono::seconds(4));
    GameServer::instance->StopServer();
    serverThread.join();
}