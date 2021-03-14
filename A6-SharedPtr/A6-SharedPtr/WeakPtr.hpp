#pragma once

#include "SharedPtr.hpp"


template <class T>
class WeakPtr
{

private:




public:

	WeakPtr()
	{

	}

	WeakPtr(const WeakPtr& other)
	{

	}

	WeakPtr(const SharedPtr& other)
	{

	}

	~WeakPtr()
	{

	}


	WeakPtr& operator=(const SharedPtr& other)
	{

	}



	void lock()
	{

	}

	bool expired()
	{

	}


	bool Invariant()
	{

	}

};


template<class T>
void swap(WeakPtr<T>& lhs, WeakPtr<T>& rhs) noexcept
{

}