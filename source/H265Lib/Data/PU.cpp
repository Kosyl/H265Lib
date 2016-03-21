#include "PU.h"

namespace HEVC
{
	PU::PU(int X, int Y, int size) :
		BlockBase(X, Y, size)
	{
	}

	PU::~PU()
	{
	}

	void PU::print(LogId logId, bool recursive)
	{
		////LOG( "PART" ) << "Pintra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", lumaPredMode: " << itsLumaModeIdx << std::endl;

		////LOG( "PART" ) << "zawarte TU:" << std::endl;
		//LOG_TAB "PART" );
		//itsTransformArea->printDescription();
		//LOG_UNTAB "PART" );
	}

	//int Pintra::getModeForChroma(int modeForLuma, int chromaPredictionDerivationMode)
	//{
	//	switch (chromaPredictionDerivationMode)
	//	{
	//	case 0:
	//		return modeForLuma == 0 ? 34 : 0;
	//	case 1:
	//		return modeForLuma == 26 ? 34 : 26;
	//	case 2:
	//		return modeForLuma == 10 ? 34 : 10;
	//	case 3:
	//		return modeForLuma == 1 ? 34 : 1;
	//	case 4:
	//		return modeForLuma;
	//	default:
	//		assert(false);
	//		return 0;
	//	}
	//}

	//void Pintra::reconstructionLoop()
	//{
	//	////LOG( "RECO" ) << "Pintra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", lumaPredMode: " << itsLumaModeIdx << std::endl;
	//	//LOG_TAB "RECO" );
	//	itsTransformArea->reconstructionLoop(shared_from_this());
	//	//LOG_UNTAB "RECO" );
	//}
}