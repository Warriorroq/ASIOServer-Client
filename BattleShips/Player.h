#pragma once
#include <map>
#include <string>

using namespace std;

class Player {
public:
	Player();
	~Player();
	static map<int, Player> players;
	string nickName;
protected:

private:

};