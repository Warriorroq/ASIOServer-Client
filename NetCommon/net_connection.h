#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class Connection : public std::enable_shared_from_this<Connection<T>>
		{
		public:
			enum class Owner
			{
				server,
				client
			};

			Connection(Owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket, ThreadSaveQueue<OwnedMessage<T>>& qIn)
				: p_asioContext(asioContext), m_socket(std::move(socket)), p_qMessagesIn(qIn)
			{
				p_nOwnerType = parent;
			}

			virtual ~Connection()
			{}
			uint32_t GetID()
			{
				return id;
			}

			void ConnectToClient(uint32_t uid = 0)
			{
				if (p_nOwnerType == Owner::server)
				{
					if (m_socket.is_open())
					{
						id = uid;
						ReadHeader();
					}
				}
			}

			void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
			{
				if (p_nOwnerType == Owner::client)
				{
					asio::async_connect(m_socket, endpoints,
						[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
						{
							if (!ec)
							{
								ReadHeader();
							}
						});
				}
			}

			void Disconnect()
			{
				if (IsConnected()) 
				{
					asio::post(p_asioContext, [this]() { m_socket.close(); });
				}
			}

			bool IsConnected()
			{
				return m_socket.is_open();
			}

			void StartListening()
			{
				
			}
			// ASYNC
			void Send(const Message<T>& msg)
			{
				asio::post(p_asioContext,
					[this, msg]()
					{
						bool bWritingMessage = !p_qMessagesOut.IsEmpty();
						p_qMessagesOut.PushBack(msg);
						if (!bWritingMessage)
						{
							WriteHeader();
						}
					});
			}
		private:
			// ASYNC
			void WriteHeader()
			{
				asio::async_write(m_socket, asio::buffer(&p_qMessagesOut.Front().header, sizeof(MessageHeader<T>)),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							if (p_qMessagesOut.Front().body.size() > 0)
							{
								WriteBody();
							}
							else
							{
								p_qMessagesOut.PopFront();
								if (!p_qMessagesOut.IsEmpty())
								{
									WriteHeader();
								}
							}
						}
						else
						{
							std::cout << "[" << id << "] Write Header Fail.\n";
							m_socket.close();
						}
					});
			}

			// ASYNC
			void WriteBody()
			{
				asio::async_write(m_socket, asio::buffer(p_qMessagesOut.Front().body.data(), p_qMessagesOut.Front().body.size()),
					[this](std::error_code ec, std::size_t length)
					{
						if (!ec)
						{
							p_qMessagesOut.PopFront();
							if (!p_qMessagesOut.IsEmpty())
							{
								WriteHeader();
							}
						}
						else
						{
							std::cout << "[" << id << "] Write Body Fail.\n";
							m_socket.close();
						}
					});
			}

			// ASYNC
			void ReadHeader()
			{
				asio::async_read(m_socket, asio::buffer(&p_msgTemporaryIn.header, sizeof(MessageHeader<T>)),
					[this](std::error_code ec, std::size_t length)
					{						
						if (!ec)
						{
							if (p_msgTemporaryIn.header.size > 0)
							{
								p_msgTemporaryIn.body.resize(p_msgTemporaryIn.header.size);
								ReadBody();
							}
							else
							{
								AddToIncomingMessageQueue();
							}
						}
						else
						{
							std::cout << "[" << id << "] Read Header Fail.\n";
							m_socket.close();
						}
					});
			}

			// ASYNC
			void ReadBody()
			{
				asio::async_read(m_socket, asio::buffer(p_msgTemporaryIn.body.data(), p_msgTemporaryIn.body.size()),
					[this](std::error_code ec, std::size_t length)
					{						
						if (!ec)
						{
							AddToIncomingMessageQueue();
						}
						else
						{
							std::cout << "[" << id << "] Read Body Fail.\n";
							m_socket.close();
						}
					});
			}

			void AddToIncomingMessageQueue()
			{				
				if (p_nOwnerType == Owner::server)
				{
					p_qMessagesIn.PushBack({ this->shared_from_this(), p_msgTemporaryIn });
				}
				else 
				{
					p_qMessagesIn.PushBack({ nullptr, p_msgTemporaryIn });
				}

				ReadHeader();
			}

		protected:
			asio::ip::tcp::socket m_socket;
			asio::io_context& p_asioContext;
			ThreadSaveQueue<Message<T>> p_qMessagesOut;
			ThreadSaveQueue<OwnedMessage<T>>& p_qMessagesIn;
			Message<T> p_msgTemporaryIn;
			Owner p_nOwnerType = Owner::server;
			uint32_t id = 0;
		};
	}
}