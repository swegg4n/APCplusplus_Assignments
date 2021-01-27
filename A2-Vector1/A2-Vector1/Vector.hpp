#pragma once

#include <cassert>
#define CHECK assert(Invariant());

#include <iostream>


template<class T>
class Vector
{

private:

	T* _pointer;


	template<class X>
	class VectorItt
	{
		T* _ptr;

	public:

#pragma region typedef
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = X&;
		using const_reference = const X&;
		using pointer = X*;
		using const_pointer = const X*;
		typedef const X* const_pointer;
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
		};

		const_iterator(iterator&);

		const_iterator& operator=(iterator&);
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
	using iterator = VectorItt<T>;
	using const_iterator = VectorItt<const T>;
	using reverse_iterator = VectorItt<T>;
	using const_reverse_iterator = VectorItt<const T>;
	using size_type = std::size_t;
#pragma endregion


#pragma region Constructors and assignment
	~Vector()
	{
		delete[] _pointer;
	};

	Vector() noexcept
	{
		_pointer = new T(8);
	};

	Vector(const Vector& other)
	{
		_pointer = new T(other.capacity());

		for (size_t i = 0; i < other.size(); i++)
		{
			_pointer[i] = other._pointer[i];
		}
	};

	Vector(Vector&& other) noexcept
	{
		_pointer = other._pointer;
		other._pointer = nullptr;
	};

	Vector(const char* other)
	{
		size_t otherSize = sizeof(other) / sizeof(char);
		_pointer = new T(otherSize);

		for (size_t i = 0; other[i] != '\0'; i++)
		{
			_pointer[i] = other[i];
		}
	};

	Vector& operator=(const Vector& other);

	Vector& operator=(Vector&& other) noexcept;
#pragma endregion


#pragma region Element access
	T& operator[](size_t i)
	{
		return _pointer[i];
	};

	T& at(size_t i)
	{
		if (i >= size())
			throw std::out_of_range("");
		else
			return _pointer[i];
	};

	const& operator[](size_t i) const
	{
		return _pointer[i];
	};

	const T& at(size_t i) const
	{
		if (i >= size())
			throw std::out_of_range("");
		else
			return _pointer[i];
	};

	T* data() noexcept
	{
		return _pointer;
	};

	const T* data() const noexcept
	{
		return _pointer;
	}:
#pragma endregion


#pragma region Iterators
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	reverse_iterator begin() noexcept;
	const_reverse_iterator begin() const noexcept;
	const_reverse_iterator cbegin() const noexcept;

	reverse_iterator end() noexcept;
	const_reverse_iterator end() const noexcept;
	const_reverse_iterator cend() const noexcept;
#pragma endregion


#pragma region Capacity
	size_t size() noexcept;

	void reserve(size_t n);

	size_t capacity();
#pragma endregion


#pragma region Modifiers
	void shrink_to_fit();

	void push_back(T c);

	void resize(size_t n);
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
		return lhs.size() < rhs.size();
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

	void swap(Vector& lhs, Vector& rhs)
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