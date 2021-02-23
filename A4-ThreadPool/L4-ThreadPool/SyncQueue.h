#pragma once

#include <queue>
#include <mutex>


template <class T>
class SyncQueue 
{

private:

	std::queue<T> _queue;
	std::mutex _mutex;

	//void lock() {
	//	std::unique_lock<std::mutex> lock(_mutex);
	//}

public:

	SyncQueue() = default;
	~SyncQueue() = default;


	void enqueue(T& t) {
		std::unique_lock<std::mutex> lock(_mutex);
		_queue.push(t);
	}

	T dequeue() {
		std::unique_lock<std::mutex> lock(_mutex);
		if (_queue.empty() == false)
		{
			T temp = std::move(_queue.front());
			_queue.pop();
			return temp;
		}
		else
		{
			return NULL;
		}
	}

	bool empty() {
		std::unique_lock<std::mutex> lock(_mutex);
		return _queue.empty();
	}

};