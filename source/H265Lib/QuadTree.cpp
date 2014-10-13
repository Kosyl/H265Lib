/**
 * @file	QuadTree.cpp
 *
 * @brief	Implementacja klas czerodrzew
 */

#include "QuadTree.h"
#include "SeqPicParameters.h"
#include "CU.h"
#include "CTU.h"
#include "TU.h"

template<class T, class S>
QuadTree<T, S>::QuadTree( CTU* parentCTU, UInt x, UInt y, UInt size ) :
CTUComponentBase( parentCTU != nullptr ? parentCTU->getPicture( ) : nullptr, parentCTU, x, y, size )
{
	for( Int i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i )
		itsSubTrees[ i ] = nullptr;

	itsUnit = nullptr;
}

template<class T, class S>
QuadTree<T, S>::~QuadTree( )
{
}

//template<class T, class S>
//Void QuadTree::createSubTrees( )
//{
//	itsSubTrees[ UPPERLEFT ] = createSubTree( itsParentCTU, itsX, itsY, itsSize / 2 );
//
//	Bool createRight = itsX + itsSize / 2 < SeqParams( )->getPicWidth( );
//	Bool createLower = itsY + itsSize / 2 < SeqParams( )->getPicHeight( );
//
//	if( createRight )
//		itsSubTrees[ UPPERRIGHT ] = createSubTree( itsParentCTU, itsX + ( itsSize / 2 ), itsY, itsSize / 2 );
//
//	if( createLower )
//		itsSubTrees[ LOWERLEFT ] = createSubTree( itsParentCTU, itsX, itsY + ( itsSize / 2 ), itsSize / 2 );
//
//	if( createLower && createRight )
//		itsSubTrees[ LOWERRIGHT ] = createSubTree( itsParentCTU, itsX + ( itsSize / 2 ), itsY + ( itsSize / 2 ), itsSize / 2 );
//
//	itsUnit = nullptr;
//}

CUQuadTree::CUQuadTree( CTU* parentCTU, UInt x, UInt y, UInt size ) :
QuadTree( parentCTU, x, y, size )
{
	assert( size == 8 || size == 16 || size == 32 || size == 64 );
}

CUQuadTree::~CUQuadTree( )
{
}

Void CUQuadTree::printDescription( )
{
	LOG( "PART" ) << "CUQuadTree[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	LOG_TAB( );
	if( isLeaf( ) )
	{
		itsUnit->printDescription( );
	}
	else
	{
		printSubTreesDescription( );
	}
	LOG_UNTAB( );
}

TUQuadTree::TUQuadTree( CU* parentCU, UInt x, UInt y, UInt size ) :
QuadTree( parentCU != nullptr ? parentCU->getParentCTU( ) : nullptr, x, y, size ),
itsParentCu( parentCU )
{
	assert( size == 4 || size == 8 || size == 16 || size == 32 || size == 64 );
}

TUQuadTree::~TUQuadTree( )
{
}

Void TUQuadTree::printDescription( )
{
	LOG( "PART" ) << "TUQuadTree[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	LOG_TAB( );
	if( isLeaf( ) )
	{
		itsUnit->printDescription( );
	}
	else
	{
		printSubTreesDescription( );
	}
	LOG_UNTAB( );
}

Void CUQuadTree::reconstructionLoop( )
{
	LOG( "RECO" ) << "CUQuadTree[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	LOG_TAB( "RECO" );
	if( isLeaf( ) )
	{
		itsUnit->reconstructionLoop( );
	}
	else
	{
		if( itsSubTrees[ UPPERLEFT ] != nullptr )
		{
			LOG( "RECO" ) << "UPPERLEFT" << std::endl;
			itsSubTrees[ UPPERLEFT ]->reconstructionLoop( );
		}
		if( itsSubTrees[ UPPERRIGHT ] != nullptr )
		{
			LOG( "RECO" ) << "UPPERRIGHT" << std::endl;
			itsSubTrees[ UPPERRIGHT ]->reconstructionLoop( );
		}
		if( itsSubTrees[ LOWERLEFT ] != nullptr )
		{
			LOG( "RECO" ) << "LOWERLEFT" << std::endl;
			itsSubTrees[ LOWERLEFT ]->reconstructionLoop( );
		}
		if( itsSubTrees[ LOWERRIGHT ] != nullptr )
		{
			LOG( "RECO" ) << "LOWERRIGHT" << std::endl;
			itsSubTrees[ LOWERRIGHT ]->reconstructionLoop( );
		}
	}
	LOG_UNTAB( "RECO" );
}

Void TUQuadTree::reconstructionLoop( std::shared_ptr<PUIntra> mainPU )
{
	LOG( "RECO" ) << "TUQuadTree[" << getX( ) << "][" << getY( ) << "], size: " << getSize( ) << std::endl;
	LOG_TAB( "RECO" );
	if( isLeaf( ) )
	{
		itsUnit->reconstructionLoop( mainPU );
	}
	else
	{
		if( itsSubTrees[ UPPERLEFT ] != nullptr )
		{
			LOG( "RECO" ) << "UPPERLEFT" << std::endl;
			itsSubTrees[ UPPERLEFT ]->reconstructionLoop( mainPU );
		}
		if( itsSubTrees[ UPPERRIGHT ] != nullptr )
		{
			LOG( "RECO" ) << "UPPERRIGHT" << std::endl;
			itsSubTrees[ UPPERRIGHT ]->reconstructionLoop( mainPU );
		}
		if( itsSubTrees[ LOWERLEFT ] != nullptr )
		{
			LOG( "RECO" ) << "LOWERLEFT" << std::endl;
			itsSubTrees[ LOWERLEFT ]->reconstructionLoop( mainPU );
		}
		if( itsSubTrees[ LOWERRIGHT ] != nullptr )
		{
			LOG( "RECO" ) << "LOWERRIGHT" << std::endl;
			itsSubTrees[ LOWERRIGHT ]->reconstructionLoop( mainPU );
		}
	}
	LOG_UNTAB( "RECO" );
}

Double CUQuadTree::getTotalCost( )
{
	if( itsMode == QTMODE_LEAF )
		return itsUnit->getTotalCost( );
	else
	{
		Double sum = 0.0;

		for( Int i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i )
		if( itsSubTrees[ i ] != nullptr )
			sum += itsSubTrees[ i ]->getTotalCost( );

		return sum;
	}
}

std::shared_ptr<TU> TUQuadTree::getTuContainingPosition( UInt x, UInt y )
{
	if( itsMode == QTMODE_LEAF )
	{
		return itsUnit;
	}
	else
	{
		UInt step = itsSize / 2;
		Int left, up;
		left = itsX + step > x ? 0 : 1;
		up = itsY + step > x ? 0 : 1;
		QTComponent comp;
		if( left && up )
			comp = UPPERLEFT;
		else if( !left && up )
			comp = UPPERRIGHT;
		else if( left && !up )
			comp = LOWERLEFT;
		else
			comp = LOWERRIGHT;

		return getSubTree( comp )->getTuContainingPosition( x, y );
	}
}