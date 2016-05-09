#pragma once

#include "Picture.h"
#include "BlockBase.h"
#include <Common/Matrix.h>
#include <Data/PU.h>

namespace HEVC
{
	class TUQuadTree;
	class PU;

	class CU : public BlockBase
	{
		Partition partitioning_mode;

	public:

		Matrix3DRef<Sample> input_samples;
		Matrix3DRef<Sample> reconstructed_samples;

		std::shared_ptr<TUQuadTree> transform_tree;
		std::vector<std::shared_ptr<PU>> prediction_units;
		PredictionType prediction_type;

		int intra_mpms[3];
		int chroma_prediction_derivation_type;

		short cu_qp_delta;
		bool transquant_bypass_on;

		CU(int x, int y, int size);
		virtual ~CU();

		void setPartitionMode(Partition type);
		double getTotalCost();

		void print(LogId logId, bool recursive = true) override;
	};
}