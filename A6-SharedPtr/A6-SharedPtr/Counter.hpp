#pragma once


template <class T>
class Counter
{

private:

	size_t _shared_useCount;
	size_t _weak_useCount;


public:

	Counter() : _shared_useCount(1), _weak_useCount(1) {}

	~Counter() = default;


	size_t Increment_shared() noexcept
	{
		return ++_shared_useCount;
	}
	size_t Increment_weak() noexcept
	{
		return ++_weak_useCount;
	}

	size_t Decrement_shared() noexcept
	{
		return --_shared_useCount;
	}
	size_t Decrement_weak() noexcept
	{
		return --_weak_useCount;
	}


	size_t Shared_useCount()
	{
		return _shared_useCount;
	}

	size_t Weak_useCount()
	{
		return _weak_useCount;
	}

};