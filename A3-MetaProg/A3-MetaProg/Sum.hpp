#pragma once


template <class T>
auto Sum(T c) {

	using c_type = decltype(*c.begin());
	using c_type_noref = typename std::remove_reference<c_type>::type;

	c_type_noref sum{};

	for (auto i : c)
		sum += i; 

	return sum;
}