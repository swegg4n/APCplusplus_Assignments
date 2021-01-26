#pragma once

#include <cassert>
#define CHECK assert(Invariant());

#include <iostream>


template<class T>
class Vector
{
	T* _pointer;


	template<class X>
	class VectorItt
	{
		friend class Vector<T>;
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
		~VectorItt();

		VectorItt();

		VectorItt(const VectorItt& other);

		VectorItt(T* p);

		VectorItt& operator=(const VectorItt& other);

		const_iterator(iterator&);

		const_iterator& operator=(iterator&);
#pragma endregion


#pragma Non-members
		T& operator*();

		T* operator->();

		T& operator[](size_t i);


		VectorItt& operator++();

		VectorItt& operator--();

		VectorItt operator++(int);

		VectorItt operator--(int);

		VectorItt operator+(difference_type i) const;

		VectorItt operator-(difference_type i) const;

		difference_type operator-(const VectorItt& other) const;


		friend bool operator==(const VectorItt& lhs, const VectorItt& rhs);

		friend bool operator!=(const VectorItt& lhs, const VectorItt& rhs);

		friend bool operator<(const VectorItt& lhs, const VectorItt& rhs);

		friend bool operator>(const VectorItt& lhs, const VectorItt& rhs);

		friend bool operator<=(const VectorItt& lhs, const VectorItt& rhs);

		friend bool operator>=(const VectorItt& lhs, const VectorItt& rhs);
#pragma endregion
	};


public:

#pragma region typedef
	using iterator = VectorItt<T>;
	using const_iterator = VectorItt<const T>;
	using size_type = std::size_t;
#pragma endregion


#pragma region Constructors and assignment
	~Vector();

	Vector() noexcept;

	Vector(const Vector& other);

	Vector(Vector&& other) noexcept;

	Vector(const char* other);


	Vector& operator=(const Vector& other);

	Vector& operator=(Vector&& other) noexcept;
#pragma endregion


#pragma region Element access
	T& operator[](size_t i);

	T& at(size_t i);

	const& operator[](size_t i) const;

	const T& at(size_t i) const;


	T* data() noexcept;

	const T* data() const noexcept;
#pragma endregion


#pragma region Iterators
	iterator begin() noexcept;
	const iterator begin() const noexcept;
	const iterator cbegin() const noexcept;

	iterator end() noexcept;
	const iterator end() const noexcept;
	const iterator cend() const noexcept;

	reverse_iterator begin() noexcept;
	const reverse_iterator begin() const noexcept;
	const reverse_iterator cbegin() const noexcept;

	reverse_iterator end() noexcept;
	const reverse_iterator end() const noexcept;
	const reverse_iterator cend() const noexcept;
#pragma endregion


#pragma region Capacity
	size_t size() noexcept;

	void reserve(size_t n);

	size_t capacity();
#pragma endregion


#pragma Modifiers
	void shrink_to_fit();

	void push_back(T c);

	void resize(size_t n);

#pragma endregion


#pragma Non-members
	friend bool operator==(const Vector& lhs, const Vector& rhs);

	friend bool operator!=(const Vector& lhs, const Vector& rhs);

	friend bool operator<(const Vector& lhs, const Vector& rhs);

	friend bool operator>(const Vector& lhs, const Vector& rhs);

	friend bool operator<=(const Vector& lhs, const Vector& rhs);

	friend bool operator>=(const Vector& lhs, const Vector& rhs);


	friend std::ostream& operator<<(std::ostream& cout, const Vector& other)
	{
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	}

	void swap(Vector& lhs, Vector& rhs);
#pragma endregion


	bool Invariant() const
	{
		return true;
	}
};