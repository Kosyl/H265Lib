#include "PU.h"

namespace H265Lib
{
	PUIntra::PUIntra(UInt X, UInt Y, UInt size, ParametersBundle parameters) :
		BlockBase(X, Y, size)
	{
	}

	PUIntra::~PUIntra()
	{
	}

	Void PUIntra::printDescription(LogId logId, bool recursive)
	{
		////LOG( "PART" ) << "PUIntra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", lumaPredMode: " << itsLumaModeIdx << std::endl;

		////LOG( "PART" ) << "zawarte TU:" << std::endl;
		//LOG_TAB "PART" );
		//itsTransformArea->printDescription();
		//LOG_UNTAB "PART" );
	}

	//UInt PUIntra::getModeForChroma(UInt modeForLuma, UInt chromaPredictionDerivationMode)
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

	//Void PUIntra::reconstructionLoop()
	//{
	//	////LOG( "RECO" ) << "PUIntra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", lumaPredMode: " << itsLumaModeIdx << std::endl;
	//	//LOG_TAB "RECO" );
	//	itsTransformArea->reconstructionLoop(shared_from_this());
	//	//LOG_UNTAB "RECO" );
	//}
}