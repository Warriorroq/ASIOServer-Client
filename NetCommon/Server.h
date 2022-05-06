#pragma once
#include "CommonHeaders.h"
#include "ThreadSaveQueue.h"
#include "Message.h"
#include "Connection.h"

using namespace asio;
using namespace ip;

namespace Net {
	template<typename T>
	class Server {
	public:
		Server(uint16_t port) : asioAcceptor(asioContext, tcp::endpoint(tcp::v4(), port)) {

		};

		virtual ~Server() {
			Stop();
		}
		
		bool Start() {
			try {
				WaitForClientConnection();
				threadContext = std::thread([this]() { asioContext.run(); });
			}
			catch(std::exception& e) {
				std::cerr << "[Server] Love you! " << e.what();
				return false;
			}
			std::cout << "[Server] Started!"<<std::endl;
			return true;
		}

		void Stop() {
			asioContext.stop();
			if (threadContext.joinable()) {
				threadContext.join();
			}
			std::cout << "[Server] Stopped!" << std::endl;
		}

		//ASYNC
		void WaitForClientConnection() {
			asioAcceptor.async_accept(
				[this](std::error_code e, tcp::socket socket) {
					if (!e) {
						std::cout << "[Server]  New Connection:" << socket.remote_endpoint() << std::endl;
						std::shared_ptr<Connection<T>> newConnection = std::shared_ptr<Connection<T>>(Connection<T>::owner::server, asioContext, std::move(socket), qMessagesIn);
						if (OnClientConnect(newConnection)) {

						}
						else {
							std::cout << "[Server] Connection denied" << endl;
						}
					}
					else {
						std::cout << "[Server]  New Connection Error:"<< e.message() << std::endl;
					}
				}
			);
		}

		void MessageClient(std::shared_ptr<Connection<T>> client, const Message<T>& message) {

		}

		void MessageAllClients(const Message<T>& message, std::shared_ptr<Connection<T>> clientToIgnore = nullptr) {

		}

	protected:
		ThreadSaveQueue<OwnedMessage<T>> qMessagesIn;
		io_context asioContext;
		std::thread threadContext;
		tcp::acceptor asioAcceptor;
		uint32_t idCounter = 10000;

		virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client) {
			return false;
		}
		
		virtual void OnClientDisconnected(std::shared_ptr<Connection<T>> client) {

		}

		virtual void OnMessage(std::shared_ptr<Connection<T>> client, Message<T>& message) {

		}
	};
}