#pragma once

#define CHECK assert(Invariant());

#include <cassert>
#include <iostream>


template<class T>
class Vector
{

private:

	T* _data;
	size_t _size;
	size_t _capacity;


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

		//using iterator = VectorItt<T>;
		//using const_iterator = VectorItt<const T>;
#pragma endregion


#pragma region Constructors and assignment
		~VectorItt() = default;

		VectorItt()
		{
			_ptr = nullptr;
		};

		VectorItt(const VectorItt& other)
		{
			_ptr = other._ptr;
		};

		VectorItt(T* p)
		{
			_ptr = static_cast<T*>(p);
		};

		VectorItt& operator=(const VectorItt& other) = default;

		/*const_iterator(iterator&)
		{
			this = const_iterator(const_cast<T*>(_data));
		};*/

		//const_iterator& operator=(iterator&)
		//{
		//	//this = const_cast<const_iterator>(iterator(this));
		//};
#pragma endregion


#pragma region Non-members
		T& operator*()
		{
			return *_ptr;
		};

		T* operator->()
		{
			return _ptr;
		};

		T& operator[](size_t i)
		{
			return _ptr[i];
		};

		VectorItt& operator++()
		{
			++_ptr;
			return *this;
		}

		VectorItt& operator--()
		{
			--_ptr;
			return *this;
		}

		VectorItt operator++(int)
		{
			auto temp = *this;
			operator++();
			return temp;
		}

		VectorItt operator--(int)
		{
			auto temp = *this;
			operator--();
			return temp;
		}

		VectorItt operator+(difference_type i) const
		{
			return _ptr + i;
		}

		VectorItt operator-(difference_type i) const
		{
			return _ptr - i;
		}

		difference_type operator-(const VectorItt<X>& other) const
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
	using iterator = VectorItt<T>;
	using const_iterator = VectorItt<const T>;
	using reverse_iterator = std::reverse_iterator<VectorItt<T>>;
	using const_reverse_iterator = std::reverse_iterator<VectorItt<const T>>;
	using size_type = std::size_t;
#pragma endregion


#pragma region Constructors and assignment
	~Vector()
	{
		delete[] _data;
	};

	Vector() noexcept
	{
		_size = 0;
		_capacity = 4;
		_data = new T[_capacity];

		CHECK;
	};

	Vector(const Vector& other)
	{
		_capacity = other._capacity;
		_size = other._size;
		_data = new T[_capacity];

		for (size_t i = 0; i < other._size; i++)
		{
			_data[i] = other._data[i];
		}

		CHECK;
	};

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

	Vector(const char* other)
	{
		size_t otherSize = std::strlen(other);

		_size = otherSize;
		_capacity = otherSize * 2;
		_data = new T[_capacity];

		for (size_t i = 0; i < otherSize; i++)
		{
			_data[i] = other[i];
		}

		CHECK;
	};

	Vector& operator=(const Vector& other)
	{
		if (*this == other)
			return *this;

		reserve(other._size);
		_size = other._size;

		for (size_t i = 0; i < _size; i++)
		{
			_data[i] = other._data[i];
		}

		CHECK;

		return *this;
	};

	Vector& operator=(Vector&& other) noexcept
	{
		delete[] _data;

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
	T& operator[](size_t i)
	{
		return _data[i];
	};

	const T& operator[](size_t i) const
	{
		return _data[i];
	};

	T& at(size_t i)
	{
		if (i >= _size)
			throw std::out_of_range("");
		else
			return _data[i];
	};

	const T& at(size_t i) const
	{
		if (i >= _size)
			throw std::out_of_range("");
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

	void reserve(size_t n)
	{
		if (n > _capacity)
		{
			T* newData = new T[n];
			for (size_t i = 0; i < _size; i++)
			{
				newData[i] = _data[i];
			}

			delete[] _data;
			_data = newData;
			_capacity = n;
		}

		CHECK;
	};

	void shrink_to_fit()
	{
		if (_capacity > _size)
		{
			T* newData = new T[_size];
			for (size_t i = 0; i < _size; i++)
			{
				newData[i] = _data[i];
			}

			delete[] _data;
			_capacity = _size;
			_data = newData;
		}

		CHECK;
	};

	void push_back(T c)
	{
		if (_size == _capacity)
			reserve(_capacity * 2);

		_data[_size++] = c;

		CHECK;
	};

	void resize(size_t n)
	{
		if (n > _capacity)
			reserve(n);

		if (n > _size)
		{
			for (size_t i = _size; i < n; i++)
			{
				_data[i] = T();
			}
		}
		_size = n;

		CHECK;
	};

	void swap(Vector& other)
	{
		Vector temp(other);
		other = *this;
		*this = temp;

		CHECK;
	}
#pragma endregion


#pragma region Non-members
	friend bool operator==(const Vector& lhs, const Vector& rhs)
	{
		auto lhs_it = lhs.begin();
		auto rhs_it = rhs.begin();

		for (; lhs_it != lhs.end() && rhs_it != rhs.end(); ++lhs_it, ++rhs_it)
		{
			if (*lhs_it != *rhs_it)
				return false;
		}
		return lhs_it == lhs.end() && rhs_it == rhs.end();
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
		return (_data != nullptr) && (_size <= _capacity);
	}

};
