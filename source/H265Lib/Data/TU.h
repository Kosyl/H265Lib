#pragma once

#include <Common/Bitset.h>
#include <Data/BlockBase.h>

namespace H265Lib
{
	class TU : public BlockBase
	{
	private:

	/*	std::shared_ptr<SequenceParameterSet> _sps;

		Int itsQPDeltaFromCU;

		UInt _initQP;

		UInt transformDepth;

		Bool itsCBFs[3];

		std::vector<std::shared_ptr<TB> > itsTBs;

		CU* itsParentCU;*/

	public:

		TU(UInt x, UInt y, UInt size, ParametersBundle parameters);

		~TU()
		{
		}

		virtual Void printDescription(LogId logId, Bool recursive = true) override;

		/*std::shared_ptr<TB> getTB(ImgComp comp) const
		{
			return itsTBs[comp];
		}*/
	};
}