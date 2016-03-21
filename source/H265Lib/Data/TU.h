#pragma once

#include <Common/Bitset.h>
#include <Data/BlockBase.h>

namespace HEVC
{
	class TU : public BlockBase
	{
	private:

	/*	std::shared_ptr<SequenceParameterSet> _sps;

		Int itsQPDeltaFromCU;

		int _initQP;

		int transformDepth;

		bool itsCBFs[3];

		std::vector<std::shared_ptr<TB> > itsTBs;

		CU* itsParentCU;*/

	public:

		TU(int x, int y, int size);

		virtual ~TU() = default;

		virtual void print(LogId logId, bool recursive = true) override;

		/*std::shared_ptr<TB> getTB(ImgComp comp) const
		{
			return itsTBs[comp];
		}*/
	};
}