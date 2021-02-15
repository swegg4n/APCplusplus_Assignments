#pragma once


template<class T, std::enable_if<std::is_nothrow_copy_constructible<T>::value> = true>
bool NoThrowCopyConstructible(T t)
{ 
	return true;
}

template<class T, std::enable_if<std::is_nothrow_copy_constructible<T>::value> = false>
bool NoThrowCopyConstructible(T t)
{
	return false;
}