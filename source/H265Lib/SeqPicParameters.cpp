/**
 * @file	SeqPicParameters.cpp
 *
 * @brief	Implementacja klas parametrów sekwencji i obrazu
 */

#include "SeqPicParameters.h"

SequenceParameters* SequenceParameters::itsInstance = nullptr;

Int** ZScanArray::itsValues = nullptr;
UInt  ZScanArray::itsSizeX = 0;
UInt  ZScanArray::itsSizeY = 0;

SequenceParameters::SequenceParameters( )
{
	initWithDefaults( );
}

SequenceParameters::~SequenceParameters( )
{
}

SequenceParameters* SequenceParameters::getInstance( )
{
	if( itsInstance == nullptr )
	{
		itsInstance = new SequenceParameters( );
	}
	return itsInstance;
}

Void SequenceParameters::initWithDefaults( )
{
	itsBitDepthLuma = itsBitDepthChroma = 8;
	setMaxCUSize( 64 );
	setMinCUSize( 8 );
	setMaxTUSize( 32 );
	setMinTUSize( 4 );
	itsIntraSmoothingEnabled = true;
	itsSAOEnabled = true;
}

PictureParameters* PictureParameters::itsInstance = nullptr;

PictureParameters::PictureParameters( )
{
	initWithDefaults( );
}

PictureParameters::~PictureParameters( )
{
}

PictureParameters* PictureParameters::getInstance( )
{
	if( itsInstance == nullptr )
		itsInstance = new PictureParameters( );
	return itsInstance;
}

Void PictureParameters::initWithDefaults( )
{
	itsDeblockingFilterControlEnabled = itsLoopFilterOnSlicesEnabled = itsLoopFilterOnTilesEnabled = false;
	itsSDHEnabled = itsTransformSkipEnabled = false;
	itsQP = 10;
	itsQPOffsetForCb = itsQPOffsetForCr = 0;
}

PictureParameters* PicParams( )
{
	return PictureParameters::getInstance( );
}

SequenceParameters* SeqParams( )
{
	return SequenceParameters::getInstance( );
}

UInt getRasterIdxIn4x4( UInt x, UInt y )
{
	return ( ( y / 4 ) * SeqParams( )->getPicWidth( ) / 4 ) + ( x / 4 );
}

Void ZScanArray::reset( )
{
	UInt y, x, m, p, i, tbX, tbY, ctbAddrRs;
	UInt minTUSize = SeqParams( )->getLog2MinTUSize( ), CTUSize = SeqParams( )->getLog2CTUSize( );
	UInt PicWidthInCTUs = SeqParams( )->getPicWidthInCTUs( ), PicHeightInCTUs = SeqParams( )->getPicHeightInCTUs( );

	if( minTUSize < 2 || CTUSize < 4 || PicHeightInCTUs <= 0 || PicHeightInCTUs <= 0 )
		return;

	UInt yLimit = PicHeightInCTUs << ( CTUSize - minTUSize );
	UInt xLimit = PicWidthInCTUs << ( CTUSize - minTUSize );
	ZScanArray::itsSizeX = xLimit;
	ZScanArray::itsSizeY = yLimit;
	ZScanArray::itsValues = getEmptyMatrix<Int>( xLimit, yLimit );
	for( y = 0; y < yLimit; ++y )
	{
		for( x = 0; x < xLimit; ++x )
		{
			tbX = ( x << minTUSize ) >> CTUSize;
			tbY = ( y << minTUSize ) >> CTUSize;
			ctbAddrRs = PicWidthInCTUs * tbY + tbX;
			ZScanArray::itsValues[ x ][ y ] = ctbAddrRs << ( ( CTUSize - minTUSize ) * 2 );
			for( i = 0, p = 0; i < ( CTUSize - minTUSize ); i++ )
			{
				m = 1 << i;
				p += ( m & x ? m * m : 0 ) + ( m & y ? 2 * m * m : 0 );
			}
			ZScanArray::itsValues[ x ][ y ] = ZScanArray::itsValues[ x ][ y ] + p;
		}
	}
}

Void ZScanArray::print( std::ostream& out )
{
	printMatrix( itsValues, itsSizeX, itsSizeY, out, 0, 0, "" );
}

Int ZScanArray::get( UInt x, UInt y )
{
	if( x < itsSizeX && y < itsSizeY )
		return ZScanArray::itsValues[ x ][ y ];
	else
		return -1;
}

UInt getZScanIdxIn4x4( const UInt puX, const UInt puY )
{
	UInt puXDivBy4 = puX >> SeqParams( )->getLog2MinTUSize( );
	UInt puYDivBy4 = puY >> SeqParams( )->getLog2MinTUSize( );
	UInt calcPuIdx = ( ( puXDivBy4 & 8 ) << 3 ) | ( ( puXDivBy4 & 4 ) << 2 ) | ( ( puXDivBy4 & 2 ) << 1 ) | ( puXDivBy4 & 1 );
	calcPuIdx |= ( ( puYDivBy4 & 8 ) << 4 ) | ( ( puYDivBy4 & 4 ) << 3 ) | ( ( puYDivBy4 & 2 ) << 2 ) | ( ( puYDivBy4 & 1 ) << 1 );
	return calcPuIdx;
}