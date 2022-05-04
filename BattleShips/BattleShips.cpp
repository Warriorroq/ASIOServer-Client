#include <iostream>
#include "GameServer.h"
using namespace std;
void SetUpServer() {
    char answer;
    cout << "Set up server? y/n" <<endl;
    cin >> answer;
    if (answer != 'y')
        return;
    GameServer::CreateServer();
    GameServer::instance->StartServer();
}
int main()
{
    SetUpServer();
}