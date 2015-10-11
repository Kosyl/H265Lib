///**
// * @file	DeblockingFilter.cpp
// *
// * @brief	Implementacja klasy filtra deblokuj¹cego.
// */
//
//#include "DeblockingFilter.h"
//#include "SeqPicParameters.h"
//#include "CTU.h"
//#include "TU.h"
//DeblockingFilter* DeblockingFilter::instance = nullptr;
//
//UShort DeblockingFilter::BetaPrim[ 52 ] =
//{
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 6, 7, 8, 9,
//	10, 11, 12, 13, 14, 15, 16, 17, 18, 20,
//	22, 24, 26, 28, 30, 32, 34, 36, 38, 40,
//	42, 44, 46, 48, 50, 52, 54, 56, 58, 60,
//	62, 64
//};
//
//UShort DeblockingFilter::TcPrim[ 54 ] =
//{
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
//	1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
//	2, 3, 3, 3, 3, 4, 4, 4, 5, 5,
//	6, 6, 7, 8, 9, 10, 11, 13, 14, 16,
//	18, 20, 22, 24
//};
//
//UShort DeblockingFilter::QPc[ 14 ] =
//{
//	29, 30, 31, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37
//};
//
//Short DeblockingFilter::getQPc( Short qPi )
//{
//	if( qPi < 30 )
//	{
//		return qPi;
//	}
//	else if( qPi>43 )
//	{
//		return qPi - 6;
//	}
//	else
//		return DeblockingFilter::QPc[ qPi - 30 ];
//}
//
//DeblockingFilter* DeblockingFilter::getInstance( )
//{
//	if( instance == nullptr )
//		instance = new DeblockingFilter( );
//	return instance;
//}
//
///** Konstruktor przyjmuje sciezke do logu i rozkaz wlaczenia/nie wlaczania logu
//*	\param path sciezka do pliku logu
//\param isLogging pisze log, jesli true
//*/
//DeblockingFilter::DeblockingFilter( ) :
//itsEdgeFlags( nullptr ),
//itsFilterStrength( nullptr ),
//itsShouldFilterCBEdge( true )
//{
//}
//
//DeblockingFilter::~DeblockingFilter( )
//{
//	resetMatrices( );
//}
//
//Void DeblockingFilter::resetMatrices( )
//{
//	if( itsEdgeFlags != nullptr )
//		deleteMatrix( itsEdgeFlags, itsCurrentSize );
//
//	if( itsFilterStrength != nullptr )
//		deleteMatrix( itsFilterStrength, itsCurrentSize );
//}
//
//Void DeblockingFilter::doFiltering( std::shared_ptr<Picture> pic, EdgeType direction )
//{
//	for( Int i = 0; i < SeqParams( )->getPicHeightInCTUs( ); ++i )
//	{
//		for( Int j = 0; j < SeqParams( )->getPicWidthInCTUs( ); ++j )
//		{
//			std::shared_ptr<CTU> ctu = pic->getCTU( j, i );
//			if( ctu->getParentSlice( )->getDeblockingFilterEnabled( ) )
//				doFiltering( ctu->getCUQuadTree( ), direction );
//		}
//	}
//}
//
//Void DeblockingFilter::doFiltering( std::shared_ptr<CUQuadTree> qt, EdgeType direction )
//{
//	if( qt->getQTMode( ) == QTMODE_LEAF )
//	{
//		doFiltering( qt->getLeaf( ), direction );
//	}
//	else
//	{
//		for( QTComponent i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i )
//		{
//			if( qt->getSubTree( i ) != nullptr )
//				doFiltering( qt->getSubTree( i ), direction );
//		}
//	}
//}
//
//Void DeblockingFilter::doFiltering( std::shared_ptr<CUIntra> cu, EdgeType direction )
//{
//	resetMatrices( );
//	itsCurrentSize = cu->getSize( );
//	/*//LOG( "FDB" ) << "CU(" << cu->getX( ) << "," << cu->getY( ) << "), " << cu->getSize( ) << "x" << cu->getSize( ) << std::endl;
//	//LOG_TAB "FDB" );*/
//	itsEdgeFlags = getEmptyMatrix<Bool>( itsCurrentSize, itsCurrentSize );
//	itsFilterStrength = getEmptyMatrix<UShort>( itsCurrentSize, itsCurrentSize );
//
//	itsShouldFilterCBEdge = checkCUEdge( cu, direction );
//	fillInTransformEdges( cu->getTransformTree( ), direction );
//	fillInPredictionEdges( cu, direction );
//	////LOG( "FDB" ) << "flagi: " << std::endl;
//	//printMatrix( itsEdgeFlags, itsCurrentSize, //LOG( "FDB" ) );
//	calcFilterStrength( cu, direction );
//	////LOG( "FDB" ) << "sila filtru: " << std::endl;
//	//printMatrix( itsFilterStrength, itsCurrentSize, //LOG( "FDB" ) );
//	filtreEdges( cu, direction );
//	//LOG_UNTAB "FDB" );
//}
//
//Bool DeblockingFilter::checkCUEdge( std::shared_ptr<CUIntra> cu, EdgeType direction )
//{
//	if( direction == EDGE_VER )
//	{
//		if( cu->getX( ) == 0 )
//			return false;
//
//		if( cu->getX( ) == cu->getParentCTU( )->getX( ) && ( PicParams( )->getLoopFilterOnSlicesEnabled( ) || PicParams( )->getLoopFilterOnTilesEnabled( ) ) )
//		{
//			UInt ctuX = cu->getParentCTU( )->getX( );
//			UInt ctuY = cu->getParentCTU( )->getY( );
//
//			if( ctuX > 0 )
//			{
//				std::shared_ptr<CTU> leftCTU = cu->getPicture( )->getCTU( ctuX - 1, ctuY );
//				if( leftCTU->getParentSlice( )->getSliceIdx( ) != cu->getParentCTU( )->getParentSlice( )->getSliceIdx( ) && PicParams( )->getLoopFilterOnSlicesEnabled( ) )
//					return false;
//
//				if( leftCTU->getParentTile( )->getIdx( ) != cu->getParentCTU( )->getParentTile( )->getIdx( ) && PicParams( )->getLoopFilterOnTilesEnabled( ) )
//					return false;
//			}
//		}
//	}
//	else
//	{
//		if( cu->getY( ) == 0 )
//			return false;
//
//		if( cu->getY( ) == cu->getParentCTU( )->getY( ) && ( PicParams( )->getLoopFilterOnSlicesEnabled( ) || PicParams( )->getLoopFilterOnTilesEnabled( ) ) )
//		{
//			UInt ctuX = cu->getParentCTU( )->getX( );
//			UInt ctuY = cu->getParentCTU( )->getY( );
//
//			if( ctuY > 0 )
//			{
//				std::shared_ptr<CTU> upperCTU = cu->getPicture( )->getCTU( ctuX, ctuY - 1 );
//				if( upperCTU->getParentSlice( )->getSliceIdx( ) != cu->getParentCTU( )->getParentSlice( )->getSliceIdx( ) && PicParams( )->getLoopFilterOnSlicesEnabled( ) )
//					return false;
//
//				if( upperCTU->getParentTile( )->getIdx( ) != cu->getParentCTU( )->getParentTile( )->getIdx( ) && PicParams( )->getLoopFilterOnTilesEnabled( ) )
//					return false;
//			}
//		}
//	}
//
//	return true;
//}
//
//Void DeblockingFilter::fillInTransformEdges( std::shared_ptr<TUQuadTree> TUtree, EdgeType direction )
//{
//	if( TUtree->getQTMode( ) == QTMODE_LEAF )
//	{
//		UInt size = TUtree->getSize( );
//		UInt relativeX = TUtree->getX( ) - TUtree->getParentCU( )->getX( );
//		UInt relativeY = TUtree->getY( ) - TUtree->getParentCU( )->getY( );
//
//		if( direction == EDGE_VER )
//		{
//			for( UInt i = 0; i < size; ++i )
//				itsEdgeFlags[ relativeX ][ relativeY + i ] = relativeX == 0 ? itsShouldFilterCBEdge : true;
//		}
//		else
//		{
//			for( UInt i = 0; i < size; ++i )
//				itsEdgeFlags[ relativeX + i ][ relativeY ] = relativeY == 0 ? itsShouldFilterCBEdge : true;
//		}
//	}
//	else
//	{
//		for( QTComponent i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i )
//		{
//			if( TUtree->getSubTree( i ) != nullptr )
//				fillInTransformEdges( TUtree->getSubTree( i ), direction );
//			else
//			{
//				assert( false );
//			}
//		}
//	}
//}
//
//Void DeblockingFilter::fillInPredictionEdges( std::shared_ptr<CUIntra>cu, EdgeType direction )
//{
//	PartitionMode mode = cu->getPartitionMode( );
//	UInt size = cu->getSize( );
//	Bool partitionAllowsFilter = true;
//
//	if( direction == EDGE_VER )
//	{
//		UInt x = 0;
//		switch( mode )
//		{
//		case PART_Nx2N:
//		case PART_NxN:
//			x = size / 2;
//			break;
//		case PART_nLx2N:
//			x = size / 4;
//			break;
//		case PART_nRx2N:
//			x = ( size / 4 ) * 3;
//			break;
//		default:
//			partitionAllowsFilter = false;
//		}
//		if( partitionAllowsFilter )
//		{
//			for( UInt i = 0; i < size; ++i )
//				itsEdgeFlags[ x ][ i ] = true;
//		}
//	}
//	else
//	{
//		UInt y = 0;
//		switch( mode )
//		{
//		case PART_2NxN:
//		case PART_NxN:
//			y = size / 2;
//			break;
//		case PART_2NxnU:
//			y = size / 4;
//			break;
//		case PART_2NxnD:
//			y = ( size / 4 ) * 3;
//			break;
//		default:
//			partitionAllowsFilter = false;
//		}
//		if( partitionAllowsFilter )
//		{
//			for( UInt i = 0; i < size; ++i )
//				itsEdgeFlags[ i ][ y ] = true;
//		}
//	}
//}
//
//Void DeblockingFilter::calcFilterStrength( std::shared_ptr<CUIntra>cu, EdgeType direction )
//{
//	Sample** picture = cu->getPicture( )->getReconMatrix( LUMA );
//	UInt cuX = cu->getX( ), cuY = cu->getY( );
//	UInt size = cu->getSize( );
//	UInt xN = ( size / 8 ) - 1;
//	UInt yN = ( size / 4 ) - 1;
//	UInt x = 0, y = 0;
//	Sample p0, q0;
//
//	if( direction == EDGE_HOR )
//		std::swap( xN, yN );
//
//	for( UInt i = 0; i <= xN; ++i )
//	{
//		for( UInt j = 0; j <= yN; ++j )
//		{
//			x = i << ( direction == EDGE_VER ? 3 : 2);
//			y = j << ( direction == EDGE_VER ? 2 : 3 );
//
//			if( itsEdgeFlags[ x ][ y ] == true )
//			{
//				q0 = picture[ cuX + x ][ cuY + y ];
//				if( direction == EDGE_VER )
//				{
//					p0 = picture[ cuX + x - 1 ][ cuY + y ];
//				}
//				else
//				{
//					p0 = picture[ cuX + x ][ cuY + y - 1 ];
//				}
//				if( cu->getPredictionType( ) == PREDICTION_INTRA )
//				{
//					itsFilterStrength[ x ][ y ] = 2;
//					continue;
//				}
//				if( cu->getPicture( )->getCuContainingPosition( cuX + x + ( direction == EDGE_VER ? -1 : 0 ), cuY + y + ( direction == EDGE_HOR ? -1 : 0 ) )->getPredictionType( ) == PREDICTION_INTRA )
//				{
//					itsFilterStrength[ x ][ y ] = 2;
//					continue;
//				}
//
//				Bool q0HasNonZeroCoeff = cu->getTransformTree( )->getTuContainingPosition( cuX + x, cuY + y )->getTB( LUMA )->hasAtLeastOneNonZeroCoeff( );
//				std::shared_ptr<CU> neighbor = cu->getPicture( )->getCuContainingPosition( cuX + x + ( direction == EDGE_VER ? -1 : 0 ), cuY + y + ( direction == EDGE_HOR ? -1 : 0 ) );
//				Bool p0HasNonZeroCoeff = neighbor->getTransformTree( )->getTuContainingPosition( cuX + x + ( direction == EDGE_VER ? -1 : 0 ), cuY + y + ( direction == EDGE_HOR ? -1 : 0 ) )->getTB( LUMA )->hasAtLeastOneNonZeroCoeff( );
//				if( q0HasNonZeroCoeff || p0HasNonZeroCoeff )
//				{
//					itsFilterStrength[ x ][ y ] = 1;
//					continue;
//				}
//
//				if( false )//warunki dla inter
//				{
//					itsFilterStrength[ x ][ y ] = 1;
//					continue;
//				}
//				itsFilterStrength[ x ][ y ] = 0;
//			}
//		}
//	}
//}
//
//Void DeblockingFilter::filtreEdges( std::shared_ptr<CUIntra>cu, EdgeType direction )
//{
//	UInt xStep = direction == EDGE_VER ? 8 : 4;
//	UInt yStep = direction == EDGE_HOR ? 8 : 4;
//	for( UInt x = 0; x < cu->getSize( ); x += xStep )
//	{
//		////LOG_TAB "FDB" );
//		for( UInt y = 0; y< cu->getSize( ); y += yStep )
//		{
//			if( itsFilterStrength[ x ][ y ] > 0 )
//			{
//				////LOG( "FDB" ) << "[" << x << "][" << y << "]" << std::endl;
//				caclDecisions( cu, x, y, direction );
//				if( itsCurrentdE != 0 )
//				{
//					/*//LOG( "FDB" ) << "filtruje" << std::endl;
//					//LOG_TAB "FDB" );*/
//					if( direction == EDGE_VER )
//						doLumaFilteringVertical( cu, x, y );
//					else
//						doLumaFilteringHorizontal( cu, x, y );
//					////LOG_UNTAB "FDB" );
//				}
//				else
//				{
//					////LOG( "FDB" ) << "pomijam" << std::endl;
//				}
//			}
//		}
//		////LOG_UNTAB "FDB" );
//	}
//	for( UShort x = 0; x < cu->getSize( )/2; x += 4 )
//	{
//		for( UShort y = 0; y < cu->getSize( )/2; y += 4 )
//		{
//			if(
//				direction == EDGE_VER &&
//				itsFilterStrength[ 2 * x ][ 2 * y ] == 2 &&
//				( ( ( cu->getX( ) / 2 + x ) >> 3 ) << 3 ) == cu->getX( ) / 2 + x
//				)
//			{
//				doChromaFiltering( cu, x, y, EDGE_VER, CB, PicParams( )->getQPOffsetForCb( ) );
//				doChromaFiltering( cu, x, y, EDGE_VER, CR, PicParams( )->getQpOffsetForCr( ) );
//			}
//			else if
//				(
//				direction == EDGE_HOR &&
//				itsFilterStrength[ 2 * x ][ 2 * y ] == 2 &&
//				( ( ( cu->getY( ) / 2 + y ) >> 3 ) << 3 ) == cu->getY( ) / 2 + y )
//			{
//				doChromaFiltering( cu, x, y, EDGE_HOR, CB, PicParams( )->getQPOffsetForCb( ) );
//				doChromaFiltering( cu, x, y, EDGE_HOR, CR, PicParams( )->getQpOffsetForCr( ) );
//			}
//		}
//	}
//}
//
//Void DeblockingFilter::caclDecisions( std::shared_ptr<CUIntra> cu, UInt x, UInt y, EdgeType direction )
//{
//	Sample** pic = cu->getPicture( )->getReconMatrix( LUMA );
//	Sample q0[ 4 ], q3[ 4 ], p0[ 4 ], p3[ 4 ];
//	if( direction == EDGE_VER )
//	{
//		for( UInt i = 0; i < 4; ++i )
//		{
//			q0[ i ] = pic[ cu->getX( ) + x + i ][ cu->getY( ) + y ];
//			q3[ i ] = pic[ cu->getX( ) + x + i ][ cu->getY( ) + y + 3 ];
//			p0[ i ] = pic[ cu->getX( ) + x - i - 1 ][ cu->getY( ) + y ];
//			p3[ i ] = pic[ cu->getX( ) + x - i - 1 ][ cu->getY( ) + y + 3 ];
//		}
//	}
//	else
//	{
//		for( UInt i = 0; i < 4; ++i )
//		{
//			q0[ i ] = pic[ cu->getX( ) + x ][ cu->getY( ) + y + i ];
//			q3[ i ] = pic[ cu->getX( ) + x + 3 ][ cu->getY( ) + y + i ];
//			p0[ i ] = pic[ cu->getX( ) + x ][ cu->getY( ) + y - i - 1 ];
//			p3[ i ] = pic[ cu->getX( ) + x + 3 ][ cu->getY( ) + y - i - 1 ];
//		}
//	}
//
//	/*//LOG( "FDB" ) << "decyzje" << std::endl;
//	//LOG( "FDB" ) << "q0:" << std::endl;
//	printTable( q0, 4, //LOG( "FDB" ), 4 );
//	//LOG( "FDB" ) << "q3:" << std::endl;
//	printTable( q3, 4, //LOG( "FDB" ), 4 );
//	//LOG( "FDB" ) << "p0:" << std::endl;
//	printTable( p0, 4, //LOG( "FDB" ), 4 );
//	//LOG( "FDB" ) << "p3:" << std::endl;
//	printTable( p3, 4, //LOG( "FDB" ), 4 );*/
//
//	Short QPq = cu->getAbsoluteQP( );
//	UInt neighborX, neighborY;
//	if( direction == EDGE_VER )
//	{
//		neighborX = cu->getX( ) + x - 1;
//		neighborY = cu->getY( ) + y;
//	}
//	else
//	{
//		neighborX = cu->getX( ) + x;
//		neighborY = cu->getY( ) + y - 1;
//	}
//	Short QPp = cu->getPicture( )->getCuContainingPosition( neighborX, neighborY )->getAbsoluteQP( );
//
//	Short qPl = ( ( QPp + QPq + 1 ) >> 1 );
//	UInt Q = clipToRange( 0, 51, qPl + cu->getParentCTU( )->getParentSlice( )->getBetaOffset( ) );
//	UShort betaPrim = DeblockingFilter::BetaPrim[ Q ];
//	UShort beta = betaPrim * ( 1 << ( SeqParams( )->getBitDepthLuma( ) - 8 ) );
//	Q = clipToRange( 0, 53, qPl + 2 * ( itsFilterStrength[ x ][ y ] - 1 ) + cu->getParentCTU( )->getParentSlice( )->getTcOffset( ) );
//	UShort tcprim = DeblockingFilter::TcPrim[ Q ];
//	UShort tc = tcprim * ( 1 << ( SeqParams( )->getBitDepthLuma( ) - 8 ) );
//
//	Sample dp0 = std::abs( p0[ 2 ] - 2 * p0[ 1 ] + p0[ 0 ] );
//	Sample dp3 = std::abs( p3[ 2 ] - 2 * p3[ 1 ] + p3[ 0 ] );
//	Sample dq0 = std::abs( q0[ 2 ] - 2 * q0[ 1 ] + q0[ 0 ] );
//	Sample dq3 = std::abs( q3[ 2 ] - 2 * q3[ 1 ] + q3[ 0 ] );
//	Sample dpq0 = dp0 + dq0;
//	Sample dpq3 = dp3 + dq3;
//	Sample dp = dp0 + dp3;
//	Sample dq = dq0 + dq3;
//	Sample d = dpq0 + dpq3;
//	UShort dE = 0, dEp = 0, dEq = 0;
//	if( d < beta )
//	{
//		////LOG( "FDB" ) << "d < beta" << std::endl;
//		Sample dpq = 2 * dpq0;
//		Bool dSam0 = decideFilterLuma( p0[ 0 ], p0[ 3 ], q0[ 0 ], q0[ 3 ], dpq, beta, tc );
//		dpq = 2 * dpq3;
//		Bool dSam3 = decideFilterLuma( p3[ 0 ], p3[ 3 ], q3[ 0 ], q3[ 3 ], dpq, beta, tc );
//		dE = 1;
//		if( dSam0 == 1 && dSam3 == 1 )
//			dE = 2;
//		if( dp < ( ( beta + ( beta >> 1 ) ) >> 3 ) )
//			dEp = 1;
//		if( dq < ( ( beta + ( beta >> 1 ) ) >> 3 ) )
//			dEq = 1;
//	}
//
//	itsCurrentBeta = beta;
//	itsCurrentdE = dE;
//	itsCurrentdEp = dEp;
//	itsCurrentdEq = dEq;
//	itsCurrentTc = tc;
//	////LOG( "FDB" ) << "B: " << beta << ", tc: " << tc << ", dE: " << dE << ", dEp: " << dEp << ", dEq: " << dEq << std::endl;
//}
//
//Bool DeblockingFilter::decideFilterLuma( Sample p0, Sample p3, Sample q0, Sample q3, Sample dpq, Short beta, Short tc )
//{
//	if(
//		dpq < ( beta >> 2 ) &&
//		( std::abs( p3 - p0 ) + std::abs( q0 - q3 ) ) < ( beta >> 3 ) &&
//		std::abs( p0 - q0 ) < ( ( 5 * tc + 1 ) >> 1 )
//		)
//		return true;
//
//	return false;
//}
//
//Void DeblockingFilter::doLumaFilteringVertical( std::shared_ptr<CUIntra> cu, UInt x, UInt y )
//{
//	Sample** q = getEmptyMatrix<Sample>( 4, 4 );
//	Sample** p = getEmptyMatrix<Sample>( 4, 4 );
//	UInt xCU = cu->getX( );
//	UInt yCU = cu->getY( );
//	Sample** source = cu->getPicture( )->getReconMatrix( LUMA );
//
//	for( UInt i = 0; i < 4; ++i )
//	{
//		for( UInt j = 0; j < 4; ++j )
//		{
//			q[ i ][ j ] = source[ xCU + x + i ][ yCU + y + j ];
//			p[ i ][ j ] = source[ xCU + x - i - 1 ][ yCU + y + j ];
//		}
//	}
//
//	for( UInt k = 0; k < 4; ++k )
//	{
//		////LOG_TAB "FDB" );
//		for( UInt i = 0; i < 4; ++i )
//		{
//			itsCurrentP[ i ] = p[ i ][ k ];
//			itsCurrentQ[ i ] = q[ i ][ k ];
//		}
//		for( UInt i = 0; i < 3; ++i )
//		{
//			itsCurrentPX[ i ] = xCU + x - i - 1;
//			itsCurrentPY[ i ] = yCU + y + k;
//			itsCurrentQX[ i ] = xCU + x + i;
//			itsCurrentQY[ i ] = yCU + y + k;
//		}
//		////LOG( "FDB" ) << "przed filtracja:" << std::endl;
//		/*printMatrix( source, 8, 4, //LOG( "FDB" ), xCU + x - 4, yCU + y );
//		//LOG_TAB "FDB" );*/
//		filteringProcess( cu );
//		////LOG_UNTAB "FDB" );
//		if( itsCurrentNDp > 0 )
//		{
//			for( UInt i = 0; i < itsCurrentNDp; ++i )
//			{
//				source[ xCU + x - i - 1 ][ yCU + y + k ] = itsCurrentFilteredP[ i ];
//			}
//		}
//		if( itsCurrentNDq > 0 )
//		{
//			for( UInt i = 0; i < itsCurrentNDq; ++i )
//			{
//				source[ xCU + x + i ][ yCU + y + k ] = itsCurrentFilteredQ[ i ];
//			}
//		}
//		/*//LOG( "FDB" ) << "po filtracji:" << std::endl;
//		printMatrix( source, 8, 4, //LOG( "FDB" ), xCU + x - 4, yCU + y );
//		//LOG_UNTAB "FDB" );*/
//	}
//
//	deleteMatrix( q, 4 );
//	deleteMatrix( p, 4 );
//}
//
//Void DeblockingFilter::doLumaFilteringHorizontal( std::shared_ptr<CUIntra> cu, UInt x, UInt y )
//{
//	Sample** q = getEmptyMatrix<Sample>( 4, 4 );
//	Sample** p = getEmptyMatrix<Sample>( 4, 4 );
//	UInt xCU = cu->getX( );
//	UInt yCU = cu->getY( );
//	Sample** source = cu->getPicture( )->getReconMatrix( LUMA );
//
//	for( UInt i = 0; i < 4; ++i )
//	{
//		for( UInt j = 0; j < 4; ++j )
//		{
//			q[ i ][ j ] = source[ xCU + x + j ][ yCU + y + i ];
//			p[ i ][ j ] = source[ xCU + x + j ][ yCU + y - i - 1 ];
//		}
//	}
//
//	for( UInt k = 0; k < 4; ++k )
//	{
//		for( UInt i = 0; i < 4; ++i )
//		{
//			itsCurrentP[ i ] = p[ i ][ k ];
//			itsCurrentQ[ i ] = q[ i ][ k ];
//		}
//		for( UInt i = 0; i < 3; ++i )
//		{
//			itsCurrentPX[ i ] = xCU + x + k;
//			itsCurrentPY[ i ] = yCU + y - i - 1;
//			itsCurrentQX[ i ] = xCU + x + k;
//			itsCurrentQY[ i ] = yCU + y + i;
//		}
//		////LOG( "FDB" ) << "przed filtracja:" << std::endl;
//		/*printMatrix( source, 4, 8, //LOG( "FDB" ), xCU + x, yCU + y -4);
//		//LOG_TAB "FDB" );*/
//		filteringProcess( cu );
//		////LOG_UNTAB "FDB" );
//		if( itsCurrentNDp > 0 )
//		{
//			for( UInt i = 0; i < itsCurrentNDp; ++i )
//			{
//				source[ xCU + x + k ][ yCU + y - i - 1 ] = itsCurrentFilteredP[ i ];
//			}
//		}
//		if( itsCurrentNDq > 0 )
//		{
//			for( UInt i = 0; i < itsCurrentNDq; ++i )
//			{
//				source[ xCU + x + k ][ yCU + y + i ] = itsCurrentFilteredQ[ i ];
//			}
//		}
//		/*//LOG( "FDB" ) << "po filtracji:" << std::endl;
//		printMatrix( source, 4, 8, //LOG( "FDB" ), xCU + x, yCU + y - 4 );*/
//	}
//
//	deleteMatrix( q, 4 );
//	deleteMatrix( p, 4 );
//}
//
//Void DeblockingFilter::filteringProcess( std::shared_ptr<CUIntra> cu )
//{
//	if( itsCurrentdE == 2 )
//	{
//		////LOG( "FDB" ) << "silne filtrowanie! nadpiszemy 3 próbki" << std::endl;
//		itsCurrentNDp = itsCurrentNDq = 3;
//		itsCurrentFilteredP[ 0 ] = clipToRange( itsCurrentP[ 0 ] - 2 * itsCurrentTc, itsCurrentP[ 0 ] + 2 * itsCurrentTc, ( itsCurrentP[ 2 ] + 2 * itsCurrentP[ 1 ] + 2 * itsCurrentP[ 0 ] + 2 * itsCurrentQ[ 0 ] + itsCurrentQ[ 1 ] + 4 ) >> 3 );
//		itsCurrentFilteredP[ 1 ] = clipToRange( itsCurrentP[ 1 ] - 2 * itsCurrentTc, itsCurrentP[ 1 ] + 2 * itsCurrentTc, ( itsCurrentP[ 2 ] + itsCurrentP[ 1 ] + itsCurrentP[ 0 ] + itsCurrentQ[ 0 ] + 2 ) >> 2 );
//		itsCurrentFilteredP[ 2 ] = clipToRange( itsCurrentP[ 2 ] - 2 * itsCurrentTc, itsCurrentP[ 2 ] + 2 * itsCurrentTc, ( 2 * itsCurrentP[ 3 ] + 3 * itsCurrentP[ 2 ] + itsCurrentP[ 1 ] + itsCurrentP[ 0 ] + itsCurrentQ[ 0 ] + 4 ) >> 3 );
//		itsCurrentFilteredQ[ 0 ] = clipToRange( itsCurrentQ[ 0 ] - 2 * itsCurrentTc, itsCurrentQ[ 0 ] + 2 * itsCurrentTc, ( itsCurrentP[ 1 ] + 2 * itsCurrentP[ 0 ] + 2 * itsCurrentQ[ 0 ] + 2 * itsCurrentQ[ 1 ] + itsCurrentQ[ 2 ] + 4 ) >> 3 );
//		itsCurrentFilteredQ[ 1 ] = clipToRange( itsCurrentQ[ 1 ] - 2 * itsCurrentTc, itsCurrentQ[ 1 ] + 2 * itsCurrentTc, ( itsCurrentP[ 0 ] + itsCurrentQ[ 0 ] + itsCurrentQ[ 1 ] + itsCurrentQ[ 2 ] + 2 ) >> 2 );
//		itsCurrentFilteredQ[ 2 ] = clipToRange( itsCurrentQ[ 2 ] - 2 * itsCurrentTc, itsCurrentQ[ 2 ] + 2 * itsCurrentTc, ( itsCurrentP[ 0 ] + itsCurrentQ[ 0 ] + itsCurrentQ[ 1 ] + 3 * itsCurrentQ[ 2 ] + 2 * itsCurrentQ[ 3 ] + 4 ) >> 3 );
//
//		/*//LOG( "FDB" ) << "itsCurrentP:" << std::endl;
//		printTable( itsCurrentP, 4, //LOG( "FDB" ), 4 );
//		//LOG( "FDB" ) << "itsCurrentFilteredP:" << std::endl;
//		printTable( itsCurrentFilteredP, itsCurrentNDp, //LOG( "FDB" ), 4 );
//		//LOG( "FDB" ) << "itsCurrentQ:" << std::endl;
//		printTable( itsCurrentQ, 4, //LOG( "FDB" ), 4 );
//		//LOG( "FDB" ) << "itsCurrentFilteredQ:" << std::endl;
//		printTable( itsCurrentFilteredQ, itsCurrentNDq, //LOG( "FDB" ), 4 );*/
//	}
//	else
//	{
//		itsCurrentNDq = itsCurrentNDp = 0;
//		Int delta = ( 9 * ( itsCurrentQ[ 0 ] - itsCurrentP[ 0 ] ) - 3 * ( itsCurrentQ[ 1 ] - itsCurrentP[ 1 ] ) + 8 ) >> 4;
//		if( std::abs( delta ) < itsCurrentTc * 10 )
//		{
//			delta = clipToRange( -1 * itsCurrentTc, 1 * itsCurrentTc, delta );
//			itsCurrentFilteredP[ 0 ] = SeqParams( )->clip( LUMA, itsCurrentP[ 0 ] + delta );
//			itsCurrentFilteredQ[ 0 ] = SeqParams( )->clip( LUMA, itsCurrentQ[ 0 ] - delta );
//			if( itsCurrentdEp == 1 )
//			{
//				Int deltaP = clipToRange( -1 * ( itsCurrentTc >> 1 ), 1 * ( itsCurrentTc >> 1 ), ( ( ( itsCurrentP[ 2 ] + itsCurrentP[ 0 ] + 1 ) >> 1 ) - itsCurrentP[ 1 ] + delta ) >> 1 );
//				itsCurrentFilteredP[ 1 ] = SeqParams( )->clip( LUMA, itsCurrentP[ 1 ] + deltaP );
//			}
//			if( itsCurrentdEq == 1 )
//			{
//				Int deltaQ = clipToRange( -1 * ( itsCurrentTc >> 1 ), 1 * ( itsCurrentTc >> 1 ), ( ( ( itsCurrentQ[ 2 ] + itsCurrentQ[ 0 ] + 1 ) >> 1 ) - itsCurrentQ[ 1 ] - delta ) >> 1 );
//				itsCurrentFilteredQ[ 1 ] = SeqParams( )->clip( LUMA, itsCurrentQ[ 1 ] + deltaQ );
//			}
//			itsCurrentNDp = itsCurrentdEp + 1;
//			itsCurrentNDq = itsCurrentdEq + 1;
//		}
//		/*//LOG( "FDB" ) << "itsCurrentNDp:" << itsCurrentNDp << std::endl;
//		//LOG( "FDB" ) << "itsCurrentNDq:" << itsCurrentNDq << std::endl;
//		//LOG( "FDB" ) << "itsCurrentP:" << std::endl;
//		printTable( itsCurrentP, 4, //LOG( "FDB" ), 4 );
//		//LOG( "FDB" ) << "itsCurrentFilteredP:" << std::endl;
//		printTable( itsCurrentFilteredP, itsCurrentNDp, //LOG( "FDB" ), 4 );
//		//LOG( "FDB" ) << "itsCurrentQ:" << std::endl;
//		printTable( itsCurrentQ, 4, //LOG( "FDB" ), 4 );
//		//LOG( "FDB" ) << "itsCurrentFilteredQ:" << std::endl;
//		printTable( itsCurrentFilteredQ, itsCurrentNDq, //LOG( "FDB" ), 4 );*/
//	}
//	if( itsCurrentNDq > 0 && ( cu->getPicture( )->getCuContainingPosition( itsCurrentQX[ 0 ], itsCurrentQY[ 0 ] )->getTransQuantBypassEnabled( ) ) ) // TODO: pcm
//	{
//		itsCurrentNDq = 0;
//	}
//	if( itsCurrentNDp > 0 && ( cu->getPicture( )->getCuContainingPosition( itsCurrentPX[ 0 ], itsCurrentPY[ 0 ] )->getTransQuantBypassEnabled( ) ) ) // TODO: pcm
//	{
//		itsCurrentNDp = 0;
//	}
//}
//
//Void DeblockingFilter::doChromaFiltering( std::shared_ptr<CUIntra> cu, UInt x, UInt y, EdgeType direction, ImgComp comp, Short qpOffset )
//{
//	Sample** pic = cu->getPicture( )->getReconMatrix( comp );
//	Sample q[ 2 ][ 4 ], p[ 2 ][ 4 ];
//	UInt xCB = cu->getX( ) / 2, yCB = cu->getY( ) / 2;
//	UInt Xq = 0, Yq = 0, Xp = 0, Yp = 0;
//	if( direction == EDGE_VER )
//	{
//		for( UInt k = 0; k < 4; ++k )
//		{
//			for( UInt i = 0; i < 2; ++i )
//			{
//				Xq = xCB + x + i;
//				Yq = yCB + y + k;
//				Xp = xCB + x - i - 1;
//				Yp = yCB + y + k;
//				q[ i ][ k ] = pic[ Xq ][ Yq ];
//				p[ i ][ k ] = pic[ Xp ][ Yp ];
//			}
//		}
//	}
//	else
//	{
//		for( UInt k = 0; k < 4; ++k )
//		{
//			for( UInt i = 0; i < 2; ++i )
//			{
//				Xq = xCB + x + k;
//				Yq = yCB + y + i;
//				Xp = xCB + x + k;
//				Yp = yCB + y - i - 1;
//				q[ i ][ k ] = pic[ Xq ][ Yq ];
//				p[ i ][ k ] = pic[ Xp ][ Yp ];
//			}
//		}
//	}
//
//	Short QPq = cu->getAbsoluteQP( );
//	UInt neighborX, neighborY;
//	if( direction == EDGE_VER )
//	{
//		neighborX = cu->getX( ) + 2 * x - 1;
//		neighborY = cu->getY( ) + 2 * y;
//	}
//	else
//	{
//		neighborX = cu->getX( ) + 2 * x;
//		neighborY = cu->getY( ) + 2 * y - 1;
//	}
//	Short QPp = cu->getPicture( )->getCuContainingPosition( neighborX, neighborY )->getAbsoluteQP( );
//
//	Short qPi = ( ( QPp + QPq + 1 ) >> 1 ) + qpOffset;
//	Short Qpc = DeblockingFilter::getQPc( qPi );
//	UInt Q = clipToRange( 0, 53, Qpc + 2 + ( cu->getParentCTU( )->getParentSlice( )->getTcOffset( ) << 1 ) );
//	UShort tcprim = DeblockingFilter::TcPrim[ Q ];
//	UShort tc = tcprim * ( 1 << ( SeqParams( )->getBitDepthChroma( ) - 8 ) );
//
//	itsCurrentTc = tc;
//
//	Sample newP = 0, newQ = 0; Sample pIn[ 2 ]; Sample qIn[ 2 ];
//	if( direction == EDGE_VER )
//	{
//		for( UInt k = 0; k < 4; ++k )
//		{
//			pIn[ 0 ] = p[ 0 ][ k ]; pIn[ 1 ] = p[ 1 ][ k ];
//			qIn[ 0 ] = q[ 0 ][ k ]; qIn[ 1 ] = q[ 1 ][ k ];
//			filterChromaSamples( cu, pIn, qIn, xCB + x - 1, yCB + y + k, xCB + x, yCB + y + k, comp, newP, newQ );
//			pic[ xCB + x ][ yCB + y + k ] = newQ;
//			pic[ xCB + x - 1 ][ yCB + y + k ] = newP;
//		}
//	}
//	else
//	{
//		for( UInt k = 0; k < 4; ++k )
//		{
//			pIn[ 0 ] = p[ 0 ][ k ]; pIn[ 1 ] = p[ 1 ][ k ];
//			qIn[ 0 ] = q[ 0 ][ k ]; qIn[ 1 ] = q[ 1 ][ k ];
//			filterChromaSamples( cu, pIn, qIn, xCB + x + k, yCB + y - 1, xCB + x + k, yCB + y, comp, newP, newQ );
//			pic[ xCB + x + k ][ yCB + y ] = newQ;
//			pic[ xCB + x + k ][ yCB + y - 1 ] = newP;
//		}
//	}
//}
//
//Void DeblockingFilter::filterChromaSamples( std::shared_ptr<CUIntra> cu, Sample p[ 2 ], Sample q[ 2 ], UInt xP, UInt yP, UInt xQ, UInt yQ, ImgComp comp, Sample& out_p, Sample& out_q )
//{
//	Int delta = clipToRange( -1 * itsCurrentTc, 1 * itsCurrentTc, ( ( ( q[ 0 ] - p[ 0 ] ) << 2 ) + p[ 1 ] - q[ 1 ] + 4 ) >> 3 );
//
//	if( cu->getPicture( )->getCuContainingPosition( xP, yP )->getTransQuantBypassEnabled( ) ) // TODO: pcm
//	{
//		out_p = p[ 0 ];
//	}
//	else
//	{
//		out_p = SeqParams( )->clip( comp, p[ 0 ] + delta );
//	}
//	if( cu->getPicture( )->getCuContainingPosition( xQ, yQ )->getTransQuantBypassEnabled( ) ) // TODO: pcm
//	{
//		out_q = q[ 0 ];
//	}
//	else
//	{
//		out_q = SeqParams( )->clip( comp, q[ 0 ] - delta );
//	}
//}