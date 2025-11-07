#pragma once

#include "net_common.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace smmo {
	namespace net
	{

		// Responsible for establishing connection and asio. Acts
		// as an access point for application to communicate with
		// the server.
		template<typename T>
		class client_interface
		{
		public:
			client_interface() : m_socket(m_context)
			{

			}

			virtual ~client_interface()
			{
				Disconnect();
			}
			bool Connect(const std::string& host, const uint16_t port)
			{
				try
				{
					m_connection = std::make_unique<connection<T>>();

					asio::ip::tcp::resolver resolver(m_context);

					// Attempt to resolve ip:port from hostname.
					m_endpoints = resolver.resolve(host, std::to_string(port));

					m_connection->ConnectToServer(m_endpoints); // todo

					// Fire up asio context thread
					thrContext = std::thread([this]() { m_context.run(); })
				}
				return false;
			}

			void Disconnect()
			{
				if (IsConnected())
				{
					m_connection->Disconnect();
				}

				// Terminate asio context.
				m_context.stop();
				// Await thread completion.
				if (thrContext.joinable())
					thrContext.join();
			}

			bool IsConnected()
			{
				if (m_connection)
					return m_connection->IsConnected();
				else
					return false;
			}

			tsqueue<owned_message<T>>& Incoming()
			{
				return m_qMessagesIn;
			}

		protected:
			// asio context for async operations.
			asio::io_context m_context;
			std::thread thrContext;
			asio::ip::tcp::socket m_socket;
			// Single connection object per client for handling
			// data transfer.
			std::unique_ptr<connection<T>> m_connection;
		private:
			// Incoming messages from the server.
			tsqueue<owned_message<T>> m_qMessagesIn;
		};
	}
}