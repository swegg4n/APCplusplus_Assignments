#pragma once

#define CHECK assert(Invariant());

#include <cassert>
#include <iostream>
#include "Dalloc.hpp"

template<class T>
class Vector
{

private:

	T* _data;
	size_t _size;
	size_t _capacity;
	Dalloc<T> _dAlloc = Dalloc<T>();


	template<class X>
	class VectorItt
	{
		T* _ptr;

	public:

#pragma region typedefs
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
		using iterator_category = std::random_access_iterator_tag;
#pragma endregion


#pragma region Constructors and assignment
		~VectorItt() = default;

		VectorItt() noexcept
		{
			_ptr = nullptr;
		};

		VectorItt<X>(const VectorItt<T>& other)	noexcept
		{
			_ptr = other._ptr;
		}

		VectorItt(T* p)	noexcept
		{
			_ptr = static_cast<T*>(p);
		};

		VectorItt<X>& operator=(const VectorItt<T>& other)
		{
			_ptr = other._ptr;
			return *this;
		};
#pragma endregion


#pragma region Non-members
		X& operator*() const noexcept
		{
			return *_ptr;
		};

		X* operator->() const noexcept
		{
			return _ptr;
		};

		X& operator[](size_t i) const noexcept
		{
			return _ptr[i];
		};

		VectorItt& operator++() noexcept
		{
			++_ptr;
			return *this;
		}

		VectorItt& operator--() noexcept
		{
			--_ptr;
			return *this;
		}

		VectorItt operator++(int) noexcept
		{
			auto temp = *this;
			operator++();
			return temp;
		}

		VectorItt operator--(int) noexcept
		{
			auto temp = *this;
			operator--();
			return temp;
		}

		VectorItt operator+(difference_type i) const noexcept
		{
			return _ptr + i;
		}

		VectorItt operator-(difference_type i) const noexcept
		{
			return _ptr - i;
		}

		VectorItt operator+=(difference_type i) noexcept
		{
			_ptr = _ptr + i;
			return _ptr;
		}

		VectorItt operator-=(difference_type i) noexcept
		{
			_ptr = _ptr - i;
			return _ptr;
		}

		difference_type operator-(const VectorItt<X>& other) const noexcept
		{
			return _ptr - other._ptr;
		}

		friend bool operator==(const VectorItt<X>& lhs, const VectorItt<X>& rhs)
		{
			return lhs._ptr == rhs._ptr;
		}

		friend bool operator!=(const VectorItt<X>& lhs, const VectorItt<X>& rhs)
		{
			return lhs._ptr != rhs._ptr;
		};

		friend bool operator<(const VectorItt<X>& lhs, const VectorItt<X>& rhs)
		{
			return lhs._ptr < rhs._ptr;
		};

		friend bool operator>(const VectorItt<X>& lhs, const VectorItt<X>& rhs)
		{
			return lhs._ptr > rhs._ptr;
		};

		friend bool operator<=(const VectorItt<X>& lhs, const VectorItt<X>& rhs)
		{
			return !(lhs._ptr > rhs._ptr);
		};

		friend bool operator>=(const VectorItt<X>& lhs, const VectorItt<X>& rhs)
		{
			return !(lhs._ptr < rhs._ptr);
		};
#pragma endregion
	};


public:

#pragma region typedef
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = VectorItt<T>;
	using const_iterator = VectorItt<const T>;
	using reverse_iterator = std::reverse_iterator<VectorItt<T>>;
	using const_reverse_iterator = std::reverse_iterator<VectorItt<const T>>;
#pragma endregion


#pragma region Constructors and assignment
	~Vector() noexcept
	{
		_dAlloc.deallocate(_data, _capacity);
	};

	Vector() noexcept
	{
		_size = 0;
		_capacity = 0;

		CHECK;
	};

	template<class Titer>
	Vector(size_t newCapacity, Titer begin, Titer end)
	{
		_size = 0;

		try
		{
			_capacity = newCapacity;
			_data = _dAlloc.allocate(_capacity);

			for (auto it = begin; it != end; ++it)
			{
				new(_data + _size) T(*it);
				++_size;
			}
		}
		catch (const std::exception&)
		{
			for (; _size > 0; _size--)
			{
				_data[_size - 1].~T();
			}
			_dAlloc.deallocate(_data, _capacity);
			_capacity = 0;

			throw std::exception("Failed to construct vector");
		}

		CHECK;
	};

	Vector(const Vector& other) : Vector(other._size, other.begin(), other.end()) {};

	Vector(Vector&& other) noexcept
	{
		_size = other._size;
		_capacity = other._capacity;
		_data = other._data;

		other._size = 0;
		other._capacity = 0;
		other._data = nullptr;

		CHECK;
	};

	/// <summary>
	/// Contructor for testing. Generates a vector with one character on each address
	/// </summary>
	Vector(const char* other) : Vector(std::strlen(other), other, other + std::strlen(other)) {};


#pragma region Assignment - Test
	/// <summary>
	/// Simple implementation of the assign function.
	/// Pros: Simple to implement
	/// Cons: Extra reallocation (inefficient & slow)
	/// </summary>
	Vector& AssSimple(const Vector& other)
	{
		if (this == &other)
			return *this;

		Vector copy(other);
		swap(copy);

		CHECK;

		return *this;
	};

	/// <summary>
	/// Fast implementation of the assign function.
	/// Pros: Assignment is performed efficiently and fast
	/// Cons: Upon error, data is NOT reset, meaning a vector could end up being half copied
	///		  Upon error, we dont know what the vector contains
	/// </summary>
	Vector& AssFast(const Vector& other)
	{
		if (this == &other)
			return *this;

		reserve(other._size);	//Only reserves more space if needed!

		if (other._size <= _size)
		{
			std::copy(other.begin(), other.begin() + other._size, _data);

			for (size_t i = other._size; i < _size; i++)
			{
				_data[i].~T();
			}
		}
		else
		{
			std::copy(other.begin(), other.end(), _data);
		}

		_size = other._size;

		CHECK;

		return *this;
	};

	/// <summary>
	/// Strong implementation of the assign function.
	/// Pros: Upon error, data is reset, leaving the vector in the same state it was before this call
	/// Cons: Extra allocation, slower to run
	/// </summary>
	Vector& AssStrong(const Vector& other)
	{
		if (this == &other)
			return *this;

		Vector original(other);

		try
		{
			reserve(other._size);	//Only reserves more space if needed!
			_size = other._size;

			for (size_t i = 0; i < _size; i++)
			{
				_data[i] = other._data[i];
			}
		}
		catch (const std::exception&)
		{
			_dAlloc.deallocate(_data, _capacity);
			_data = original._data;
			_size = original._size;
			_capacity = original._capacity;
		}

		CHECK;

		return *this;
	};
#pragma endregion


	Vector& operator=(const Vector& other)
	{
		//return AssSimple(other);
		return AssFast(other);
		//return AssStrong(other);
	};

	Vector& operator=(Vector&& other) noexcept
	{
		_dAlloc.deallocate(_data, _capacity);

		_size = other._size;
		_capacity = other._capacity;
		_data = other._data;

		other._size = 0;
		other._capacity = 0;
		other._data = nullptr;

		CHECK;

		return *this;
	};
#pragma endregion


#pragma region Element access
	T& operator[](size_t i) noexcept
	{
		return _data[i];
	};

	const T& operator[](size_t i) const	noexcept
	{
		return _data[i];
	};

	T& at(size_t i)
	{
		if (i < 0 || i >= _size)
			throw std::out_of_range("Index out of range");
		else
			return _data[i];
	};

	const T& at(size_t i) const
	{
		if (i < 0 || i >= _size)
			throw std::out_of_range("Index out of range");
		else
			return _data[i];
	};

	T* data() noexcept
	{
		return _data;
	};

	const T* data() const noexcept
	{
		return _data;
	};
#pragma endregion


#pragma region Iterators
	iterator begin() noexcept { return iterator(_data); }
	const_iterator begin() const noexcept { return const_iterator(_data); }
	const_iterator cbegin() const noexcept { return const_iterator(_data); }

	iterator end() noexcept { return iterator(_data + _size); }
	const_iterator end() const noexcept { return const_iterator(_data + _size); }
	const_iterator cend() const noexcept { return const_iterator(_data + _size); }

	reverse_iterator rbegin() noexcept { return reverse_iterator(_data + _size); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_data + _size); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_data + _size); }

	reverse_iterator rend() noexcept { return reverse_iterator(_data); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(_data); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_data); }
#pragma endregion


#pragma region Capacity & Modifiers
	size_t size() const noexcept
	{
		return _size;
	};

	size_t capacity() const noexcept
	{
		return _capacity;
	};

	/// <summary>
	/// Reallocates memory (if necessary) to fit <paramref name="n"/> elements in the vector
	/// </summary>
	void reserve(size_t n)
	{
		if (n > _capacity)
		{
			if (_data == nullptr)
			{
				_data = _dAlloc.allocate(n);
				_size = 0;
				_capacity = n;
			}
			else
			{
				T* newData = _dAlloc.allocate(n);
				size_t i = 0;

				try
				{
					for (; i < _size; i++)
					{
						new(newData + i) T(_data[i]);
					}
					_dAlloc.deallocate(_data, _capacity);
					_data = newData;
					_capacity = n;
				}
				catch (const std::exception&)
				{
					for (; i > 0; i--)
					{
						newData[i].~T();
					}
					_dAlloc.deallocate(_data, n);
					_size = 0;
					_capacity = n;

					throw std::exception("reserve - FAIL");
				}
			}
		}

		CHECK;
	};

	/// <summary>
	/// Reallocates memory (if necessary) so only "size"-number of elements fit the vector
	/// </summary>
	void shrink_to_fit()
	{
		if (_capacity > _size)
		{
			T* newData = _dAlloc.allocate(_size);
			size_t oldCap = _capacity;
			size_t i = 0;

			try
			{
				for (; i < _size; i++)
				{
					new(newData + i) T(_data[i]);
				}
				_dAlloc.deallocate(_data, _capacity);
				_capacity = _size;
				_data = newData;
			}
			catch (const std::exception&)
			{
				for (; i > 0; i++)
				{
					newData[i].~T();
				}
				_dAlloc.deallocate(newData, _size);
				_capacity = oldCap;

				throw std::exception("shrink_to_fit - FAIL");
			}
		}

		CHECK;
	};

	/// <summary>
	/// Places the value <paramref name="c"/> at the end of the vector
	/// </summary>
	void push_back(const T& c)
	{
		emplace_back(c);
	};

	/// <summary>
	/// Moves the value <paramref name="c"/> to the end of the vector
	/// </summary>
	void push_back(T&& c)
	{
		emplace_back(std::move(c));
	}

	/// <summary>
	/// Constructs a new element at the end of the vector
	/// </summary>
	template<class... Args>
	T& emplace_back(Args&&... args)
	{
		if (_capacity == 0)
			reserve(1);
		else if (_size == _capacity)
			reserve(_capacity * 2);

		try
		{
			new(_data + _size) T(std::forward<Args>(args)...);
			++_size;
		}
		catch (const std::exception&)
		{
			throw std::exception("emplace_back - FAIL");
		}

		CHECK;

		return *(_data + _size - 1);
	}


	/// <summary>
	/// Reallocates memory (if necessary) to fit <paramref name="n"/> elements in the list. Fills unused adresses with new T()
	/// </summary>
	void resize(size_t n)
	{
		reserve(n);		//Only reserves more space if necessary

		if (n > _size)
		{
			for (size_t i = _size; i < n; i++)
			{
				new (_data + i) T();
			}
		}
		_size = n;

		CHECK;
	};

	/// <summary>
	/// Swaps this vector with <paramref name="other"/>
	/// </summary>
	void swap(Vector& other)
	{
		std::swap(_data, other._data);
		std::swap(_size, other._size);
		std::swap(_capacity, other._capacity);

		CHECK;
	}
#pragma endregion


#pragma region Non-members
	friend bool operator==(const Vector& lhs, const Vector& rhs)
	{
		if (lhs.size() != rhs.size())
			return false;

		auto lhs_it = lhs.begin();
		auto rhs_it = rhs.begin();

		for (; lhs_it != lhs.end() && rhs_it != rhs.end(); ++lhs_it, ++rhs_it)
		{
			if (*lhs_it != *rhs_it)
				return false;
		}
		return true;
	};

	friend bool operator!=(const Vector& lhs, const Vector& rhs)
	{
		return !(lhs == rhs);
	};

	friend bool operator<(const Vector& lhs, const Vector& rhs)
	{
		auto lhs_it = lhs.begin();
		auto rhs_it = rhs.begin();

		for (; lhs_it != lhs.end() && rhs_it != rhs.end(); ++lhs_it, ++rhs_it)
		{
			if (*lhs_it != *rhs_it)
				return *lhs_it < *rhs_it;
		}
		return lhs_it == lhs.end() && rhs_it != rhs.end();
	};

	friend bool operator>(const Vector& lhs, const Vector& rhs)
	{
		return (rhs < lhs);
	};

	friend bool operator<=(const Vector& lhs, const Vector& rhs)
	{
		return !(lhs > rhs);
	};

	friend bool operator>=(const Vector& lhs, const Vector& rhs)
	{
		return !(lhs < rhs);
	};

	friend std::ostream& operator<<(std::ostream& cout, const Vector& other)
	{
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	}

	friend void swap(Vector& lhs, Vector& rhs)
	{
		lhs.swap(rhs);
	}
#pragma endregion


	bool Invariant() const
	{
		if (_data == nullptr)
			return _capacity == 0;
		else
			return _size <= _capacity;
	}

};
