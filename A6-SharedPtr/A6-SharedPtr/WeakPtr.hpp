#pragma once

#include "SharedPtr.hpp"
#include "Counter.hpp"

template<class T> class SharedPtr;


template <class T>
class WeakPtr
{

public:
	friend class SharedPtr<T>;

private:

	T* _ptr;
	Counter<T>* _counter;


public:

	WeakPtr() : _ptr(nullptr), _counter(nullptr) {}

	WeakPtr(const WeakPtr<T>& other) : _ptr(other._ptr), _counter(other._counter)
	{
		if (_counter != nullptr) _counter->Increment_weak();
	}

	WeakPtr(const SharedPtr<T>& other) : _ptr(other._ptr), _counter(other._counter)
	{
		if (_counter != nullptr) _counter->Increment_weak();
	}

	~WeakPtr()
	{
		if (_counter != nullptr && _counter->Decrement_weak() == 0)
		{
			if (_counter->Shared_useCount() == 0)
			{
				delete _counter;
			}
		}
	}


	WeakPtr<T>& operator=(const WeakPtr<T>& other)
	{
		WeakPtr(other).swap(*this);

		return *this;
	}

	WeakPtr<T>& operator=(const SharedPtr<T>& other)
	{
		WeakPtr(other).swap(*this);

		return *this;
	}


	SharedPtr<T> lock() noexcept
	{
		return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
	}

	const bool expired() const noexcept
	{
		if (_counter != nullptr)
		{
			if (_counter->Shared_useCount() == 0 && _counter->Weak_useCount() == 1)
			{
				delete _counter;

				WeakPtr<T>* w_ptr = const_cast<WeakPtr<T>*>(this);
				w_ptr->_ptr = nullptr;
				w_ptr->_counter = nullptr;

				return true;
			}
			else
			{
				return _counter->Shared_useCount() == 0;
			}
		}
		else
		{
			return true;
		}
	}

	void swap(WeakPtr<T>& rhs) noexcept
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
void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept
{
	lhs.swap(rhs);
}