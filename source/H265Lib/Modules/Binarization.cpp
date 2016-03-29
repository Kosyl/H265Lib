///**
// * @file	Binarization.cpp
// *
// * @brief	Implementacja kalkulatora binaryzacji.
// */
//
//#include "Binarization.h"
//#include "Utils.h"
//#include "CoeffGroup.h"
//#include "SeqPicParameters.h"
//#include <bitset>
//#include <fstream>
//#include <stdio.h>
//
//#pragma region Scanning Modes
//
//const int Binarization::MinInGroup[ 10 ] = { 0, 1, 2, 3, 4, 6, 8, 12, 16, 24 };
//const int Binarization::GroupIdx[ 32 ] = { 0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9 };
//Binarization* Binarization::instance = nullptr;
//const Int ScanningMode::CG_SIZE = 4;
//
//ScanningMode::ScanningMode( )
//{
//}
//
//ScanningMode::~ScanningMode( )
//{
//}
//
//HorizontalScanningMode::HorizontalScanningMode( )
//{
//}
//
//HorizontalScanningMode::~HorizontalScanningMode( )
//{
//}
//
//std::vector<std::shared_ptr<CoeffGroup> > HorizontalScanningMode::scan( Coeff** coeffs, int size, int& out_lastCoeffX, int& out_lastCoeffY )
//{
//	int sizeInCGs = size / ScanningMode::CG_SIZE;
//	int numCGs = sizeInCGs * sizeInCGs;
//
//	Short num = 0;
//	std::vector<std::shared_ptr<CoeffGroup> > res;
//	bool pastFirstNonZero = false, pastFirstNonZeroInCG = false;
//
//	for( Int cgY = sizeInCGs - 1; cgY >= 0; --cgY )
//	{
//		for( Int cgX = sizeInCGs - 1; cgX >= 0; --cgX )
//		{
//			Submatrix<Coeff> subset( coeffs, ScanningMode::CG_SIZE, cgX*ScanningMode::CG_SIZE, cgY*ScanningMode::CG_SIZE );
//			num = 0;
//			std::shared_ptr<CoeffGroup> cg = std::make_shared<CoeffGroup>( );
//
//			pastFirstNonZeroInCG = false;
//
//			for( Int y = ScanningMode::CG_SIZE - 1; y >= 0; --y )
//			{
//				for( Int x = ScanningMode::CG_SIZE - 1; x >= 0; --x )
//				{
//					( *cg )[ num ] = subset[ x ][ y ];
//
//					if( ( *cg )[ num ] != 0 )
//					{
//						if( !pastFirstNonZero )
//						{
//							pastFirstNonZero = true;
//							out_lastCoeffX = cgX*ScanningMode::CG_SIZE + x;
//							out_lastCoeffY = cgY*ScanningMode::CG_SIZE + y;
//						}
//						if( !pastFirstNonZeroInCG )
//						{
//							pastFirstNonZeroInCG = true;
//							cg->setFirstPosInScan( num );
//						}
//						cg->setLastPosInScan( num );
//					}
//
//					++num;
//				}
//			}
//
//			if( pastFirstNonZero || ( cgX == 0 && cgY == 0 ) )
//			{
//				res.push_back( cg );
//			}
//		}
//	}
//	return res;
//}
//
//VerticalScanningMode::VerticalScanningMode( )
//{
//}
//
//VerticalScanningMode::~VerticalScanningMode( )
//{
//}
//
//std::vector<std::shared_ptr<CoeffGroup> > VerticalScanningMode::scan( Coeff** coeffs, int size, int& out_lastCoeffX, int& out_lastCoeffY )
//{
//	int sizeInCGs = size / ScanningMode::CG_SIZE;
//	int numCGs = sizeInCGs * sizeInCGs;
//
//	Short num = 0;
//	std::vector<std::shared_ptr<CoeffGroup> > res;
//	bool pastFirstNonZero = false, pastFirstNonZeroInCG = false;;
//
//	for( Int cgX = sizeInCGs - 1; cgX >= 0; --cgX )
//	{
//		for( Int cgY = sizeInCGs - 1; cgY >= 0; --cgY )
//		{
//			Submatrix<Coeff> subset( coeffs, ScanningMode::CG_SIZE, cgX*ScanningMode::CG_SIZE, cgY*ScanningMode::CG_SIZE );
//			num = 0;
//			std::shared_ptr<CoeffGroup> cg = std::make_shared<CoeffGroup>( );
//
//			pastFirstNonZeroInCG = false;
//
//			for( Int x = ScanningMode::CG_SIZE - 1; x >= 0; --x )
//			{
//				for( Int y = ScanningMode::CG_SIZE - 1; y >= 0; --y )
//				{
//					( *cg )[ num ] = subset[ x ][ y ];
//
//					if( ( *cg )[ num ] != 0 )
//					{
//						if( !pastFirstNonZero )
//						{
//							pastFirstNonZero = true;
//							out_lastCoeffX = cgX*ScanningMode::CG_SIZE + x;
//							out_lastCoeffY = cgY*ScanningMode::CG_SIZE + y;
//						}
//						if( !pastFirstNonZeroInCG )
//						{
//							pastFirstNonZeroInCG = true;
//							cg->setFirstPosInScan( num );
//						}
//						cg->setLastPosInScan( num );
//					}
//					++num;
//				}
//			}
//
//			if( pastFirstNonZero || ( cgX == 0 && cgY == 0 ) )
//			{
//				res.push_back( cg );
//			}
//		}
//	}
//	return res;
//}
//
//DiagonalScanningMode::DiagonalScanningMode( )
//{
//}
//
//DiagonalScanningMode::~DiagonalScanningMode( )
//{
//}
//
//const int DiagonalScanningMode::coeffIdxInCG[ 2 ][ 16 ] =
//{
//	{ 3, 3, 2, 3, 2, 1, 3, 2, 1, 0, 2, 1, 0, 1, 0, 0 },
//	{ 3, 2, 3, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 0, 1, 0 }
//};
//
//std::vector<std::shared_ptr<CoeffGroup> > DiagonalScanningMode::scan( Coeff** coeffs, int size, int& out_lastCoeffX, int& out_lastCoeffY )
//{
//	int sizeInCGs = size / ScanningMode::CG_SIZE;
//	int numCGs = sizeInCGs * sizeInCGs;
//	int numCGInLine = 1;
//	Int diff = 1;
//	Int xySum = 2 * sizeInCGs - 2;
//
//	int x = 0, y = 0, cgX, cgY, checkedCGs = 0;
//	std::vector<std::shared_ptr<CoeffGroup> > res;
//
//	bool pastFirstNonZero = false;
//
//	do
//	{
//		for( int i = 0; i < numCGInLine; ++i )
//		{
//			cgX = sizeInCGs - 1 - i - ( ( checkedCGs > numCGs / 2 ) ? sizeInCGs - numCGInLine : 0 );
//			cgY = xySum - cgX;
//
//			Submatrix<Coeff> subset( coeffs, ScanningMode::CG_SIZE, cgX*ScanningMode::CG_SIZE, cgY*ScanningMode::CG_SIZE );
//
//			std::shared_ptr<CoeffGroup> cg = std::make_shared<CoeffGroup>( );
//
//			bool pastFirstNonZeroInCG = false;
//
//			for( Short i = 0; i < 16; ++i )
//			{
//				x = coeffIdxInCG[ 0 ][ i ];
//				y = coeffIdxInCG[ 1 ][ i ];
//
//				( *cg )[ i ] = subset[ x ][ y ];
//
//				if( ( *cg )[ i ] != 0 )
//				{
//					if( !pastFirstNonZero )
//					{
//						pastFirstNonZero = true;
//						out_lastCoeffX = cgX*ScanningMode::CG_SIZE + x;
//						out_lastCoeffY = cgY*ScanningMode::CG_SIZE + y;
//					}
//					if( !pastFirstNonZeroInCG )
//					{
//						pastFirstNonZeroInCG = true;
//						cg->setFirstPosInScan( i );
//					}
//					cg->setLastPosInScan( i );
//				}
//			}
//
//			++checkedCGs;
//			if( pastFirstNonZero || checkedCGs == numCGs )
//			{
//				res.push_back( cg );
//			}
//
//			if( checkedCGs >= numCGs / 2 )
//				diff = -1;
//		}
//
//		--xySum;
//		numCGInLine += diff;
//	}
//	while( numCGInLine > 0 );
//
//	return res;
//}
//
//#pragma endregion
//
//const Int Binarization::COEFF_REMAIN_THRESHOLD = 3; //przejœcie z GR na TU+EG(k)
//
//Binarization* Binarization::getInstance( )
//{
//	if( Binarization::instance == nullptr )
//		Binarization::instance = new Binarization( );
//	return Binarization::instance;
//}
//
//Binarization::Binarization( ) :
//itsCurrentCUUsesTransQuantBypass( false )
//{
//}
//
//Binarization::~Binarization( )
//{
//}
//
//
//int Binarization::binarizeCoefficientRemainingLevel_NumBits( int symbol, int &rParam )
//{
//	Int value = (Int)symbol;
//	Int threshold = 3 << rParam;
//
//	if( value < threshold )
//	{
//		return ( value >> rParam ) + 1 + rParam;
//	}
//	value -= threshold;
//
//	return ( ( ( numBits( value + ( 1 << rParam ) ) ) - 1 ) << 1 ) + 4 - rParam;
//}
//
//int Binarization::countBinsInTB( Coeff** coefficients, int size, ScanningMode* scanningMode )
//{
//	int totalBins = 0;
//	int log2TUSize = 2;
//
//	int lastCoeffX = 0, lastCoeffY = 0;
//
//	std::vector<std::shared_ptr<CoeffGroup> > CGs = scanningMode->scan( coefficients, size, lastCoeffX, lastCoeffY );
//
//	if( lastCoeffX == 0 && lastCoeffY == 0 && ( CGs[ 0 ] )->getLastPosInScan( ) == -1 )
//		return 0;
//
//	if( size == 4 && PicParams( )->getTransformSkipEnabled( ) && !itsCurrentCUUsesTransQuantBypass )
//	{
//		++totalBins;
//	}
//
//	Coeff* remains = new Coeff[ Constants::NUM_COEFFS_IN_CG ];
//
//	totalBins += binarizeLastSignificantXY_NumBits( lastCoeffX, size );
//	totalBins += binarizeLastSignificantXY_NumBits( lastCoeffY, size );
//
//	for( int i = 0; i < CGs.size( ); ++i ) //TUTAJ sa obliczenia dla kazdego bloku 4x4 (Coefficient Group, ja nazywam CG), chyba o to chodzilo Pastuszakowi
//	{
//		std::shared_ptr<CoeffGroup> CG = CGs[ i ];
//
//		if( !( i == 0 || i == ( CGs.size( ) - 1 ) ) )
//		{
//			++totalBins; //sigCGflag
//		}
//
//		int numNonZero = countNonZeroCoeffsAndSigns( CG, ( i + 1 ) == CGs.size( ) );
//
//		if( numNonZero == 0 && !( i == 0 || i == ( CGs.size( ) - 1 ) ) )
//			continue;
//
//		if( i == 0 )// zliczanie mapy znaczonoœci, bez LastCoeff
//		{
//			boost::dynamic_bitset<> tmp = CG->getSignificantCoeffFlags( );
//			tmp.resize( Constants::NUM_COEFFS_IN_CG - 1 - CG->getFirstPosInScan( ) );
//			CG->setSignificantCoeffFlags( tmp );
//			totalBins += CG->getSignificantCoeffFlags( ).size( );
//		}
//		else
//		{
//			totalBins += CG->getSignificantCoeffFlags( ).size( );
//		}
//
//		if( CG->getLastPosInScan( ) - CG->getFirstPosInScan( ) >= Constants::SDH_THRESHOLD && useSDH( ) && !itsCurrentCUUsesTransQuantBypass )
//		{
//			boost::dynamic_bitset<> tmp = reverseBitset( CG->getCoeffSigns( ) );
//			tmp.resize( tmp.size( ) - 1 );
//			CG->setCoeffSigns( reverseBitset( tmp ) );
//		}
//
//		int numSignBits = CG->getCoeffSigns( ).size( );
//		totalBins += numSignBits; // zliczanie bitów znaków
//
//		for( int i = 0; i < numNonZero; ++i ) // wiemy ze sa >=1, wiec kodowac bedziemy x-1
//		{
//			remains[ i ] = ( *CG )[ i ] - 1;
//		}
//
//		int C1andC2bins = calcC1andC2( CG, remains, numNonZero );
//
//		totalBins += C1andC2bins; // biny flag C1 i C2
//
//		int riceParam = 0;
//
//		for( int i = 0; i < numNonZero; ++i )
//		{
//			if( remains[ i ] >= 0 )
//			{
//				int size = binarizeCoefficientRemainingLevel_NumBits( remains[ i ], riceParam );
//				totalBins += size;
//				if( ( *CG )[ i ] > 3 * ( 1 << riceParam ) )
//				{
//					riceParam = std::min<int>( riceParam + 1, 4 );
//				}
//			}
//		}
//	}
//	delete[] remains;
//
//	return totalBins;
//}
//
//int Binarization::countNonZeroCoeffsAndSigns( std::shared_ptr<CoeffGroup> cg, bool isDCGroup )
//{
//	int count = 0;
//
//	boost::dynamic_bitset<> sigCoeffs, signs;
//
//	int numCoeffs = Constants::CG_SIZE * Constants::CG_SIZE;
//	Int overwriteIdx = 0;
//
//	for( int i = 0; i < numCoeffs; ++i )
//	{
//		Coeff coeff = ( *cg )[ i ];
//		if( coeff != 0 )
//		{
//			signs.push_back( coeff > 0 ? 0 : 1 );
//			sigCoeffs.push_back( 1 );
//			++count;
//
//			( *cg )[ overwriteIdx ] = std::abs( coeff );
//
//			++overwriteIdx;
//		}
//		else
//		{
//			sigCoeffs.push_back( 0 );
//		}
//	}
//	cg->setSignificantCoeffFlags( reverseBitset( sigCoeffs ) );
//	cg->setCoeffSigns( reverseBitset( signs ) );
//	if( ( cg->getFirstPosInScan( ) == cg->getLastPosInScan( ) ) && ( cg->getFirstPosInScan( ) == 15 ) && ( cg->getSignificantCoeffFlags( ).size( ) == 16 ) && !isDCGroup )
//	{
//		boost::dynamic_bitset<> tmp = ( cg->getSignificantCoeffFlags( ) >> 1 );
//		tmp.resize( 15 );
//		cg->setSignificantCoeffFlags( tmp );
//	}
//	return count;
//}
//
//int Binarization::binarizeTR_NumBits( int val, int maxLenBeforeSuffix, int riceParam )
//{
//	int len = 0;
//	int prefix = val >> riceParam;
//	int threshold = maxLenBeforeSuffix >> riceParam;
//
//	if( prefix < threshold )
//	{
//		len = prefix + 1;
//	}
//	else
//	{
//		len = threshold;
//	}
//
//	if( maxLenBeforeSuffix > val )
//	{
//		len += riceParam;
//	}
//	return len;
//}
//
//int Binarization::binarizeEGk_NumBits( Int val, int k )
//{
//	int len = 0;
//	Int absloute = abs( val );
//	bool stopLoop = false;
//	do
//	{
//		if( absloute >= ( 1 << k ) )
//		{
//			++len;
//			absloute = absloute - ( 1 << k );
//			k++;
//		}
//		else
//		{
//			++len;
//			while( k-- )
//				++len;
//			stopLoop = true;
//		}
//	}
//	while( !stopLoop );
//
//	return len;
//}
//
//boost::dynamic_bitset<> Binarization::binarizeLastSignificantXYPrefix(int prefix, int transformSize)
//{
//	return binarizeTR(prefix, (log2Int(transformSize) << 1) - 1, 0);
//}
//
//boost::dynamic_bitset<> Binarization::binarizeLastSignificantXYSuffix(int suffix, int prefix)
//{
//	return binarizeFL(suffix, 1 << ((prefix >> 1) - 2));
//}
//
//boost::dynamic_bitset<> Binarization::binarizeFL(int val, int maxVal)
//{
//	boost::dynamic_bitset<> res;
//	int len = numBits(maxVal);
//	for (Int i = len - 1; i >= 0; i--)
//	{
//		res.push_back((val >> i) & 1);
//	}
//	return res;
//}
//
//boost::dynamic_bitset<> Binarization::binarizeTR(int val, int maxLenBeforeSuffix, int riceParam)
//{
//	boost::dynamic_bitset<> res;
//
//	int prefix = val >> riceParam;
//	int threshold = maxLenBeforeSuffix >> riceParam;
//
//	if (prefix < threshold)
//	{
//		for (int i = 0; i < prefix; ++i)
//		{
//			res.push_back(1);
//		}
//		res.push_back(0);
//	}
//	else
//	{
//		for (int i = 0; i < threshold; ++i)
//		{
//			res.push_back(1);
//		}
//	}
//
//	if (maxLenBeforeSuffix > val)
//	{
//		int suffix = val % (1 << riceParam);
//
//		for (int i = 0; i < riceParam; ++i)
//		{
//			res.push_back((suffix >> i) & 1);
//		}
//	}
//	return res;
//}
//
//int Binarization::calcC1andC2( std::shared_ptr<CoeffGroup> cg, Coeff* out_remains, int numNonZero )
//{
//	int result = 0;
//	int C1count = 0;
//	int C2count = 0;
//	boost::dynamic_bitset<> C1;
//
//	for( int i = 0; i < numNonZero; ++i )
//	{
//		if( ( *cg )[ i ] > 0 )
//		{
//			++C1count;
//			C1.push_back( ( *cg )[ i ] > 1 );
//
//			out_remains[ i ] = out_remains[ i ] - 1;
//			if( C2count == 0 && ( *cg )[ i ] > 1 )
//			{
//				C2count = 1;
//				cg->setC2( ( *cg )[ i ] > 2 );
//
//				out_remains[ i ] = out_remains[ i ] - 1;
//			}
//		}
//		if( C1count == 8 ) break;
//	}
//
//	cg->setC1Group( reverseBitset( C1 ) );
//
//	return C1count + C2count;
//}
//
//int Binarization::binarizeLastSignificantXY_NumBits( int position, int transformSize )
//{
//	int pref = 0, suf = 0;
//	binarizeLastSignificantXY_GetPrefixAndSuffix( position, transformSize, pref, suf );
//	return binarizeTR_NumBits( pref, ( log2Int( transformSize ) << 1 ) - 1, 0 ) + ( pref > 3 ? numBits( 1 << ( ( pref >> 1 ) - 2 ) ) : 0 );
//}
//
//void Binarization::binarizeLastSignificantXY_GetPrefixAndSuffix( int position, int transformSize, int& out_prefix, int& out_suffix )
//{
//	out_prefix = GroupIdx[ position ];
//
//	if( out_prefix > 3 )
//	{
//		out_suffix = position - MinInGroup[ out_prefix ];
//	}
//}
//
//bool Binarization::useSDH( )
//{
//	return PicParams( )->getSDHEnabled( );
//}