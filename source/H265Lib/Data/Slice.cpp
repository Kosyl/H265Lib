#include "Slice.h"

namespace HEVC
{
	Slice::Slice(size_t beginCtuX, size_t beginCtuY, size_t lengthInCtus):
		begin_ctu_x(beginCtuX),
		begin_ctu_y(beginCtuY),
		length_in_ctus(lengthInCtus)
	{
	}

	Slice::~Slice()
	{
	}
}