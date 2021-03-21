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
		if (_counter) _counter->Increment_weak();
	}

	WeakPtr(const SharedPtr<T>& other) : _ptr(other._ptr), _counter(other._counter)
	{
		if (_counter) _counter->Increment_weak();
	}

	~WeakPtr()
	{
		if (_counter) _counter->Decrement_weak();
	}


	WeakPtr<T>& operator=(const SharedPtr<T>& other)
	{
		_ptr = other._ptr;
		_counter = other._counter;
		if (_counter) _counter->Increment_weak();

		return *this;
	}


	SharedPtr<T> lock() noexcept
	{
		return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
	}

	bool expired() noexcept
	{
		if (_counter != nullptr)
		{
			if (_counter->Shared_useCount() == 0)
			{
				delete _counter;
				_ptr = nullptr;
				_counter = nullptr;

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}

		//return (_counter) ? _counter->Shared_useCount() == 0 : true;
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