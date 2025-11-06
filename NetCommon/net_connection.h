#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace smmo
{
	namespace net
	{
		template<typename T>
		class connection : public std::enable_shared_from_this<connection<T>>
		{
		public:
			connection()
			{}

			virtual ~connection()
			{}

		public:
			bool ConnectToServer();
			bool Disconnect();
			bool isConnected() const;

		public:
			bool Send(const message<T>& msg);

		protected:
			// Each connection has a unique socket to its remote.
			asio::ip::tcp::socket m_socket;

			// This context will provided by either server or client.
			asio::io_context& m_asioContext;

			// Queue containing all messages to be sent to remote side
			// of this connection.
			tsqueue<message<T>> m_qMessagesOut;


			// Queue containing messages that are being received from 
			// the remote side of this connection. It is a ref since the 
			// 'owner' of this connection is required to provide the queue.
			tsqueue<owned_message>& m_qMessagesIn;
		};

	}
}