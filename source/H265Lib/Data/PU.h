#pragma once

#include <cassert>
#include "CU.h"
#include <vector>
#include <Common/Enums.h>

namespace HEVC
{
	class Pintra;
	class TB;

	class PU : public BlockBase
	{
	private:

	public:

		PU(int X, int Y, int size);
		~PU();
		
		short intra_luma_prediction_mode;
		short intra_chroma_prediction_mode;
		short intra_chroma_mode_derivation_type;

		void setIntraPredictionMode(const short lumaMode, const short chroma_derivation_type);

		short getIntraModeIdx(ImgComp comp) const;

		virtual void print(LogId logId, bool recursive = true) override;
	};
}
