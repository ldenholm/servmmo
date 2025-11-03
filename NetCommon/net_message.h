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
				return sizeof(header_message<T> + body.size());
			}

			// Overwrite << operator to easily inspect packets.
			friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
			{
				os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
				return os;
			}

			/* Push any data into our message body.
				Example: Let int x, y be some variables.
						 Let msg be our message<T>.

				Usage: msg << x << y (pushing data in).
					   msg >> x >> y (pulling data out).
			*/
			template<typename DataType>
			friend message<T>& operator << (message<T>& msg, const DataType& data)
			{
				// Check data can be pushed into body vector with no fancy shenanigans.
				static_assert(std::is_standard_layout<DataType>::value, "Too complicated \
					to push this data into message body vector. ");

				// Save current body size of message as this is where we begin to insert data.
				size_t i = msg.body.size();

				// Resize the body to allow additional space to store new data.
				msg.body.resize(i + sizeof(Datatype));

				// Push (copy) the new data into our message body.
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType);

				// Recompute the message size.
				msg.header.size = msg.size();
				return msg;
			}

			/* Pull any data out of our message body.
				Example: Let int x, y be some variables.
						 Let msg be our message<T>.

				Usage: msg >> x >> y (pulling data out).
			*/
			template<typename DataType>
			friend message<T> operator >> (message<T>& msg, DataType& data)
			{
				// Check data can easily be extracted.
				static_assert(std::is_standard_layout < DataType::value, "Data cannot easily \
					be extraced.");

				size_t bytes = msg.body.size() - sizeof(DataType);

				/*
				
				|--------|---------|
				| msg    |DataType-|
				|--------|---------|
				
				*/

				std::memcpy(&data, msg.body.data() + bytes, sizeof(DataType);

				// Resize the message body.
				msg.body.resize(bytes);

				// Recompute the entire message size
				msg.header.size = msg.size();

				return msg;

			}
		};
	}
}