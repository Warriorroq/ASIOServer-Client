#pragma once
#include "CommonHeaders.h"

using namespace std;

template<typename T>
class ThreadSaveQueue {
public:
	ThreadSaveQueue() = default;
	ThreadSaveQueue(const ThreadSaveQueue<T>&) = delete;
	virtual ~ThreadSaveQueue() { 
		Clear(); 
	}

	const T& Front() {
		scoped_lock lock(muxQueue);
		return deqQueue.front();
	}

	const T& Back() {
		scoped_lock lock(muxQueue);
		return deqQueue.back();
	}

	void PushBack(const T& item) {
		scoped_lock lock(muxQueue);
		deqQueue.emplace_back(move(item));
	}

	void PushFront(const T& item) {
		scoped_lock lock(muxQueue);
		deqQueue.emplace_front(move(item));
	}

	bool Empty() {
		scoped_lock lock(muxQueue);
		return deqQueue.empty();
	}

	size_t Count() {
		scoped_lock lock(muxQueue);
		return deqQueue.size();
	}

	void Clear() {
		scoped_lock lock(muxQueue);
		deqQueue.clear();
	}

	T Pop() {
		scoped_lock lock(muxQueue);
		auto item = move(deqQueue.front());
		deqQueue.pop_front();
		return item;
	}

protected:
	mutex muxQueue;
	deque<T> deqQueue;
};