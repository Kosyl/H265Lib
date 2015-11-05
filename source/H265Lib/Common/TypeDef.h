#pragma once

#include <string>

#define interface struct __declspec(novtable)

using Void = void;
using Bool = bool;
using Char = char;
using UChar = unsigned char;
using Short = short;
using UShort = unsigned short;
using Int = int;
using Long = long;
using UInt = unsigned int;
using Double = double;
using Float = float;
using String = std::string;

using Pixel = UChar;
using Sample = Short;
using Coeff = Int;