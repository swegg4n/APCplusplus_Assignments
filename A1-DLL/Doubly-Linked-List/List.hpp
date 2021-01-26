#pragma once

#include <cassert>
#define CHECK assert(Invariant());

#include <iostream>


template<class T>
class List
{
	class Node;


	class Link
	{
		friend class List<T>;
		Link* _prev = &_head;
		Link* _next = &_head;

		Link() : _prev(this), _next(this) {}


#pragma region Helper functions
		Node* next() { return static_cast<Node*>(_next); }
		Node* prev() { return static_cast<Node*>(_prev); }

		/// <summary>
		/// Places <paramref name="newNode"/> in the list (before this link)
		/// </summary>
		void insert(Node* newNode)
		{
			newNode->_next = this;
			newNode->_prev = _prev;
			_prev->_next = newNode;
			this->_prev = newNode;
		};

		void erase()
		{
			_prev->_next = _next;
			_next->_prev = _prev;
			delete static_cast<Node*>(this);
		};
#pragma endregion

	};


	class Node : public Link
	{
		friend class List;
		T _data;

	public:
		Node(const T& data) : _data(data) {}
	};


	template<class X>
	class ListIter
	{
		friend class List<T>;
		Link* _ptr;

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
		~ListIter() = default;

		ListIter() { _ptr = nullptr; }

		ListIter(const ListIter& other) { _ptr = other._ptr; }

		ListIter(Link* p) : _ptr(static_cast<Node*>(p)) {}

		ListIter& operator=(const ListIter& other)
		{
			_ptr = other._ptr;
			return *this;
		};
#pragma endregion


#pragma region Non-members
		T& operator*()
		{
			return static_cast<Node*>(_ptr)->_data;
		};

		T* operator->()
		{
			return &static_cast<Node*>(_ptr)->_data;
		};

		// ++it
		ListIter& operator++()
		{
			_ptr = _ptr->_next;
			return *this;
		};

		// --it
		ListIter& operator--()
		{
			_ptr = _ptr->_prev;
			return *this;
		};

		// it++
		ListIter operator++(int)
		{
			auto temp = *this;
			operator++();
			return temp;
		};

		// it++
		ListIter operator--(int)
		{
			auto temp = *this;
			operator--();
			return temp;
		};

		friend bool operator==(const ListIter& lhs, const ListIter& rhs)
		{
			return lhs._ptr == rhs._ptr;
		};

		friend bool operator!=(const ListIter& lhs, const ListIter& rhs)
		{
			return lhs._ptr != rhs._ptr;
		};
#pragma endregion

	};

	Link _head;


public:

#pragma region typedef
	using iterator = ListIter<T>;
	using const_iterator = ListIter<const T>;
	using size_type = std::size_t;
#pragma endregion


#pragma region Constructors and assignment

	~List()
	{
		while (_head._next != end())
		{
			pop_front();
		}

		CHECK;
	};

	List() = default;

	List(const List& other)
	{
		for (auto it = other.begin(); it != other.end(); ++it)
		{
			push_back(*it);
		}

		CHECK;
	};

	/// <summary>
	/// Constructor for testing. Generates a list with one character in each node
	/// </summary>
	List(const char* other)
	{
		const char* ptr = other;
		while (*ptr != '\0')
		{
			push_back(*ptr);
			++ptr;
		}

		CHECK;
	};

	void operator=(const List& other)
	{
		List copy(other);
		swap(copy);

		CHECK;
	}
#pragma endregion


#pragma region Element access
	T& front()
	{
		return *begin();
	};

	const T& front() const
	{
		return *cbegin();
	};

	T& back()
	{
		iterator it = end();
		--it;
		return *it;
	};

	const T& back() const
	{
		const_iterator it = cend();
		--it;
		return *it;
	};
#pragma endregion


#pragma region Iterators
	iterator begin() noexcept { return iterator(_head._next); }
	const_iterator begin() const noexcept { return const_iterator(const_cast<Link*>(_head._next)); }
	const_iterator cbegin() const noexcept { return const_iterator(const_cast<Link*>(_head._next)); }

	iterator end() noexcept { return iterator(&_head); }
	const_iterator end() const noexcept { return const_iterator(const_cast<Link*>(&_head)); }
	const_iterator cend() const noexcept { return const_iterator(const_cast<Link*>(&_head)); }
#pragma endregion


#pragma region Capacity
	bool empty() const noexcept { return begin() == end(); };

	/// <summary>
	/// Counts the number of elements in the list. O(n)
	/// </summary>
	size_t size() const noexcept
	{
		size_t count = 0;
		for (auto it = begin(); it != end(); ++it, ++count);
		return count;
	};
#pragma endregion


#pragma region Modifiers

	/// <summary>
	/// Creates a new node at <paramref name="pos"/>
	/// </summary>
	iterator insert(iterator pos, const T& value)
	{
		Node* newNode = new Node(value);
		pos._ptr->insert(newNode);

		CHECK;

		return iterator(newNode);
	};

	/// <summary>
	/// Removes the link at position <paramref name="pos"/>
	/// </summary>
	iterator erase(const iterator& pos)
	{
		Link* next_it = pos._ptr->_next;
		pos._ptr->erase();

		CHECK;

		return iterator(next_it);
	}

	/// <summary>
	/// Adds the value <paramref name="value"/> to the end of the list
	/// </summary>
	void push_back(const T& value)
	{
		insert(end(), value);

		CHECK;
	};

	/// <summary>
	/// Adds the value <paramref name="value"/> to the beginning of the list
	/// </summary>
	void push_front(const T& value)
	{
		insert(begin(), value);

		CHECK;
	};

	/// <summary>
	/// Removes the link at the end of the list
	/// </summary>
	void pop_back()
	{
		erase(end()._ptr->_prev);

		CHECK;
	};

	/// <summary>
	/// Removes the link at the beginning of the list
	/// </summary>
	void pop_front()
	{
		erase(begin()._ptr);

		CHECK;
	};

	/// <summary>
	/// Swaps this list with <paramref name="rhs"/>
	/// </summary>
	void swap(List& rhs)
	{
		bool thisEmpty = this->empty();
		bool rhsEmpty = rhs.empty();

		Link* this_to_next = _head._next;
		Link* this_to_prev = _head._prev;

		if (rhsEmpty == false)
		{
			_head._next = rhs._head._next;
			_head._prev = rhs._head._prev;
			_head._next->_prev = &_head;
			_head._prev->_next = &_head;
		}
		else
		{
			_head._next = &_head;
			_head._prev = &_head;
		}

		if (thisEmpty == false)
		{
			rhs._head._next = this_to_next;
			rhs._head._prev = this_to_prev;
			rhs._head._next->_prev = &rhs._head;
			rhs._head._prev->_next = &rhs._head;
		}
		else
		{
			rhs._head._next = &rhs._head;
			rhs._head._prev = &rhs._head;
		}

		CHECK;
	};

	/// <summary>
	/// Inserts all elements from <paramref name="other"/> (between <paramref name="first"/> and <paramref name="last"/>) at position <paramref name="pos"/>.  [<paramref name="first"/>, <paramref name="last"/>[
	/// </summary>
	void splice(const iterator pos, List& other, iterator first, iterator last)
	{
		const iterator insertPos = pos._ptr->_prev;
		iterator end = last._ptr->_prev;

		if (other.empty() == false && first != last)
		{
			Link* first_prev = first._ptr->_prev;

			insertPos._ptr->_next->_prev = end._ptr;
			end._ptr->_next = insertPos._ptr->_next;
			insertPos._ptr->_next = first._ptr;
			first._ptr->_prev = insertPos._ptr;

			first_prev->_next = last._ptr;
			last._ptr->_prev = first_prev;
		}

		CHECK;
	}
#pragma endregion


#pragma region Non-members
	friend bool operator==(const List& lhs, const List& rhs)
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

	friend bool operator!=(const List& lhs, const List& rhs)
	{
		return !(lhs == rhs);
	};

	friend bool operator<(const List& lhs, const List& rhs)
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

	friend bool operator>(const List& lhs, const List& rhs)
	{
		return !(lhs < rhs) && (lhs != rhs);
	};

	friend bool operator<=(const List& lhs, const List& rhs)
	{
		return (lhs < rhs) || (lhs == rhs);
	};

	friend bool operator>=(const List& lhs, const List& rhs)
	{
		return (lhs > rhs) || (lhs == rhs);
	};

	friend std::ostream& operator<<(std::ostream& cout, const List& other)
	{
		for (auto it = other.begin(); it != other.end(); ++it)
		{
			cout << *it << ", ";
		}
		return cout;
	};

	friend void swap(List<T>& lhs, List<T>& rhs)
	{
		lhs.swap(rhs);
	};
#pragma endregion


	/// <summary>
	/// Method for checking if the lists pointes point to the correct elements
	/// </summary>
	bool Invariant()
	{
		Link* temp(&_head);

		for (auto it = begin(); it != end(); ++it)
		{
			if (temp != temp->_next->_prev)
				return false;
			temp = temp->_next;
		}
		return true;
	};
};
