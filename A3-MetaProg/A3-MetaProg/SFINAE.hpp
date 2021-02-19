#pragma once

#include <type_traits>



template <class T, std::enable_if_t<std::is_nothrow_copy_constructible<T>::value, bool> = true>
bool NoThrowCopyConstructible(T) {
	return true;
}

template <class T, std::enable_if_t<!std::is_nothrow_copy_constructible<T>::value, bool> = true>
bool NoThrowCopyConstructible(T) {
	return false;
}
