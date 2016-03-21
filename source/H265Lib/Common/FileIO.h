#pragma once

#include <algorithm>
#include "TypeDef.h"
#include "Common\Matrix.h"
#include <iostream>
#include <fstream>
#include <memory>

namespace HEVC
{
	namespace FileIO
	{
		bool goToMarker(std::ifstream& str, std::string key);

		template <typename T>
		bool readValue(std::ifstream& str, std::string key, T& out_value);

		template <typename T>
		std::unique_ptr<Matrix<T>> readMatrix(std::ifstream& str, std::string key);
	}
}