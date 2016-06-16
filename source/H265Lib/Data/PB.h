#pragma once

#include "BlockBase.h"
#include <Common/Enums.h>
#include <Common/Matrix.h>


namespace HEVC
{
	struct ParametersBundle;

	struct PB : public BlockBase
	{
		ImgComp img_comp;

		MatrixRef<Sample> intra_source_samples;
		Matrix<Sample> prediction;

		int intra_mode;

		PB(ImgComp comp, int x, int y, int size, ParametersBundle parameters);
		virtual ~PB();

		virtual void print(LogId logId, bool recursive = true) override;
	};

}
