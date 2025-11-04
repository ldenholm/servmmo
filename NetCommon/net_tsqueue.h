#pragma once
#include "net_common.h"

namespace smmo
{
	namespace net
	{
		template<typename T>
		class tsqueue
		{
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete; // Disable copy
			virtual ~tsqueue() { clear(); }

		public:
			const T& front()
			{
				std::scoped_lock lock(mQueue);
				return dQueue.front();
			}

			const T& back()
			{
				std::scoped_lock lock(mQueue);
				return dQueue.back();
			}

			void push_back(const T& item)
			{
				std::scoped_lock lock(mQueue);
				dQueue.emplace_back(std::move(item));
			}

			void push_front(const T& item)
			{
				std::scoped_lock lock(mQueue);
				dQueue.emplace_front(std::move(item));
			}

			bool empty()
			{
				std::scoped_lock lock(mQueue);
				return dQueue.empty();
			}

			size_t count()
			{
				std::scoped_lock lock(mQueue);
				return dQueue.size();
			}

			void clear()
			{
				std::scoped_lock lock(mQueue);
				dQueue.clear();
			}

			T pop_front()
			{
				std::scoped_lock lock(mQueue);
				// We tell compiler we don't need the front ref for long
				// (since we're about to pop it) hence we can move
				// instead of copy the data for a much faster operation.
				auto t = std::move(dQueue.front());
				dQueue.pop_front();
				return t;
			}

			T pop_back()
			{
				std::scoped_lock lock(mQueue);
				auto t = std::move(dQueue.back());
				dQueue.pop_back();
				return t;
			}

		protected:
			// We care about queue internal state consistency considering
			// multiple threads trying to push data to our queue.
			std::mutex mQueue;
			std::deque<T> dQueue;
		};
	}
}