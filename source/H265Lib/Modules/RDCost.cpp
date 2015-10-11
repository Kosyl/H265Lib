///**
// * @file	RDCost.cpp
// *
// * @brief	Implementacja klasy estymatora funkcji kosztu.
// */
//
//#include "RDCost.h"
//
//const Double RDCost::lambda[ 53 ] =
//{
//	0.053125, 0.0669333, 0.08433, 0.10625, 0.1338666, 0.16866136, 0.2125, 0.2677332231, 0.3373227, 0.425, 0.5354664462, 0.674645447, 0.85, 1.07093289, 1.34929, 1.7, 2.1418657848, 2.698581788, 3.4, 4.28373157, 5.39716357, 6.8, 8.567463139, 10.794327, 13.6, 17.13492627857, 21.5886543, 27.2, 34.269852557, 43.1773, 54.4, 68.539, 86.354617, 108.8, 137.07941, 172.71, 217.6, 274.15882, 345.4184689, 435.2, 548.31764, 690.8369, 870.4, 1096.63528, 1381.67387, 1740.8, 2193.27, 2763.34775, 3481.6, 4386.5411, 5526.6955, 6963.2, 8773.1
//};
//
//RDCost* RDCost::itsInstance = nullptr;
//
//RDCost* RDCost::getInstance( )
//{
//	if( itsInstance == nullptr )
//		itsInstance = new RDCost( );
//	return itsInstance;
//}
//
//RDCost::RDCost( ) :
//itsDistortionFunction( nullptr )
//{
//}
//
//RDCost::~RDCost( )
//{
//}
//
//Void RDCost::setDistortionFunction( Long( *newFunc )( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon ) )
//{
//	itsDistortionFunction = newFunc;
//}
//
//Double RDCost::calcCost( UInt estBits, UInt QP, Submatrix<Sample> picOrig, Submatrix<Sample> picRecon )
//{
//	Double lambda = RDCost::lambda[ QP ];
//	Long dist = itsDistortionFunction(  picOrig, picRecon );
//	return ( ( (Double)dist ) + lambda * ( (Double)estBits ) );
//}
//
//Double RDCost::calcCost( UInt estBits, UInt distortion, UInt QP )
//{
//	Double lambda = RDCost::lambda[ QP ];
//	return ( ( (Double)distortion ) + lambda * ( (Double)estBits ) );
//}
//
//Long RDCost::SSD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon )
//{
//	assert( picOrig.size( ) == picRecon.size( ) );
//	//LOG( "DIST" ) << "######################################" << std::endl;
//	//LOG( "DIST" ) << "SSD, macierz glowna: " << std::endl;
//	//printMatrix( picOrig, picOrig.size( ), //LOG( "DIST" ) );
//	//LOG( "DIST" ) << "macierz znieksztalcona: " << std::endl;
//	//printMatrix( picRecon, picOrig.size( ), //LOG( "DIST" ) );
//	//LOG( "DIST" ) << "znieksztalcenia: " << std::endl;
//	Long res = 0;
//	Int v1, v2;
//
//	for( UInt i = 0; i < picOrig.size( ); ++i )
//	{
//		for( UInt j = 0; j < picOrig.size( ); ++j )
//		{
//			v1 = picOrig[ i ][ j ];
//			v2 = picRecon[ i ][ j ];
//			res += ( v1 - v2 ) * ( v1 - v2 );
//			//LOG( "DIST" ) << ( v1 - v2 ) * ( v1 - v2 ) << " ";
//		}
//		//LOG( "DIST" ) << std::endl;
//	}
//	return res;
//}
//
//Long RDCost::SAD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon )
//{
//	assert( picOrig.size( ) == picRecon.size( ) );
//	Long res = 0;
//	Int v1, v2;
//
//	for( UInt i = 0; i < picOrig.size( ); ++i )
//	{
//		for( UInt j = 0; j < picOrig.size( ); ++j )
//		{
//			v1 = picOrig[ i ][ j ];
//			v2 = picRecon[ i ][ j ];
//			res += std::abs( v1 - v2 );
//		}
//	}
//	return res;
//}
//
//UInt RDCost::calcDistortion( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon )
//{
//	return itsDistortionFunction( picOrig, picRecon );
//}