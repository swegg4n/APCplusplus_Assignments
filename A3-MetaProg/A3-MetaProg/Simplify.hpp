#pragma once


#pragma region SimplifyType

template<class T>
struct SimplifyType {
	using type = T;
};

template<class T>
using SimplifyType_t = typename SimplifyType<T>::type;


template<class T>
struct SimplifyType<T*> {
	using type = T;
};

template<class T>
struct SimplifyType<T&> {
	using type = T;
};

template<class T>
struct SimplifyType<T[]> {
	using type = T;
};

template<class T>
struct SimplifyType<const T> {
	using type = T;
};

template<class T>
struct SimplifyType<const T*> {
	using type = const T;
};

template<class T>
struct SimplifyType<const T&> {
	using type = const T;
};

template<class T>
struct SimplifyType<const T[]> {
	using type = const T;
};

#pragma endregion


#pragma region BaseType

template<class T>
struct BaseType {
	using type = T;
};

template<class T>
using BaseType_t = typename BaseType<T>::type;


template<class T>
struct BaseType<T*> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<T&> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<T[]> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<const T> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<const T*> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<const T&> {
	using type = BaseType_t<T>;
};

template<class T>
struct BaseType<const T[]> {
	using type = BaseType_t<T>;
};

#pragma endregion
