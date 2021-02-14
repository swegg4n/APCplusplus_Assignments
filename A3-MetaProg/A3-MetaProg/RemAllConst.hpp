#pragma once


template<class T>
struct RAC {
	using type = T;
};

template<class T>
struct RAC<T*> {
	using type = T*;
};

template<class T>
struct RAC<const T> {
	using type = T;
};