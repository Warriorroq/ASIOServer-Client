#pragma once
#include "net_common.h"

namespace olc
{
	namespace net
	{
		template <typename T>
		class CommonClient
		{
		public:
			CommonClient() 
			{}

			virtual ~CommonClient()
			{
				Disconnect();
			}

			bool Connect(const std::string& host, const uint16_t port)
			{
				try
				{
					asio::ip::tcp::resolver resolver(p_context);
					asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
					p_connection = std::make_unique<Connection<T>>(Connection<T>::owner::client, p_context, asio::ip::tcp::socket(p_context), _qMessagesIn);
					p_connection->ConnectToServer(endpoints);
					p_thrContext = std::thread([this]() { p_context.run(); });
				}
				catch (std::exception& e)
				{
					std::cerr << "Client Exception: " << e.what() << "\n";
					return false;
				}
				return true;
			}

			void Disconnect()
			{
				if(IsConnected())
				{
					p_connection->Disconnect();
				}
				p_context.stop();
				if (p_thrContext.joinable()) 
				{
					p_thrContext.join();
				}
				p_connection.release();
			}

			bool IsConnected()
			{
				if (p_connection)
					return p_connection->IsConnected();
				else
					return false;
			}

			void Send(const Message<T>& msg)
			{
				if (IsConnected()) 
				{
					p_connection->Send(msg);
				}
			}

			ThreadSaveQueue<OwnedMessage<T>>& Incoming()
			{ 
				return _qMessagesIn;
			}

		protected:
			asio::io_context p_context;
			std::thread p_thrContext;
			std::unique_ptr<Connection<T>> p_connection;
			
		private:
			ThreadSaveQueue<OwnedMessage<T>> _qMessagesIn;
		};
	}
}