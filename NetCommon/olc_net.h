//GitHub:		https://www.github.com/onelonecoder
#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_client.h"
#include "net_server.h"
#include "net_connection.h"

enum class CustomMessages : uint32_t {
	ServerPing,
	ServerMessage,
	ServerAccept,
	PlayerAction,
	CreatePlayer,
	Disconnect
};

enum class PlayerActions : uint32_t {
	SendedTextMessage,
	Shoot,
};