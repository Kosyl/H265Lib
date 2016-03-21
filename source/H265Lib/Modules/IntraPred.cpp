///**
// * @file	IntraPred.cpp
// *
// * @brief	Implementacja klay predyktora intra
// */
//
//#include "IntraPred.h"
//
//IntraPred* IntraPred::itsInstance = nullptr;
//
//IntraPred::IntraPred( ) :
//itsCurrentPB( nullptr ),
//itsReferenceValues( nullptr ),
//itsCornerValue( 0 )
//{
//	itsModes = new IntraMode *[ 4 ];
//	itsModes[ INTRAMODE_PLANAR ] = new PlanarMode( );
//	itsModes[ INTRAMODE_DC ] = new DcMode( );
//	itsModes[ INTRAMODE_LINEAR ] = new LinearMode( );
//	itsModes[ INTRAMODE_ANGULAR ] = new AngMode( );
//}
//
//IntraPred::~IntraPred( )
//{
//	for( int i = 0; i < 4; i++ )
//		delete itsModes[ i ];
//	delete[] itsModes;
//}
//
//IntraPred* IntraPred::getInstance( )
//{
//	if( itsInstance == nullptr )
//		itsInstance = new IntraPred( );
//	return itsInstance;
//}
//
//int IntraPred::getFilteringThreshold( ) const
//{
//	int thresh = 10;
//	switch( itsCurrentPB->getSize( ) )
//	{
//	case 8:
//		thresh = 7;
//		break;
//	case 16:
//		thresh = 1;
//		break;
//	case 32:
//		thresh = 0;
//		break;
//	};
//	return thresh;
//}
//
//bool IntraPred::isFilteringRequired( ) const
//{
//	if( itsCurrentPB->getImgComp( ) != LUMA )
//		return false;
//
//	if( itsCurrentPB->getModeIdx( ) == INTRAMODE_DC || itsCurrentPB->getSize( ) == 4 )
//		return false;
//
//	int dist = std::min( std::abs( ( (Int)itsCurrentPB->getModeIdx( ) ) - 10 ), std::abs( ( (Int)itsCurrentPB->getModeIdx( ) ) - 26 ) );
//	if( dist <= getFilteringThreshold( ) )
//		return false;
//
//	return true;
//}
//
//Sample IntraPred::filtRef( const Sample mainRef, const Sample leftRef, const Sample rightRef ) const
//{
//	return ( leftRef + 2 * mainRef + rightRef + 2 ) >> 2;
//}
//
//void IntraPred::filterSideRefs( const IntraDirection dir )
//{
//	Sample* refs = itsReferenceValues[ dir ];
//	Sample* newRefs = new Short[ 2 * itsCurrentPB->getSize( ) ];
//
//	Sample prevRef = itsCornerValue, currRef;
//	int limit = 2 * itsCurrentPB->getSize( ) - 1;
//	for( int x = 0; x < limit; ++x, prevRef = currRef )
//	{
//		currRef = refs[ x ];
//		newRefs[ x ] = filtRef( refs[ x ], prevRef, refs[ x + 1 ] );
//	}
//	newRefs[ 2 * itsCurrentPB->getSize( ) - 1 ] = refs[ 2 * itsCurrentPB->getSize( ) - 1 ];
//	delete[] itsReferenceValues[ dir ];
//	itsReferenceValues[ dir ] = newRefs;
//}
//
//void IntraPred::doReferenceFiltering( )
//{
//	assert( itsReferenceValues != nullptr && itsReferenceValues[ INTRA_DIR_LEFT ] != nullptr && itsReferenceValues[ INTRA_DIR_TOP ] != nullptr );
//
//	int firstLeft = itsReferenceValues[ INTRA_DIR_LEFT ][ 0 ];
//	int firstTop = itsReferenceValues[ INTRA_DIR_TOP ][ 0 ];
//
//	filterSideRefs( INTRA_DIR_LEFT );
//	filterSideRefs( INTRA_DIR_TOP );
//
//	itsCornerValue = filtRef( itsCornerValue, firstLeft, firstTop );
//}
//
//bool IntraPred::checkSmoothConditions( const IntraDirection dir ) const
//{
//	assert( dir != INTRA_DIR_CORNER );
//	Sample* currRefs = itsReferenceValues[ dir ];
//	int cond = std::abs( itsCornerValue + currRefs[ 2 * itsCurrentPB->getSize( ) - 1 ] - 2 * currRefs[ itsCurrentPB->getSize( ) - 1 ] );
//	int limit = 1 << ( SeqParams( )->getBitDepthLuma( ) - 5 );
//	return cond < limit;
//}
//
//bool IntraPred::isSmoothingRequired( ) const
//{
//	bool doSmoothing = ( itsCurrentPB->getSize( ) == 32 ) && SeqParams( )->getStrongIntraSmoothingEnabled( );
//	bool smoothCond = checkSmoothConditions( INTRA_DIR_LEFT ) && checkSmoothConditions( INTRA_DIR_TOP );
//	return doSmoothing && smoothCond;
//}
//
//Sample IntraPred::getSmoothedReferenceAtPosition( const IntraDirection dir, const int offset ) const
//{
//	Sample lastRef = itsReferenceValues[ dir ][ 2 * itsCurrentPB->getSize( ) - 1 ];
//	return ( ( 63 - offset ) * itsCornerValue + ( offset + 1 ) * lastRef + 32 ) >> 6;
//}
//
//void IntraPred::smoothSideRefs( const IntraDirection dir )
//{
//	assert( itsCurrentPB->getSize( ) == 32 );
//
//	Sample* refs = itsReferenceValues[ dir ];
//	Sample* newRefs = new Short[ 2 * itsCurrentPB->getSize( ) ];
//
//	int limit = 2 * itsCurrentPB->getSize( ) - 1;
//	for( int x = 0; x < limit; ++x )
//		newRefs[ x ] = getSmoothedReferenceAtPosition( dir, x );
//
//	newRefs[ limit ] = refs[ limit ];
//	delete itsReferenceValues[ dir ];
//	itsReferenceValues[ dir ] = newRefs;
//}
//
//void IntraPred::doReferenceSmoothing( )
//{
//	smoothSideRefs( INTRA_DIR_LEFT );
//	smoothSideRefs( INTRA_DIR_TOP );
//}
//
//IntraMode *IntraPred::getPredictionStrategy( )
//{
//	switch( itsCurrentPB->getModeIdx( ) )
//	{
//	case 0:
//		return itsModes[ INTRAMODE_PLANAR ];
//	case 1:
//		return itsModes[ INTRAMODE_DC ];
//	case 10:
//	case 26:
//		return itsModes[ INTRAMODE_LINEAR ];
//	default:
//		return itsModes[ INTRAMODE_ANGULAR ];
//	}
//}
//
//Sample** IntraPred::calcPred( PBIntra* newPb )
//{
//	assert( newPb != nullptr );
//
//	itsCurrentPB = newPb;
//
//	itsCurrentPB->calcReferences( );
//
//	itsCornerValue = itsCurrentPB->getCornerReference( );
//	itsReferenceValues = new Short*[2];
//	itsReferenceValues[INTRA_DIR_LEFT] = new Sample[2 * newPb->getSize()];
//	itsReferenceValues[INTRA_DIR_TOP] = new Sample[2 * newPb->getSize()];
//	Sample* ptr1 = itsCurrentPB->getSideRefs( INTRA_DIR_LEFT );
//	Sample* ptr2 = itsCurrentPB->getSideRefs(INTRA_DIR_TOP);
//	for (int i = 0; i < 2 * newPb->getSize(); ++i)
//	{
//		itsReferenceValues[INTRA_DIR_LEFT][i] = ptr1[i];
//		itsReferenceValues[INTRA_DIR_TOP][i] = ptr2[i];
//	}
//
//	if( isFilteringRequired( ) )
//	{
//		if( isSmoothingRequired( ) )
//			doReferenceSmoothing( );
//		else
//			doReferenceFiltering( );
//	}
//
//	IntraMode *strategy = getPredictionStrategy( );
//	strategy->setPb( itsCurrentPB );
//	strategy->setCorner( itsCornerValue );
//	strategy->setSideRefs( INTRA_DIR_LEFT, itsReferenceValues[ INTRA_DIR_LEFT ] );
//	strategy->setSideRefs( INTRA_DIR_TOP, itsReferenceValues[ INTRA_DIR_TOP ] );
//
//	Sample** pred = strategy->calcPred( );
//
//	delete[] itsReferenceValues;
//	itsCurrentPB = nullptr;
//
//	return pred;
//}
//
//Sample **IntraPred::calcPredForceRefs( PBIntra *newPb, Sample *leftRefs, Sample *topRefs, const Sample corner )
//{
//	assert( newPb != nullptr );
//
//	itsCurrentPB = newPb;
//
//	itsCornerValue = corner;
//	itsReferenceValues = new Short*[ 2 ];
//	itsReferenceValues[ INTRA_DIR_LEFT ] = new Short[ 2 * itsCurrentPB->getSize( ) ]; 
//	itsReferenceValues[ INTRA_DIR_TOP ] = new Short[ 2 * itsCurrentPB->getSize( ) ];
//	for( int i = 0; i < 2 * itsCurrentPB->getSize( ); ++i )
//	{
//		itsReferenceValues[ INTRA_DIR_LEFT ][ i ] = leftRefs[ i ];
//		itsReferenceValues[ INTRA_DIR_TOP ][ i ] = topRefs[ i ];
//	}
//	
//	if( isFilteringRequired( ) )
//	{
//		if( isSmoothingRequired( ) )
//			doReferenceSmoothing( );
//		else
//			doReferenceFiltering( );
//	}
//	IntraMode *strategy = getPredictionStrategy( );
//	strategy->setPb( itsCurrentPB );
//	strategy->setCorner( itsCornerValue );
//	strategy->setSideRefs( INTRA_DIR_LEFT, itsReferenceValues[ INTRA_DIR_LEFT ] );
//	strategy->setSideRefs( INTRA_DIR_TOP, itsReferenceValues[ INTRA_DIR_TOP ] );
//
//	Sample** pred = strategy->calcPred( );
//
//	strategy->setPb( nullptr );
//	strategy->setCorner( -1 );
//
//	deleteMatrix( itsReferenceValues, 2 );
//	itsCornerValue = -1;
//	itsCurrentPB = nullptr;
//
//	return pred;
//}