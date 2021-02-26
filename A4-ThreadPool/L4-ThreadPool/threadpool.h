
#ifndef thread_pool_h
#define thread_pool_h

#include <thread>
#include <vector>
#include <future>
#include "SyncQueue.h"


class threadpool {

private:

	class Task
	{
	private:

		threadpool* _threadpool;


	public:

		Task(threadpool* threadpool) {
			this->_threadpool = threadpool;
		}

		void operator()() {
			std::function<void()> f;

			while (_threadpool->_running)
			{
				std::unique_lock<std::mutex> lock(_threadpool->_mutex);
				while (_threadpool->_queue.empty())
					_threadpool->_conditionLock.wait(lock);

				f = _threadpool->_queue.dequeue();

				if (f != NULL)
					f();
			}
		}

	};


	bool _running;
	std::vector<std::thread> _threads;
	SyncQueue<std::function<void()>> _queue;
	std::condition_variable _conditionLock;
	std::mutex _mutex;


public:

	threadpool(size_t threads) {
		_threads = std::vector<std::thread>(threads);
		for (size_t i = 0; i < threads; i++)
		{
			_threads[i] = std::thread(Task(this));
		}
		_running = true;
	}

	~threadpool() {
		_running = false;
		_conditionLock.notify_all();
		for (size_t i = 0; i < _threads.size(); i++)
		{
			_threads[i].join();
		}
	}


	threadpool(const threadpool&) = delete;
	threadpool& operator=(threadpool&&) = delete;


	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args) {
		auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		std::function<void()> out_f = [task]() {
			(*task)();
		};
		_queue.enqueue(out_f);
		_conditionLock.notify_one();
		return task->get_future();
	}

};

#endif
