#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class CommonServer
		{
		public:
			CommonServer(uint16_t port)
				: p_asioAcceptor(p_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
			{

			}

			virtual ~CommonServer()
			{
				Stop();
			}

			bool Start()
			{
				try
				{
					WaitForClientConnection();

					p_threadContext = std::thread([this]() { p_asioContext.run(); });
				}
				catch (std::exception& e)
				{
					std::cerr << "[SERVER] Exception: " << e.what() << "\n";
					return false;
				}

				std::cout << "[SERVER] Started!\n";
				return true;
			}

			void Stop()
			{
				p_asioContext.stop();

				if (p_threadContext.joinable()) 
				{ 
					p_threadContext.join(); 
				}

				std::cout << "[SERVER] Stopped!\n";
			}

			// ASYNC - Instruct asio to wait for connection
			void WaitForClientConnection()
			{
				p_asioAcceptor.async_accept(
					[this](std::error_code ec, asio::ip::tcp::socket socket)
					{
						if (!ec)
						{
							std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";

							std::shared_ptr<Connection<T>> newconn = 
								std::make_shared<Connection<T>>(Connection<T>::owner::server, 
									p_asioContext, std::move(socket), p_qMessagesIn);
						
							if (OnClientConnect(newconn))
							{								
								p_deqConnections.push_back(std::move(newconn));

								p_deqConnections.back()->ConnectToClient(p_nIDCounter++);

								std::cout << "[" << p_deqConnections.back()->GetID() << "] Connection Approved\n";
							}
							else
							{
								std::cout << "[-----] Connection Denied\n";
							}
						}
						else
						{
							std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
						}

						WaitForClientConnection();
					});
			}

			void MessageClient(std::shared_ptr<Connection<T>> client, const Message<T>& msg)
			{
				if (client && client->IsConnected())
				{
					client->Send(msg);
				}
				else
				{
					OnClientDisconnect(client);

					client.reset();

					p_deqConnections.erase(
						std::remove(p_deqConnections.begin(), p_deqConnections.end(), client), p_deqConnections.end());
				}
			}
			
			void MessageAllClients(const Message<T>& msg, std::shared_ptr<Connection<T>> pIgnoreClient = nullptr)
			{
				bool bInvalidClientExists = false;

				for (auto& client : p_deqConnections)
				{
					if (client && client->IsConnected())
					{
						if (client != pIgnoreClient)
						{
							client->Send(msg);
						}
					}
					else
					{
						OnClientDisconnect(client);
						client.reset();
						bInvalidClientExists = true;
					}
				}

				if (bInvalidClientExists) 
				{
					p_deqConnections.erase(
						std::remove(p_deqConnections.begin(), p_deqConnections.end(), nullptr), p_deqConnections.end());
				}
			}

			void Update(size_t nMaxMessages = -1, bool bWait = false)
			{
				if (bWait)
				{
					p_qMessagesIn.Wait();
				}

				size_t nMessageCount = 0;
				while (nMessageCount < nMaxMessages && !p_qMessagesIn.IsEmpty())
				{
					auto msg = p_qMessagesIn.PopFront();
					OnMessage(msg.remote, msg.msg);
					nMessageCount++;
				}
			}

		protected:
			virtual bool OnClientConnect(std::shared_ptr<Connection<T>> client)
			{
				return false;
			}

			virtual void OnClientDisconnect(std::shared_ptr<Connection<T>> client)
			{

			}

			virtual void OnMessage(std::shared_ptr<Connection<T>> client, Message<T>& msg)
			{

			}

			ThreadSaveQueue<OwnedMessage<T>> p_qMessagesIn;
			std::deque<std::shared_ptr<Connection<T>>> p_deqConnections;
			asio::io_context p_asioContext;
			std::thread p_threadContext;
			asio::ip::tcp::acceptor p_asioAcceptor;
			uint32_t p_nIDCounter = 100;
		};
	}
}