#pragma once
#include "CommonHeaders.h"
#include "ThreadSaveQueue.h"
#include "Message.h"

using namespace asio;
using namespace ip;

namespace Net {
	template<typename T>
	class Connection : public enable_shared_from_this<Connection<T>> {
	public:
		Connection() {

		}

		virtual ~Connection() {

		}
		
		bool TryConnectToServer();
		bool Disconnect();
		bool Connected() const;
		bool Send(const Message<T>& message);
	protected:
		tcp::socket socket;
		io_context& asioContext;
		ThreadSaveQueue<Message<T>> qMessagesToOut;
		ThreadSaveQueue<Message<T>> qMessagesToIn;
	};
}