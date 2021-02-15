#pragma once


template<class T>
struct RAC {
	using type = T;
};

template<class T>
struct RAC<const T> {
	typedef typename RAC<T>::type type;
};

template<class T>
struct RAC<T*> {
	typedef typename RAC<T>::type* type;
};

template<class T>
struct RAC<T&> {
	typedef typename RAC<T>::type& type;
};