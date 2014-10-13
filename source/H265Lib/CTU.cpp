/**
 * @file	CTU.cpp
 *
 * @brief	Implementacja klasy jednostki drzewa kodowego.
 */

#include "CTU.h"
#include "CU.h"

CTU::CTU( Picture* picture, std::shared_ptr<Slice> slice, UShort x, UShort y ) :
UnitBase( picture, x, y, SeqParams( )->getCTUSize( ) ),
itsSAOInfo( ),
itsParentTile( ),
itsParentSlice( slice )
{
	assert( slice != nullptr );
	itsCUQuadTree = std::make_shared<CUQuadTree>( this, itsX, itsY, SeqParams( )->getMaxCUSize( ) );
}

CTU::CTU( Picture* picture, std::shared_ptr<Slice> slice, std::shared_ptr<Tile> tile, UShort x, UShort y ) :
CTU( picture, slice, x, y )
{
	assert( tile != nullptr );
	itsParentTile = tile;
}

CTU::~CTU( )
{
}

Void CTU::printDescription( )
{
	LOG( "PART" ) << "CTU[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	itsCUQuadTree->printDescription( );
}

Void CTU::reconstructionLoop( )
{
	LOG( "RECO" ) << "CTU[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	LOG_TAB( "RECO" );
	itsCUQuadTree->reconstructionLoop( );
	LOG_UNTAB("RECO" );
}