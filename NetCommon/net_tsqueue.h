#pragma once

#include "net_common.h"

namespace olc
{
	namespace net
	{
		template<typename T>
		class ThreadSaveQueue
		{
		public:
			ThreadSaveQueue() = default;
			ThreadSaveQueue(const ThreadSaveQueue<T>&) = delete;
			virtual ~ThreadSaveQueue() { Clear(); }

			const T& Front()
			{
				p_muxQueue.lock();
				auto& var = p_deqQueue.front();
				p_muxQueue.unlock();
				return var;
			}

			const T& Back()
			{
				p_muxQueue.lock();
				auto& var = p_deqQueue.back();
				p_muxQueue.unlock();
				return var;
			}

			T PopFront()
			{
				p_muxQueue.lock();
				auto t = std::move(p_deqQueue.front());
				p_deqQueue.pop_front();
				p_muxQueue.unlock();
				return t;
			}

			T PopBack()
			{
				p_muxQueue.lock();
				auto t = std::move(p_deqQueue.back());
				p_deqQueue.pop_back();
				p_muxQueue.unlock();
				return t;
			}

			void PushBack(const T& item)
			{
				p_muxQueue.lock();
				p_deqQueue.emplace_back(std::move(item));

				std::unique_lock<std::mutex> ul(p_muxBlocking);
				p_cvBlocking.notify_one();
				p_muxQueue.unlock();
			}

			void PushFront(const T& item)
			{
				p_muxQueue.lock();
				p_deqQueue.emplace_front(std::move(item));

				std::unique_lock<std::mutex> ul(p_muxBlocking);
				p_cvBlocking.notify_one();
				p_muxQueue.unlock();
			}

			bool IsEmpty()
			{
				p_muxQueue.lock();
				auto empty = p_deqQueue.empty();
				p_muxQueue.unlock();
				return empty;
			}

			size_t Count()
			{
				p_muxQueue.lock();
				auto size = p_deqQueue.size();
				p_muxQueue.unlock();
				return size;
			}

			void Clear()
			{
				p_muxQueue.lock();
				p_deqQueue.clear();
				p_muxQueue.unlock();
			}

			void Wait()
			{
				while (IsEmpty())
				{
					std::unique_lock<std::mutex> ul(p_muxBlocking);
					p_cvBlocking.wait(ul);
				}
			}

		protected:
			std::mutex p_muxQueue;
			std::deque<T> p_deqQueue;
			std::condition_variable p_cvBlocking;
			std::mutex p_muxBlocking;
		};
	}
}