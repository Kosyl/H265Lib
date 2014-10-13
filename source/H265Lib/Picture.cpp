/**
 * @file	Picture.cpp
 *
 * @brief	implementacja klasy obrazu
 */

#include "Picture.h"
#include "CTU.h"
#include "Slice.h"
#include "Tile.h"
#include "PartitioningStrategy.h"
#include "CU.h"

Picture::Picture( )
{
	itsSamplesY = itsSamplesCb = itsSamplesCr = itsReconCb = itsReconCr = itsReconY = nullptr;
	initFrame( );
}

void Picture::initFrame( )
{
	setSize( SeqParams( )->getPicWidth( ), SeqParams( )->getPicHeight( ) );

	itsSlices.clear( );
	itsCTUs.clear( );
	itsCurrentCTU.reset( );
	itsCurrentSlice.reset( );

	if( PicParams( )->getTilesEnabled( ) == true )
	{
		itsCurrentTile.reset( );
		itsTiles.clear( );
	}

	initCTUs( );
}

Picture::~Picture( )
{
	deleteMatrices( );
}

void Picture::deleteMatrices( )
{
	if( itsSamplesY )
	{
		UInt width = SeqParams( )->getPicWidth( );

		deleteMatrix( itsSamplesY, width );
		deleteMatrix( itsReconY, width );
		deleteMatrix( itsSamplesCr, width / 2 );
		deleteMatrix( itsSamplesCb, width / 2 );
		deleteMatrix( itsReconCr, width / 2 );
		deleteMatrix( itsReconCb, width / 2 );
	}
}

void Picture::setSize( UInt width, UInt height )
{
	deleteMatrices( );

	if( width == 0 || height == 0 ) return;

	itsSamplesY = getEmptyMatrix<Sample>( width, height );
	itsReconY = getEmptyMatrix<Sample>( width, height );
	UInt ch = height / 2;
	UInt cw = width / 2;

	itsSamplesCb = getEmptyMatrix<Sample>( cw, ch );
	itsSamplesCr = getEmptyMatrix<Sample>( cw, ch );
	itsReconCb = getEmptyMatrix<Sample>( cw, ch );
	itsReconCr = getEmptyMatrix<Sample>( cw, ch );
}

void Picture::loadFrameFromYuv( std::ifstream& yuvFile )
{
	UChar tmp;
	UInt width = SeqParams( )->getPicWidth( ), height = SeqParams( )->getPicHeight( );

	for( UInt i = 0; i < height; ++i )
	{
		for( UInt j = 0; j < width; ++j )
		{
			yuvFile.read( reinterpret_cast<Char*>( &tmp ), 1 );
			itsSamplesY[ j ][ i ] = tmp;
		}
	}

	UInt ch = height / 2;
	UInt cw = width / 2;

	for( UInt i = 0; i < ch; ++i )
	{
		for( UInt j = 0; j < cw; ++j )
		{
			yuvFile.read( reinterpret_cast<Char*>( &tmp ), 1 );
			itsSamplesCb[ j ][ i ] = tmp;
		}
	}
	for( UInt i = 0; i < ch; ++i )
	{
		for( UInt j = 0; j < cw; ++j )
		{
			yuvFile.read( reinterpret_cast<Char*>( &tmp ), 1 );
			itsSamplesCr[ j ][ i ] = tmp;
		}
	}
}

Sample** Picture::getSamples( const ImgComp comp ) const
{
	if( comp == LUMA )
		return itsSamplesY;
	else if( comp == CR )
		return itsSamplesCr;
	else
		return itsSamplesCb;
}

Sample** Picture::getReconMatrix( const ImgComp comp ) const
{
	if( comp == LUMA )
		return itsReconY;
	else if( comp == CR )
		return itsReconCr;
	else
		return itsReconCb;
}

Void Picture::initCTUs( )
{
	UShort width = SeqParams( )->getPicWidth( ), height = SeqParams( )->getPicHeight( ), CTUSize = SeqParams( )->getCTUSize( );
	UShort picWidthInCTUs = SeqParams( )->getPicWidthInCTUs( ), picHeightInCTUs = SeqParams( )->getPicHeightInCTUs( );

	itsCurrentSlice = std::make_shared<Slice>( this );
	itsSlices.push_back( itsCurrentSlice );

	if( PicParams( )->getTilesEnabled( ) == true )
	{
		itsCurrentTile = std::make_shared<Tile>( 0, 0, picWidthInCTUs*CTUSize, picHeightInCTUs*CTUSize );
		itsTiles.push_back( itsCurrentTile );

		for( UInt i = 0; i < height; i += CTUSize )
		{
			for( UInt j = 0; j < width; j += CTUSize )
			{
				std::shared_ptr<CTU> ctu( new CTU( this, itsCurrentSlice, itsCurrentTile, j, i ) );
				itsCTUs.push_back( ctu );
				itsCurrentTile->appendCTU( ctu );
				itsCurrentSlice->appendCTU( ctu );
			}
		}
	}
	else
	{
		for( UInt i = 0; i < height; i += CTUSize )
		{
			for( UInt j = 0; j < width; j += CTUSize )
			{
				std::shared_ptr<CTU> ctu( new CTU( this, itsCurrentSlice, j, i ) );
				itsCTUs.push_back( ctu );
				itsCurrentSlice->appendCTU( ctu );
			}
		}
	}
	itsCurrentCTU = itsCTUs[ 0 ];
}

Void Picture::initPartitions( PartitioningStrategy* strategy )
{
	for( UInt i = 0; i < itsCTUs.size( ); ++i )
	{
		if( itsCTUs[ i ] != nullptr )
			strategy->doPartitioning( itsCTUs[ i ] );
	}
}

Void Picture::printDescription( )
{
	LOG( "PART" ) << "Obraz " << SeqParams( )->getPicWidth( ) << " x " << SeqParams( )->getPicHeight( ) << std::endl;
	printMatrix( itsSamplesY, SeqParams( )->getPicWidth( ), SeqParams( )->getPicHeight( ), LOG( "PART" ) );
	LOG_TAB( );
	for( UInt i = 0; i < itsCTUs.size( ); ++i )
	{
		if( itsCTUs[ i ] != nullptr ) itsCTUs[ i ]->printDescription( );
	}
	LOG_UNTAB( );
}

Void Picture::reconstructionLoop( )
{
	LOG( "RECO" ) << "REKONSTRUKCJA " << SeqParams( )->getPicWidth( ) << " x " << SeqParams( )->getPicHeight( ) << std::endl << "Luma:" << std::endl;
	LOG( "MATRIX" ) << "LUMA:" << std::endl;
	printMatrix( itsSamplesY, SeqParams( )->getPicWidth( ), SeqParams( )->getPicHeight( ), LOG( "RECO" ) );
	printMatrix( itsSamplesY, SeqParams( )->getPicWidth( ), SeqParams( )->getPicHeight( ), LOG( "MATRIX" ) );
	LOG( "RECO" ) << "CB:" << std::endl;
	printMatrix( itsSamplesCb, SeqParams( )->getPicWidth( ) / 2, SeqParams( )->getPicHeight( ) / 2, LOG( "RECO" ) );
	printMatrix( itsSamplesCb, SeqParams( )->getPicWidth( ) / 2, SeqParams( )->getPicHeight( ) / 2, LOG( "MATRIX" ) );
	LOG( "RECO" ) << "CR:" << std::endl;
	printMatrix( itsSamplesCr, SeqParams( )->getPicWidth( ) / 2, SeqParams( )->getPicHeight( ) / 2, LOG( "MATRIX" ) );
	printMatrix( itsSamplesCr, SeqParams( )->getPicWidth( ) / 2, SeqParams( )->getPicHeight( ) / 2, LOG( "RECO" ) );
	LOG_TAB( "RECO" );
	for( UInt i = 0; i < itsCTUs.size( ); ++i )
	{
		if( itsCTUs[ i ] != nullptr ) itsCTUs[ i ]->reconstructionLoop( );
	}
	LOG_UNTAB( "RECO" );
}

std::shared_ptr<CU> Picture::getCuContainingPosition( UInt x, UInt y )
{
	std::shared_ptr<CTU> ctu = getCTUBySamplePosition( x, y );
	std::shared_ptr<CU> cu = nullptr;
	std::shared_ptr<CUQuadTree> tree = ctu->getCUQuadTree( );

	do
	{
		if( tree->getQTMode( ) == QTMODE_LEAF )
		{
			cu = tree->getLeaf( );
		}
		else
		{
			UInt step = tree->getSize( ) / 2;
			Int left, up;
			left = tree->getX( ) + step > x ? 1 : 0;
			up = tree->getY( ) + step > x ? 1 : 0;
			QTComponent comp;
			if( left && up )
				comp = UPPERLEFT;
			else if( !left && up )
				comp = UPPERRIGHT;
			else if( left && !up )
				comp = LOWERLEFT;
			else
				comp = LOWERRIGHT;

			tree = tree->getSubTree( comp );
		}
	}
	while( cu == nullptr );
	return cu;
}