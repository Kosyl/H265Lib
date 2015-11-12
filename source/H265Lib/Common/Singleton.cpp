#include "Singleton.h"

template<typename T>
std::unique_ptr<T> Singleton<T>::_instance = nullptr;