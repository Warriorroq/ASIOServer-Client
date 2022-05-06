#pragma once
#include "CommonHeaders.h"
#include "ThreadSaveQueue.h"
#include "Message.h"
#include "Connection.h"

using namespace asio;
using namespace ip;

namespace Net {

	template<typename T>
	class Client {
	public:
		ThreadSaveQueue<OwnedMessage<T>> qMessagesIn;

		Client: _socket(_context) {
			
		};

		virtual ~Client() {
			Disconnect();
		}

		bool Connect(const string& host, const uint16_t port) {
			try
			{
				_connection = std::make_unique<Connection<T>>();
				tcp::resolver resolver(_context);
				_endPoint = resolver.resolve(host, std::to_string(port));
				_connection->TryConnectToServer(_endPoint);
				_contextThread = std::thread([this]() {_context.run(); })
				return true;
			}
			catch(std::exception& e) {
				std::cout << e.what();
				return false;
			}
		}

		void Disconnect() {
			if (Connected()) {
				_connection->Disconnect();
			}

			_context.stop();

			if (_contextThread.joinable()) {
				_contextThread.join()
			}
			_connection.Release();
		}

		bool Connected() {
			if (_connection) {
				return _connection->Connected();
			}
			return false;
		}

	protected:
		io_context _context;
		std::thread _contextThread;
		tcp::socket _socket;
		unique_ptr<Connection<T>> _connection;
	};
}