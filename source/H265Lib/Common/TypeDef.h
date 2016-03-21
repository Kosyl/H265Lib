#pragma once

#include <string>

#define interface struct __declspec(novtable)
#define clearptr(x) if(x!=nullptr) delete x; x = nullptr
using Pixel = uint8_t;
using Sample = uint16_t;
using Coeff = int32_t;

using LogId = std::string;