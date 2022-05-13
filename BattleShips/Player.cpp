#include "Player.h"

map<int, Player> Player::players = map<int, Player>();

Player::Player() {
	nickName = "guest";
}
Player::~Player() {
	nickName.clear();
}