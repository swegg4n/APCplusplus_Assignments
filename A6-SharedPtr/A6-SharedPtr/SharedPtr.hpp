#pragma once

#include "WeakPtr.hpp"
#include <algorithm>


template <class T>
class SharedPtr
{

private:

	T* _ptr = nullptr;
	size_t* _useCount = nullptr;



	void add_use(T* other)
	{
		if (other != nullptr)
		{
			if (_useCount != nullptr)
			{
				++(*_useCount);
			}
			else
			{
				_useCount = new size_t(1);
			}
		}
	}

	void remove_use()
	{
		if (_useCount != nullptr)
		{
			if (-- * _useCount <= 0)
			{
				delete _ptr;
				delete _useCount;
			}
			_useCount = nullptr;
		}
		_ptr = nullptr;
	}


public:

	SharedPtr() : _ptr(nullptr), _useCount(nullptr) { }

	SharedPtr(T* other)
	{
		_ptr = other;
		_useCount = nullptr;
		add_use(other);
	}

	SharedPtr(const std::nullptr_t)
	{
		remove_use();
	}

	SharedPtr(SharedPtr& other)
	{
		if (this != &other)
		{
			_ptr = other._ptr;
			_useCount = other._useCount;
			add_use(_ptr);
		}
	}

	SharedPtr(SharedPtr&& other) noexcept
	{
		if (this != &other)
		{
			_ptr = other._ptr;
			_useCount = other._useCount;

			add_use(_ptr);
			other.remove_use();
		}
	}

	~SharedPtr()
	{
		remove_use();
	}


	SharedPtr& operator=(const SharedPtr& other)
	{
		if (this != &other)
		{
			remove_use();

			_ptr = other._ptr;
			_useCount = other._useCount;

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
			_useCount = other._useCount;

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


	void reset()
	{
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
		return *_useCount;
	}


	void swap(SharedPtr<T>& rhs) noexcept
	{
		std::swap(_ptr, rhs._ptr);
		std::swap(_useCount, rhs._useCount);
	}


	bool Invariant()
	{
		return (_ptr == nullptr && _useCount == 0)
			|| (_ptr != nullptr && _useCount > 0);
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
