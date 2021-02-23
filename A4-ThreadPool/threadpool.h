
//#ifndef THREAD_POOL_H
//#define THREAD_POOL_H
//
//class ThreadPool {
//public:
//    //used by main (you are free to change here and in main)
//    ThreadPool(size_t threads);
//    template<class F, class... Args>
//    auto enqueue(F&& f, Args&&... args);
//
//    /* todo: your implementation and more methods and variables*/
//};
//
//#endif


#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
#include <utility>
#include <future>
#include <cstdint>
#include ""

class ThreadPool
{ /* todo */
private:
	class WorkItem
	{
	private:
		ThreadPool* _ownerThreadpool;
		int nmbr;
	public:
		WorkItem(ThreadPool* ownerPool, int i)
		{
			_ownerThreadpool = ownerPool;
			nmbr = i;
		}

		void operator()()
		{
			std::function<void()> func;
			while (_ownerThreadpool->_running)
			{
				{
					std::unique_lock<std::mutex> lock(_ownerThreadpool->_mutex);
					if (_ownerThreadpool->_queue.empty())
					{
						_ownerThreadpool->_conditionLock.wait(lock);
					}
					func = _ownerThreadpool->_queue.dequeue();
				}
				if (func != NULL)
				{
					func();
				}
			}

			std::cout << "Bout to join" << std::endl;
		}
	};

	bool _running;
	std::vector<std::thread> _threads;
	MutexQueue<std::function<void()>> _queue;
	std::mutex _mutex;
	std::condition_variable _conditionLock;

public:
	ThreadPool(unsigned int n)
	{
		_threads = std::vector<std::thread>(n);
		_running = true;
		for (size_t i = 0; i < n; ++i)
		{
			_threads[i] = std::thread(WorkItem(this, i));
		}
	}

	~ThreadPool()
	{
		_running = false;
		_conditionLock.notify_all();
		for (size_t i = 0; i < _threads.size(); ++i)
		{
			if (_threads[i].joinable())
			{
				_threads[i].join();
			}
		}
	}

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;

	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;

	template <class F, class ... Args>
	auto enqueue(F& f, Args&& ... args) -> std::future<decltype(f(args...))>
	{
		std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		auto task = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);
		std::function<void()> out_function = [task]() {
			(*task)();
		};
		_queue.enqueue(out_function);
		_conditionLock.notify_one();
		return task->get_future();
	}
};

#endif