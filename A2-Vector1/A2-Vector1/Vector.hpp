#pragma once

#include <cassert>
#define CHECK assert(Invariant());

#include <iostream>


template<class T>
class Vector
{

private:

	T* _data;


	template<class X>
	class VectorItt
	{
		//friend class Vector<T>;
		T* _ptr;

	public:

#pragma region typedefs
		typedef T									value_type;
		typedef std::ptrdiff_t						difference_type;
		typedef T*									pointer;
		typedef T&									reference;
		typedef std::random_access_iterator_tag		iterator_category;
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
			_ptr = p;
		};

		VectorItt& operator=(const VectorItt& other)
		{
			_ptr = other._ptr;
			return *this;
		};

		//const_iterator(iterator&)
		//{
		//	this = const_iterator(const_cast<T*>(_data));
		//};

		//const_iterator& operator=(iterator&)
		//{
		//	this = const_iterator(const_cast<T*>(_data));
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

		difference_type operator-(const VectorItt& other) const
		{
			return _ptr - other._ptr;
		}


		friend bool operator==(const VectorItt& lhs, const VectorItt& rhs)
		{
			return lhs._ptr == rhs._ptr;
		}

		friend bool operator!=(const VectorItt& lhs, const VectorItt& rhs)
		{
			return lhs._ptr != rhs._ptr;
		};

		friend bool operator<(const VectorItt& lhs, const VectorItt& rhs)
		{
			return lhs._ptr < rhs._ptr;
		};

		friend bool operator>(const VectorItt& lhs, const VectorItt& rhs)
		{
			return lhs._ptr > rhs._ptr;
		};

		friend bool operator<=(const VectorItt& lhs, const VectorItt& rhs)
		{
			return !(lhs._ptr > rhs._ptr);
		};

		friend bool operator>=(const VectorItt& lhs, const VectorItt& rhs)
		{
			return !(lhs._ptr < rhs._ptr);
		};
#pragma endregion

	};


public:

#pragma region typedef
	typedef VectorItt<T>			iterator;
	typedef VectorItt<const T>		const_iterator;
	typedef VectorItt<T>			reverse_iterator;
	typedef VectorItt<const T>		const_reverse_iterator;
	typedef std::size_t				size_type;
#pragma endregion


#pragma region Constructors and assignment
	~Vector()
	{
		delete[] _data;
	};

	Vector() noexcept
	{
		_data = new T(8);
	};

	Vector(const Vector& other)
	{
		_data = new T(other.capacity());

		for (size_t i = 0; i < other.size(); i++)
		{
			_data[i] = other._data[i];
		}
	};

	Vector(Vector&& other) noexcept
	{
		_data = other._data;
		other._data = nullptr;
	};

	Vector(const char* other)
	{
		size_t otherSize = sizeof(other) / sizeof(char);
		_data = new T(otherSize);

		for (size_t i = 0; other[i] != '\0'; i++)
		{
			_data[i] = other[i];
		}
	};

	Vector& operator=(const Vector& other)
	{

	};

	Vector& operator=(Vector&& other) noexcept
	{

	};
#pragma endregion


#pragma region Element access
	T& operator[](size_t i)
	{
		return _data[i];
	};

	T& at(size_t i)
	{
		if (i >= size())
			throw std::out_of_range("");
		else
			return _data[i];
	};

	const T& operator[](size_t i) const
	{
		return _data[i];
	};

	const T& at(size_t i) const
	{
		if (i >= size())
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
	const_iterator begin() const noexcept { return iterator(_data); }
	const_iterator cbegin() const noexcept { return iterator(_data); }

	iterator end() noexcept { return iterator(_data + size()); }
	const_iterator end() const noexcept { return iterator(_data + size()); }
	const_iterator cend() const noexcept { return iterator(_data + size()); }

	reverse_iterator rbegin() noexcept { return end(); }
	const_reverse_iterator rbegin() const noexcept { return end(); }
	const_reverse_iterator rcbegin() const noexcept { return cend(); }

	reverse_iterator rend() noexcept { return begin(); }
	const_reverse_iterator rend() const noexcept { return begin(); }
	const_reverse_iterator rcend() const noexcept { return cbegin(); }
#pragma endregion


#pragma region Capacity & Modifiers
	size_t size() noexcept
	{
		return end() - begin();
	};

	void reserve(size_t n)
	{
		if (n >= capacity())
		{
			T* newData = new T[n];
			for (size_t i = 0; i < size(); i++)
			{
				newData[i] = _data[i];
			}

			delete[] _data;
			_data = newData;
		}
	};

	size_t capacity()
	{
		//return sizeof(_data) / sizeof(T);
	};

	void shrink_to_fit()
	{
		if (capacity() > size())
		{
			T* newData = new T[size()];
			for (size_t i = 0; i < size(); i++)
			{
				newData[i] = _data[i];
			}

			delete[] _data;
			_data = newData;
		}
	};

	void push_back(T c)
	{
		if (size() == capacity())
			reserve(capacity() * 2);

		_data[size()] = c;
	};

	void resize(size_t n)
	{
		if (n > capacity())
			reserve(n);

		if (n > size())
		{
			for (size_t i = size(); i < n; i++)
			{
				_data[i] = T();
			}
		}
	};
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
		return !(lhs < rhs) && (lhs != rhs);
	};

	friend bool operator<=(const Vector& lhs, const Vector& rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	};

	friend bool operator>=(const Vector& lhs, const Vector& rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	};

	friend std::ostream& operator<<(std::ostream& cout, const Vector& other)
	{
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	}

	friend void swap(Vector& lhs, Vector& rhs)
	{
		Vector temp = lhs;
		lhs = rhs;
		rhs = temp;
	}
#pragma endregion


	bool Invariant() const
	{
		return true;
	}

};