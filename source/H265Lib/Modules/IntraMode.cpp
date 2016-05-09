/**
 * @file	IntraMode.cpp
 *
 * @brief	Implementacja klas strategii predykcji intra
 */

#include <Modules/IntraMode.h>
#include <Common/Calc.h>

namespace HEVC
{
	IntraMode::IntraMode()
	{
	}

	IntraMode::~IntraMode()
	{
	}

	PlanarMode::PlanarMode() :
		IntraMode()
	{
	}

	PlanarMode::~PlanarMode()
	{
	}

	Matrix<Sample> PlanarMode::calcPred(ReferenceSamples samples, ImgComp img_comp, int mode_idx)
	{
		size_t size = samples.block_size;
		int log2PuSize = Calc::log2(size);

		Sample **pred = initPred();
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				Int offset = (itsReferenceValues[INTRA_DIR_LEFT][y] + itsReferenceValues[INTRA_DIR_TOP][x] + 1) << log2PuSize;
				Int sumLeft = (x + 1) * (itsReferenceValues[INTRA_DIR_TOP][size] - itsReferenceValues[INTRA_DIR_LEFT][y]);
				Int sumTop = (y + 1) * (itsReferenceValues[INTRA_DIR_LEFT][size] - itsReferenceValues[INTRA_DIR_TOP][x]);
				pred[x][y] = (offset + sumLeft + sumTop) >> (log2PuSize + 1);
				/*Int x1 = ( itsReferenceValues[ INTRA_DIR_LEFT ][ y ] )*( size - 1 - x );
				Int x2 = ( itsReferenceValues[ INTRA_DIR_TOP ][ x ] )*( size - 1 - y );
				Int sumLeft = ( x + 1 ) * ( itsReferenceValues[ INTRA_DIR_TOP ][ size ] );
				Int sumTop = ( y + 1 ) * ( itsReferenceValues[ INTRA_DIR_LEFT ][ size ] );
				pred[ x ][ y ] = ( x1 + x2 + size + sumLeft + sumTop ) >> ( log2PuSize + 1 );*/
			}
		}
		return pred;
	}

	DcMode::DcMode()
	{
	}

	DcMode::~DcMode()
	{
	}

	void DcMode::calcDcVal()
	{
		int size = itsCurrentPB->getSize();
		itsDCVal = size;
		for (int x = 0; x < size; ++x)
			itsDCVal += itsReferenceValues[INTRA_DIR_LEFT][x] + itsReferenceValues[INTRA_DIR_TOP][x];
		itsDCVal >>= log2Int(size) + 1;
	}

	Sample DcMode::getFiltCorner() const
	{
		if (itsFiltreEdges)
			return (itsReferenceValues[INTRA_DIR_LEFT][0] + 2 * itsDCVal + itsReferenceValues[INTRA_DIR_TOP][0] + 2) >> 2;
		else
			return itsDCVal;
	}

	Sample DcMode::getFiltEdge(const IntraDirection dir, const int offset) const
	{
		assert(dir != INTRA_DIR_CORNER);
		assert(itsReferenceValues != nullptr && itsReferenceValues[INTRA_DIR_LEFT] != nullptr && itsReferenceValues[INTRA_DIR_TOP] != nullptr);

		const Sample* refs = itsReferenceValues[dir];

		if (itsFiltreEdges)
			return (refs[offset] + 3 * itsDCVal + 2) >> 2;
		else
			return itsDCVal;
	}

	Sample** DcMode::calcPred()
	{
		assert(itsCurrentPB != nullptr);
		assert(itsReferenceValues != nullptr && itsReferenceValues[INTRA_DIR_LEFT] != nullptr && itsReferenceValues[INTRA_DIR_TOP] != nullptr);

		calcDcVal();

		itsFiltreEdges = (itsCurrentPB->getImgComp() == LUMA) && (itsCurrentPB->getSize() < 32);

		Sample** pred = initPred();
		pred[0][0] = getFiltCorner();
		for (Int x = 1; x < itsCurrentPB->getSize(); x++)
		{
			pred[0][x] = getFiltEdge(INTRA_DIR_LEFT, x);
			pred[x][0] = getFiltEdge(INTRA_DIR_TOP, x);
		}

		for (Int x = 1; x < itsCurrentPB->getSize(); x++)
		{
			for (Int y = 1; y < itsCurrentPB->getSize(); y++)
				pred[x][y] = itsDCVal;
		}

		return pred;
	}

	LinearMode::LinearMode()
	{
	}

	LinearMode::~LinearMode()
	{
	}

	Sample LinearMode::getFiltEdge(const IntraDirection dir, const int offset)
	{
		const Sample* mainRefs = itsReferenceValues[dir];
		const Sample* sideRefs = itsReferenceValues[(dir + 1) % 2];
		int ref = mainRefs[0] + ((sideRefs[offset] - itsCornerReference) >> 1);
		return SeqParams()->clip(itsCurrentPB->getImgComp(), ref);
	}

	Sample** LinearMode::calcPred()
	{
		assert(itsCurrentPB != nullptr);
		assert(itsReferenceValues != nullptr && itsReferenceValues[INTRA_DIR_LEFT] != nullptr && itsReferenceValues[INTRA_DIR_TOP] != nullptr);

		IntraDirection dir = itsCurrentPB->getModeIdx() == 10 ? INTRA_DIR_LEFT : INTRA_DIR_TOP;
		bool filtEdge = (itsCurrentPB->getImgComp() == LUMA) && (itsCurrentPB->getSize() < 32);

		Sample **pred = initPred();

		pred[0][0] = itsReferenceValues[dir][0];
		if (filtEdge)
			pred[0][0] = getFiltEdge(dir, 0);

		for (Int x = 1; x < itsCurrentPB->getSize(); x++)
			pred[x][0] = dir == INTRA_DIR_TOP ? itsReferenceValues[INTRA_DIR_TOP][x] : filtEdge ? getFiltEdge(INTRA_DIR_LEFT, x) : itsReferenceValues[INTRA_DIR_LEFT][0];

		for (Int y = 1; y < itsCurrentPB->getSize(); y++)
			pred[0][y] = dir == INTRA_DIR_LEFT ? itsReferenceValues[INTRA_DIR_LEFT][y] : filtEdge ? getFiltEdge(INTRA_DIR_TOP, y) : itsReferenceValues[INTRA_DIR_TOP][0];

		for (Int x = 1; x < itsCurrentPB->getSize(); x++)
			for (Int y = 1; y < itsCurrentPB->getSize(); y++)
				pred[x][y] = dir == INTRA_DIR_LEFT ? itsReferenceValues[INTRA_DIR_LEFT][y] : itsReferenceValues[INTRA_DIR_TOP][x];

		return pred;
	}

	const Int AngMode::angles[] = { 32, 26, 21, 17, 13, 9, 5, 2, 0, -2, -5, -9, -13, -17, -21, -26, -32, -26, -21, -17, -13, -9, -5,
	-2, 0, 2, 5, 9, 13, 17, 21, 26, 32 };

	const Int AngMode::invAngles[] = { -4096, -1638, -910, -630, -482, -390, -315, -256, -315, -390, -482, -630, -910, -1638, -4096 };

	AngMode::AngMode()
	{
		refsArray = nullptr;
	}

	AngMode::~AngMode()
	{
		if (refsArray != nullptr)
			delete[] refsArray;
	}

	int AngMode::getAngle() const
	{
		assert((itsCurrentPB->getModeIdx() != 0) && (itsCurrentPB->getModeIdx() != 1) && (itsCurrentPB->getModeIdx() != 10) && (itsCurrentPB->getModeIdx() != 26));
		return AngMode::angles[itsCurrentPB->getModeIdx() - 2];
	}

	int AngMode::getInvAngle() const
	{
		assert((itsCurrentPB->getModeIdx() > 10) || (itsCurrentPB->getModeIdx() < 26));
		return AngMode::invAngles[itsCurrentPB->getModeIdx() - 11];
	}

	void AngMode::getRefsArray()
	{
		assert(itsCurrentPB != nullptr);
		assert(itsReferenceValues != nullptr && itsReferenceValues[INTRA_DIR_LEFT] != nullptr && itsReferenceValues[INTRA_DIR_TOP] != nullptr);

		Int angle = getAngle();

		bool modeHor = itsCurrentPB->getModeIdx() < 18;

		Int size = (Int)itsCurrentPB->getSize();

		if (refsArray != nullptr)
			delete[] refsArray;
		refsArray = new Sample[3 * size + 1];
		memset(refsArray, 0, (3 * size + 1) * sizeof(Sample));

		/*//LOG( "PRED" ) << "mode = " << itsCurrentPB->getModeIdx( );
		//LOG( "PRED" ) << std::endl;
		//LOG( "PRED" ) << "references: " << std::endl << itsCornerReference << " ";
		for( Int i = 0; i < 2 * size; ++i )
		{
			//LOG( "PRED" ) << itsReferenceValues[ INTRA_DIR_TOP ][ i ] << " ";
		}
		//LOG( "PRED" ) << std::endl;
		for( Int i = 0; i < 2 * size; ++i )
		{
			//LOG( "PRED" ) << itsReferenceValues[ INTRA_DIR_LEFT ][ i ] << std::endl;
		}*/

		int start = size;
		refsArray[start++] = itsCornerReference;
		for (Int x = start; x <= 2 * size; x++)
		{
			refsArray[x] = modeHor ? itsReferenceValues[INTRA_DIR_LEFT][x - start] : itsReferenceValues[INTRA_DIR_TOP][x - start];

			////LOG( "PRED" ) << "refsArray[ " << x << " ] <- itsReferenceValues[ " << x - start << " ] = " << refsArray[ x ] << std::endl;
		}

		if (angle < 0)
		{
			Int invAngle = getInvAngle();
			Int limit = (angle * size) >> 5;
			if (limit < -1)
			{
				for (Int x = -1; x >= limit; x--)
				{
					Int refIdx = ((x*invAngle + 128) >> 8) - 1;
					refsArray[size + x] = modeHor ? itsReferenceValues[INTRA_DIR_TOP][refIdx] : itsReferenceValues[INTRA_DIR_LEFT][refIdx];

					////LOG( "PRED" ) << "refsArray[ " << size + x << " ] <- itsReferenceValues[ " << refIdx << " ] = " << refsArray[ size + x ] << std::endl;
				}
			}
		}
		else
		{
			for (Int x = 2 * size + 1; x <= 3 * size; ++x)
			{
				refsArray[x] = modeHor ? itsReferenceValues[INTRA_DIR_LEFT][x - 1 - size] : itsReferenceValues[INTRA_DIR_TOP][x - 1 - size];
				////LOG( "PRED" ) << "refsArray[ " << x << " ] <- itsReferenceValues[ " << x - 1 - size << " ] = " << refsArray[ x ] << std::endl;
			}
		}

		/*//LOG( "PRED" ) << "refs" << std::endl;
		for( Int j = 0; j < 3; ++j )
		{
			for( Int i = j*size; i < ( j + 1 ) * size + j / 2; ++i )
				//LOG( "PRED" ) << std::setw( 4 ) << i << " ";
			//LOG( "PRED" ) << std::endl;
			for( Int i = j*size; i < ( j + 1 ) * size + j / 2; ++i )
				//LOG( "PRED" ) << std::setw( 4 ) << refsArray[ i ] << " ";
			//LOG( "PRED" ) << std::endl;
		}*/
	}

	Sample **AngMode::calcPred()
	{
		assert(itsCurrentPB != nullptr);
		assert(itsReferenceValues != nullptr && itsReferenceValues[INTRA_DIR_LEFT] != nullptr && itsReferenceValues[INTRA_DIR_TOP] != nullptr);

		getRefsArray();

		Int angle = getAngle();

		bool modeHor = itsCurrentPB->getModeIdx() < 18;

		Sample **pred = initPred();
		Int size = itsCurrentPB->getSize();

		Int angleSum = 0, iFact, iIdx, refIdx;
		for (Int x = 0; x < itsCurrentPB->getSize(); x++)
		{
			angleSum += angle;
			iFact = angleSum & 31;
			iIdx = angleSum >> 5;
			for (Int y = 0; y < itsCurrentPB->getSize(); y++)
			{
				refIdx = iIdx + y + 1;
				/*//LOG( "PRED" ) << "x = " << x << ", y = " << y << std::endl;
				//LOG( "PRED" ) << "refIdx = " << refIdx << std::endl;
				//LOG( "PRED" ) << "size+refIdx = " << size + refIdx << std::endl;
				//LOG( "PRED" ) << "refs[ size+refIdx ] = " << refsArray[ size + refIdx ] << std::endl;
				//LOG( "PRED" ) << "refs[ size + refIdx + 1 ] = " << refsArray[ size + refIdx + 1 ] << std::endl;*/
				if (iFact != 0)
				{
					Int comp0 = (32 - iFact) * refsArray[size + refIdx];
					Int comp1 = iFact * refsArray[size + refIdx + 1];
					pred[x][y] = (comp0 + comp1 + 16) >> 5;
				}
				else
				{
					pred[x][y] = refsArray[size + refIdx];
				}
			}
		}

		if (!modeHor)
			for (Int y = 0; y < itsCurrentPB->getSize(); y++)
				for (Int x = y; x < itsCurrentPB->getSize(); x++)
				{
					Int swap = pred[x][y];
					pred[x][y] = pred[y][x];
					pred[y][x] = swap;
				}

		delete[] refsArray;
		refsArray = nullptr;

		return pred;
	}
}