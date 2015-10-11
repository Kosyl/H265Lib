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
	}
}