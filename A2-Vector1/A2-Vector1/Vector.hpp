#pragma once

#define CHECK assert(Invariant());

#include <cassert>
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
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using pointer = X*;
		using iterator_category = std::random_access_iterator_tag;

		using iterator = VectorItt<T>;
		using const_iterator = VectorItt<const T>;
#pragma endregion


#pragma region Constructors and assignment
		~VectorItt() = default;

		VectorItt()
		{
			_ptr = nullptr;
		};

		VectorItt(const VectorItt<X>& other)
		{
			_ptr = other._ptr;
		};

		VectorItt(T* p)
		{
			_ptr = static_cast<T*>(p);
		};

		VectorItt<X>& operator=(const VectorItt<X>& other) = default;

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
		X& operator*()
		{
			return *_ptr;
		};

		X* operator->()
		{
			return _ptr;
		};

		X& operator[](size_t i)
		{
			return _ptr[i];
		};

		VectorItt<X>& operator++()
		{
			++_ptr;
			return *this;
		}

		VectorItt<X>& operator--()
		{
			--_ptr;
			return *this;
		}

		VectorItt<X> operator++(int)
		{
			auto temp = *this;
			operator++();
			return temp;
		}

		VectorItt<X> operator--(int)
		{
			auto temp = *this;
			operator--();
			return temp;
		}

		VectorItt<X> operator+(difference_type i) const
		{
			return _ptr + i;
		}

		VectorItt<X> operator-(difference_type i) const
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
		_data = new T[8];
	};

	Vector(const Vector& other)
	{
		_data = new T[other.capacity()];

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
		size_t otherSize = std::strlen(other);	//TODO: check so that the empty char isn't counted!
		_data = new T[otherSize];

		for (size_t i = 0; i < otherSize; i++)
		{
			_data[i] = other[i];
		}
	};

	Vector& operator=(const Vector& other)
	{
		Vector temp(other);
		swap(*this, temp);
		return *this;
	};

	Vector& operator=(Vector&& other) noexcept
	{
		_data = other._data;
		other._data = nullptr;
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
		if (i >= size())
			throw std::out_of_range("");
		else
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
	const_iterator begin() const noexcept { return const_iterator(_data); }
	const_iterator cbegin() const noexcept { return const_iterator(_data); }

	iterator end() noexcept { return iterator(_data + size()); }
	const_iterator end() const noexcept { return const_iterator(_data + size()); }
	const_iterator cend() const noexcept { return const_iterator(_data + size()); }

	reverse_iterator rbegin() noexcept { return reverse_iterator(_data + size()); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(_data + size()); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(_data + size()); }

	reverse_iterator rend() noexcept { return reverse_iterator(_data); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(_data); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(_data); }
#pragma endregion


#pragma region Capacity & Modifiers
	size_t size() const noexcept
	{
		return end() - begin();
	};

	size_t capacity() const noexcept
	{
		return 0;
		//return sizeof(_data) / sizeof(T);
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