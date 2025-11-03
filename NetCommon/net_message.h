#pragma once
#include "net_common.h"

namespace smmo
{
	namespace net
	{

		// Using a template here to allow enum header types to easily 
		// identify messages (validation at compilation).
		template <typename T>
		struct message_header
		{
			T id{};
			uint32_t size = 0;
		};

		template <typename T>
		struct message
		{
			message_header<T> header{};
			std::vector<uint8_t> body;

			size_t size() const
			{
				body.size
				return sizeof(header_message<T> + body.size());
			}

			// Overwrite << operator to easily inspect packets.
			friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
			{
				os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
				return os;
			}
		};
	}
}