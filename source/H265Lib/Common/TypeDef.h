#pragma once

#include <string>

#define interface struct __declspec(novtable)

using Pixel = uint8_t;
using Sample = uint16_t;
using Coeff = int32_t;

using LogId = std::string;

template <class T>
using owner = T;