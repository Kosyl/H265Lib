#include "PB.h"

namespace HEVC
{
	PB::PB(ImgComp comp, int x, int y, int size, ParametersBundle parameters) :
		BlockBase( x, y, size)
		/*itsComp(comp),
		itsParentTB(parentTB),
		itsCornerReference(0),
		itsReferencesReady(false),
		itsPredictionTarget(nullptr),
		itsPredictionDone(false)*/
	{
	}

	PB::~PB()
	{
	}

	void PB::print(LogId logId, bool recursive)
	{
		////LOG( "OPT" ) << "PBIntra[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", comp: " << itsComp << std::endl;
	}
}