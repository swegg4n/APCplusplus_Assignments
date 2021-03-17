#pragma once

#include "WeakPtr.hpp"
#include "Counter.hpp"

template<class T> class WeakPtr;


template <class T>
class SharedPtr
{
public:
	friend class WeakPtr<T>;


private:

	T* _ptr;
	Counter<T>* _counter;


	void add_use(T* other)
	{
		if (other != nullptr)
		{
			if (_counter != nullptr)
			{
				_counter->Increment_shared();
			}
			else
			{
				_counter = new Counter<T>();
			}
		}
	}

	void remove_use()
	{
		if (_counter != nullptr && _counter->Decrement_shared() == 0)
		{
			delete _ptr;

			if (_counter->Decrement_weak() == 0)
			{
				delete _counter;
			}
		}

		_ptr = nullptr;
		_counter = nullptr;
	}


public:

	SharedPtr() : _ptr(nullptr), _counter(nullptr) {}

	SharedPtr(T* other) : _ptr(other), _counter(new Counter<T>()) {}

	SharedPtr(const std::nullptr_t) : _ptr(nullptr), _counter(nullptr) {}

	SharedPtr(const WeakPtr<T>& other) : _ptr(other._ptr), _counter(nullptr)
	{
		if (other.expired())
			throw std::exception("Construction of SharedPtr FAIL - WeakPtr is expired");
		else
			add_use(other._ptr);
	}

	SharedPtr(SharedPtr& other) : _ptr(nullptr), _counter(nullptr)
	{
		if (this != &other)
		{
			_ptr = other._ptr;
			_counter = other._counter;

			add_use(_ptr);
		}
	}

	SharedPtr(SharedPtr&& other) : _ptr(nullptr), _counter(nullptr)
	{
		if (this != &other)
		{
			_ptr = other._ptr;
			_counter = other._counter;

			add_use(_ptr);
			other.remove_use();
		}
	}

	~SharedPtr()
	{
		remove_use();
	}


	SharedPtr& operator=(const SharedPtr& other) noexcept
	{
		if (this != &other)
		{
			remove_use();

			_ptr = other._ptr;
			_counter = other._counter;

			add_use(_ptr);
		}

		return *this;
	}

	SharedPtr& operator=(SharedPtr&& other) noexcept
	{
		if (this != &other)
		{
			remove_use();

			_ptr = other._ptr;
			_counter = other._counter;

			add_use(_ptr);
			other.remove_use();
		}

		return *this;
	}


	bool operator==(const std::nullptr_t) const
	{
		return _ptr == nullptr;
	}
	bool operator<(const std::nullptr_t rhs) const
	{
		return _ptr < *rhs.get();
	}
	bool operator==(const SharedPtr& rhs) const
	{
		return _ptr == rhs.get();
	}
	bool operator<(const SharedPtr& rhs) const
	{
		return _ptr < rhs.get();
	}


	T& operator*()
	{
		return *_ptr;
	}

	T* operator->()
	{
		return _ptr;
	}


	operator bool() const
	{
		return _ptr != nullptr;
	}


	void reset() noexcept
	{
		//SharedPtr{}.swap(*this);
		remove_use();
	}

	T* get()
	{
		return _ptr;
	}

	const T* get() const
	{
		return _ptr;
	}

	size_t use_count()
	{
		if (_ptr != nullptr)
		{
			return _counter->Shared_useCount();
		}
		else
		{
			return 0;
		}
	}

	const size_t use_count() const
	{
		if (_ptr != nullptr)
		{
			return _counter->Shared_useCount();
		}
		else
		{
			return 0;
		}
	}


	void swap(SharedPtr<T>& rhs) noexcept
	{
		std::swap(_ptr, rhs._ptr);
		std::swap(_counter, rhs._counter);
	}


	bool Invariant()
	{
		return (_ptr == nullptr && _counter == nullptr)
			|| (_ptr != nullptr && _counter != nullptr);
	}

};


template<class T>
void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept
{
	lhs.swap(rhs);
}

template<class T, class... Args>
SharedPtr<T> MakeShared(Args&&... args)
{
	return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
