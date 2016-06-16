#include "PU.h"
#include <cassert>
#include <Common/Logger.h>

namespace HEVC
{
	PU::PU(int X, int Y, int size) :
		BlockBase(X, Y, size)
	{
	}

	PU::~PU()
	{
	}

	void PU::setIntraPredictionMode(const short lumaMode, const short chroma_derivation_type)
	{
		assert(lumaMode >= 0 && lumaMode <= 34);
		intra_luma_prediction_mode = lumaMode;
		intra_chroma_mode_derivation_type = chroma_derivation_type;

		switch (intra_chroma_mode_derivation_type)
		{
		case 0:
			intra_chroma_prediction_mode = intra_luma_prediction_mode == 0 ? 34 : 0;
			break;
		case 1:
			intra_chroma_prediction_mode = intra_luma_prediction_mode == 26 ? 34 : 26;
			break;
		case 2:
			intra_chroma_prediction_mode = intra_luma_prediction_mode == 10 ? 34 : 10;
			break;
		case 3:
			intra_chroma_prediction_mode = intra_luma_prediction_mode == 1 ? 34 : 1;
			break;
		case 4:
			intra_chroma_prediction_mode = intra_luma_prediction_mode;
			break;
		default:
			assert(false);
		}
	}

	short PU::getIntraModeIdx(ImgComp comp) const
	{
		if (comp == Luma)
			return intra_luma_prediction_mode;
		else
			return intra_chroma_prediction_mode;
	}

	void PU::print(LogId logId, bool recursive)
	{

	}
}