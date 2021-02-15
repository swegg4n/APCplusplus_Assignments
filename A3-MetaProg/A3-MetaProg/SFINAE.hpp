#pragma once



template<class T, class = std::enable_if_t<std::is_nothrow_copy_constructible<T>::value>>
bool NoThrowCopyConstructible(T t)
{
	return true;
}

template<class T, class = std::enable_if_t<!std::is_nothrow_copy_constructible<T>::value>>
bool NoThrowCopyConstructible(T t)
{
	return false;
}