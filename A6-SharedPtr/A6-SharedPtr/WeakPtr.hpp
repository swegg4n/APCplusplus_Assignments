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

	WeakPtr(const WeakPtr<T>& other) : _ptr(other._ptr), _counter(other._counter) {}

	WeakPtr(const SharedPtr<T>& other) : _ptr(other._ptr), _counter(other._counter) {}

	~WeakPtr() = default;


	WeakPtr<T>& operator=(const SharedPtr<T>& other)
	{
		_ptr = other._ptr;	//(0)
		_counter = other._counter;

		//WeakPtr copy(other);	//(1)
		//this->swap(copy);

		return *this;
	}


	SharedPtr<T> lock() noexcept
	{
		return expired() ? nullptr : SharedPtr<T>(*this);
	}

	bool expired() const noexcept
	{
		if (_counter != nullptr)
		{
			return _counter->Shared_useCount() == 0;
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
		return true;
	}

};


template<class T>
void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept
{
	lhs.swap(rhs);
}