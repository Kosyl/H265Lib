/**
 * @file	TU.cpp
 *
 * @brief	Implementacja klas bloku i jednostki transformacji
 */

#include "TU.h"
#include "CU.h"
#include "Transform.h"
#include "Quant.h"
#include "PU.h"

TU::TU( CU* parentCU, UInt x, UInt y, UInt size ) :
CTUComponentBase( parentCU != nullptr ? parentCU->getPicture( ) : nullptr, parentCU != nullptr ? parentCU->getParentCTU( ) : nullptr, x, y, size ),
itsParentCU( parentCU ),
itsTBs( )
{
	assert( size == 4 || size == 8 || size == 16 || size == 32 );
	itsTBs.push_back( std::make_shared<TB>( this, x, y, size, LUMA ) );

	if( size == SeqParams( )->getMinTUSize( )  )
	{
		if( x % 8 == 0 && y % 8 == 0 )
		{
			itsTBs.push_back( std::make_shared<TB>( this, x / 2, y / 2, size, CB ) );
			itsTBs.push_back( std::make_shared<TB>( this, x / 2, y / 2, size, CR ) );
		}
	}
	else
	{
		itsTBs.push_back( std::make_shared<TB>( this, x / 2, y / 2, size / 2, CB ) );
		itsTBs.push_back( std::make_shared<TB>( this, x / 2, y / 2, size / 2, CR ) );
	}
}

Void TU::printDescription( )
{
	LOG( "PART" ) << "TU[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	LOG_TAB( );
	for( UInt i = 0; i < itsTBs.size( ); ++i )
		itsTBs[ i ]->printDescription( );
	LOG_UNTAB( );
}

//////////////////////////////////////

TB::TB( TU* parentTU, UShort x, UShort y, UShort size, ImgComp comp ) :
UnitBase( parentTU != nullptr ? parentTU->getPicture( ) : nullptr, x, y, size ),
itsParentTU( parentTU ),
itsComp( comp ),
itsScanStrategy( nullptr ),
itsCodedSubBlockFlags( ),
itsLastSignificantCoeffX( 0 ),
itsLastSignificantCoeffY( 0 )
{
	UInt shift = ( itsComp == LUMA ? 0 : 1 );
	UInt reconOffsetX = ( itsParentTU->getX( ) - itsParentTU->getParentCU( )->getX( ) ) >> ( shift );
	UInt reconOffsetY = ( itsParentTU->getY( ) - itsParentTU->getParentCU( )->getY( ) ) >> ( shift );

	itsPictureSamples = Submatrix<Sample>( parentTU->getPicture( )->getSamples( itsComp ), size, x, y );
	itsReconstructionTarget = Submatrix<Sample>( parentTU->getPicture( )->getReconMatrix( itsComp ), size, x, y );
	itsPrediction = Submatrix<Sample>( parentTU->getParentCU( )->getPredictionMatrix( itsComp ), size, reconOffsetX, reconOffsetY );
	itsResiduals = getEmptyMatrix<Sample>( itsSize, itsSize );
	itsCoefficients = getEmptyMatrix<Coeff>( itsSize, itsSize );
	itsHelperPB = std::make_shared<PBIntra>( this, comp, x, y, size );
}

TB::~TB( )
{
	deleteMatrix( itsResiduals, itsSize );
	deleteMatrix( itsCoefficients, itsSize );
}

Void TB::transformAndReconstruct( std::shared_ptr<PUIntra> mainPU )
{
#pragma region Log

	LOG( "OPT" ) << "TB[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", comp: " << itsComp << std::endl;
	LOG( "OPT" ) << "Helper PB:" << std::endl;
	itsHelperPB->printDescription( );

	LOG_TAB( "OPT" );

	UInt shift = itsComp == LUMA ? 0 : 1;

#pragma endregion
	LOG( "OPT" ) << "Pelna predykcja przed:" << std::endl;
	printMatrix( itsParentTU->getParentCU( )->getPredictionMatrix( itsComp ), (itsParentTU->getParentCU( )->getSize()) >> shift, LOG( "OPT" ) );
	itsHelperPB->calcAndWritePredictionToCU( mainPU );

#pragma region Log
	LOG( "OPT" ) << "Predykcja:" << std::endl;
	printMatrix( itsPrediction, itsSize, LOG( "OPT" ) ); 
	LOG( "OPT" ) << "Pelna predykcja po:" << std::endl;
	printMatrix( itsParentTU->getParentCU( )->getPredictionMatrix( itsComp ), (itsParentTU->getParentCU( )->getSize( ) ) >> shift, LOG( "OPT" ) );

#pragma endregion

	assignScanStrategy( );

	for( UInt x = 0; x < itsSize; ++x )
	{
		for( UInt y = 0; y < itsSize; ++y )
		{
			itsResiduals[ x ][ y ] = itsPictureSamples[ x ][ y ] - itsPrediction[ x ][ y ];
		}
	}

#pragma region Log

	LOG( "OPT" ) << "Residua:" << std::endl;
	printMatrix( itsResiduals, itsSize, LOG( "OPT" ) );
	LOG( "OPT" ) << "QP: " << PicParams( )->getQP( itsComp ) << std::endl;
#pragma endregion

	Transform* tr = Transform::getInstance( );
	Quant* quant = Quant::getInstance( );

	Coeff** tmpCoeffs = getEmptyMatrix<Coeff>( itsSize, itsSize );
	Sample** reconResiduals = getEmptyMatrix<Sample>( itsSize, itsSize );

	tr->forwardTrans( itsResiduals, tmpCoeffs, itsSize, SeqParams( )->getBitDepth( itsComp ), false, false );
	quant->quant( tmpCoeffs, itsCoefficients, PicParams( )->getQP( itsComp ), SeqParams( )->getBitDepth( itsComp ), itsSize );
	quant->deQuant( itsCoefficients, tmpCoeffs, PicParams( )->getQP( itsComp ), SeqParams( )->getBitDepth( itsComp ), itsSize );
	tr->inverseTrans( tmpCoeffs, reconResiduals, itsSize, SeqParams( )->getBitDepth( itsComp ), false, false );

#pragma region Log
	LOG( "OPT" ) << "Residua po rekonstrukcji:" << std::endl;
	printMatrix( reconResiduals, itsSize, LOG( "OPT" ) );
	LOG( "OPT" ) << "itsCoefficients:" << std::endl;
	printMatrix( itsCoefficients, itsSize, LOG( "OPT" ) );
#pragma endregion

	Sample sample = 0;
	for( UInt x = 0; x < itsSize; ++x )
	{
		for( UInt y = 0; y < itsSize; ++y )
		{
			Short sample = itsPictureSamples[ x ][ y ] + itsResiduals[ x ][ y ] - reconResiduals[ x ][ y ];
			itsReconstructionTarget[ x ][ y ] = SeqParams( )->clip( itsComp, sample );
		}
	}
#pragma region Log
	LOG( "OPT" ) << "Obraz zrekonstruowany:" << std::endl;
	printMatrix( itsReconstructionTarget, itsSize, itsSize, LOG( "OPT" ) );
	LOG_UNTAB( "OPT" );
#pragma endregion
}

Void TB::printDescription( )
{
	LOG( "PART" ) << "TB[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << ", comp: " << itsComp << std::endl;
	LOG( "PART" ) << "Helper PB:" << std::endl;
	LOG_TAB( "PART" );
	itsHelperPB->printDescription( );
	LOG_UNTAB( "PART" );
}

Void TB::assignScanStrategy( )
{
	assert( itsHelperPB != nullptr && itsHelperPB->predictionDone( ) );
	itsScanStrategy = new DiagonalScanningMode( );
	if( itsSize == 4 || ( itsSize == 8 && itsComp == LUMA ) )
	{
		UInt mode = itsHelperPB->getModeIdx( );
		if( mode >= 6 && mode <= 14 )
		{
			delete itsScanStrategy;
			itsScanStrategy = new VerticalScanningMode( );
		}
		else if( mode >= 22 && mode <= 30 )
		{
			delete itsScanStrategy;
			itsScanStrategy = new HorizontalScanningMode( );
		}
	}
}

Bool TB::hasAtLeastOneNonZeroCoeff( )
{
	for( Int i = 0; i < itsSize; ++i )
	{
		for( Int j = 0; j < itsSize; ++j )
		{
			if( itsCoefficients[ i ][ j ] != 0 )
				return true;
		}
	}
	return false;
}