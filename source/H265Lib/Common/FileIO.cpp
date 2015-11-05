#include "Common/FileIO.h"
#include <string>

namespace H265Lib
{
	namespace FileIO
	{
		Bool goToMarker(std::ifstream& str, std::string key)
		{
			std::string s, log, value;
			Bool end = false;
			do
			{
				str >> s;
				if (str.eof() || str.fail())
					return false;
				if (s == "$")
				{
					str >> s;
					if (str.eof() || str.fail())
						return false;
					if (s == key)
					{
						end = true;
						break;
					}
				}
			} while (!str.eof() && !str.fail());
			if (!end)
				return false;

			return true;
		}

		template <typename T>
		Bool readValue(std::ifstream& str, std::string key, T& out_value)
		{
			try
			{
				if (!goToMarker(str, key) || str.eof() || str.fail())
					return false;
				str >> out_value;
				if (str.eof() || str.fail())
					return false;
			}
			catch (...)
			{
				return false;
			}
			return true;
		}

		template <typename T>
		std::unique_ptr<Matrix<T>> readMatrix(std::ifstream& str, std::string key)
		{
			std::unique_ptr<Matrix<T>> result;

			try
			{
				size_t width, height;

				if (!FileIO::goToMarker(str, key) || str.eof() || str.fail())
					return false;
				if (!FileIO::goToMarker(str, "WIDTH") || str.eof() || str.fail())
					return false;
				str >> width;
				if (str.eof() || str.fail())
					return false;

				if (!FileIO::goToMarker(str, "HEIGHT") || str.eof() || str.fail())
					return false;
				str >> height;
				if (str.eof() || str.fail())
					return false;
				if (!FileIO::goToMarker(str, "MATRIX") || str.eof() || str.fail())
					return false;

				result = std::make_unique<Matrix<T>>(height, width);

				T tmp;
				for (UInt i = 0; i < height; ++i)
				{
					for (UInt j = 0; j < width; ++j)
					{
						str >> tmp;
						if (str.eof() || str.fail())
							return false;
						result->at(j, i) = tmp;
					}
				}
			}
			catch (...)
			{
				return nullptr;
			}
			return result;
		}


		template Bool readValue(std::ifstream& str, std::string key, unsigned int& out_value);
		template std::unique_ptr<Matrix<int>> readMatrix<int>(std::ifstream& str, std::string key);
	}
}