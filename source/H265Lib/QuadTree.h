/**
* @file	QuadTree.h
*
* @brief	Deklaracja klas czterodrzew
*/

#ifndef _CUQUADTREE_H_
#define _CUQUADTREE_H_

#include "Utils.h"
#include "Picture.h"
#include "UnitBase.h"

class CU;
class TU;
class CUIntra;
class PUIntra;

/**
 * @class	QuadTree
 *
 * @brief	Bazowa klasa czterodrzewa.
 *
 * @tparam	T			Typ liœcia
 * @tparam	self	Typ poddrzewa (w CRTP - klasa pochodna)
 */

template< class T, class self >
class QuadTree : public CTUComponentBase
{
protected:

	/**
	 * @brief	WskaŸniki na poddrzewa.
	 */

	std::shared_ptr<self > itsSubTrees[ 4 ];

	/**
	 * @brief	WskaŸnik na lisæ.
	 */

	std::shared_ptr<T> itsUnit;

	/**
	 * @brief	Tryb drzewa (liœæ/poddrzewo).
	 */

	QTMode itsMode;

	/**
	 * @brief Zniekszta³cenia dla ka¿dej sk³adowej obrazu.
	 */

	UInt itsTotalDistortion[ 3 ];

	/**
	 * @brief	Suma binów dla ka¿dej sk³adowej obrazu.
	 */

	UInt itsTotalBins[ 3 ];
	
public:

	/**
	 * @fn	QuadTree::QuadTree( CTU* parentCTU, UInt x, UInt y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in,out]	Nadrzêdna jednostka CTU.
	 * @param	x									 	Wspó³rzêdna X drzewa.
	 * @param	y									 	Wspó³rzêdna Y drzewa.
	 * @param	size							 	Rozmiar drzewa.
	 */

	QuadTree( CTU* parentCTU, UInt x, UInt y, UInt size );

	/**
	 * @fn	virtual QuadTree::~QuadTree( );
	 *
	 * @brief	Destruktor.
	 */

	virtual ~QuadTree( );

#pragma region Akcesory

	/**
	 * @fn	std::shared_ptr<self> QuadTree::getSubTree( QTComponent placement )
	 *
	 * @brief	Dostêp do poddrzewa na zadanej pozycji.
	 *
	 * @param	placement Pozycja poddrzewa w drzewie.
	 *
	 * @return	WskaŸnik na poddrzewo.
	 */

	std::shared_ptr<self> getSubTree( QTComponent placement )
	{
		return itsSubTrees[ placement ];
	}

	/**
	 * @fn	std::shared_ptr<T> QuadTree::getLeaf( )
	 *
	 * @brief	Dostêp do liœcia.
	 *
	 * @return	WskaŸnik na liœæ.
	 */

	std::shared_ptr<T> getLeaf( )
	{
		return itsUnit;
	}

	/**
	 * @fn	Void QuadTree::setLeaf( std::shared_ptr<T> rhs )
	 *
	 * @brief	Ustawia liœæ drzewa.
	 *
	 * @param	rhs	WskaŸnik na liœæ.
	 *
	 * @return	Void.
	 */

	Void setLeaf( std::shared_ptr<T> rhs )
	{
		itsUnit = rhs;
	}

	/**
	 * @fn	Void QuadTree::clear( )
	 *
	 * @brief	Usuwa liœæ i poddrzewa.
	 *
	 * @return	Void.
	 */

	Void clear( )
	{
		for( UInt i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i )
		{
			itsSubTrees[ i ] = nullptr;
		}
		setLeaf( nullptr );
	}

	/**
	 * @fn	QTMode QuadTree::getQTMode( ) const
	 *
	 * @brief	Zwraca tryb drzewa.
	 *
	 * @return	Tryb drzewa.
	 */

	QTMode getQTMode( ) const
	{
		return itsMode;
	}

	/**
	 * @fn	void QuadTree::setQTMode( QTMode val )
	 *
	 * @brief	Ustawia tryb drzewa.
	 *
	 * @param	val	Nowy tryb drzewa.
	 */

	void setQTMode( QTMode val )
	{
		itsMode = val;
	}

	/**
	 * @fn	UInt QuadTree::getTotalDistortion( ImgComp comp ) const
	 *
	 * @brief	Zniekszta³cenie dla danej sk³adowej obrazu.
	 *
	 * @param	comp	Sk³adowa obrazu.
	 *
	 * @return	Wartoœæ zniekszta³cenia.
	 */

	UInt getTotalDistortion( ImgComp comp ) const
	{
		return itsTotalDistortion[comp];
	}

	/**
	 * @fn	void QuadTree::setTotalDistortion( UInt val, ImgComp comp )
	 *
	 * @brief	Ustawia wartoœæ zniekszta³cenia.
	 *
	 * @param	val 	Zniekszta³cenie.
	 * @param	comp	Sk³adowa obrazu.
	 */

	void setTotalDistortion( UInt val, ImgComp comp )
	{
		itsTotalDistortion[comp] = val;
	}	

	/**
	 * @fn	UInt QuadTree::getTotalBins( ImgComp comp ) const
	 *
	 * @brief	Pobiera iloœæ binów na których jest kodowany obszar danej sk³adowej obrazu.
	 *
	 * @param	comp	Sk³adowa obrazu.
	 *
	 * @return	Iloœæ binów.
	 */

	UInt getTotalBins( ImgComp comp ) const
	{
		return itsTotalBins[ comp ];
	}

	/**
	 * @fn	void QuadTree::setTotalBins( UInt val, ImgComp comp )
	 *
	 * @brief	Ustawia liczbê binów dla danej sk³adowej.
	 *
	 * @param	val 	Ilosæ binów.
	 * @param	comp	Sk³adowa obrazu.
	 */

	void setTotalBins( UInt val, ImgComp comp )
	{
		itsTotalBins[ comp ] = val;
	}

#pragma endregion

	/**
	 * @fn	virtual std::shared_ptr<self > QuadTree::createSubTree( UInt x, UInt y, UInt size ) = 0;
	 *
	 * @brief	Tworzy poddrzewo.
	 *
	 * @param	x			Wspó³rzêdna X poddrzewa.
	 * @param	y			Wspó³rzêdna Y poddrzewa.
	 * @param	size	Rozmiar poddrzewa.
	 *
	 * @return	Nowe poddrzewo.
	 */

	virtual std::shared_ptr<self > createSubTree( UInt x, UInt y, UInt size ) = 0;

	/**
	 * @fn	Void QuadTree::createSubTrees( )
	 *
	 * @brief	Tworzy max. 4 poddrzewa.
	 *
	 * @return	Void
	 */

	Void createSubTrees( )
	{
		itsSubTrees[ UPPERLEFT ] = createSubTree( itsX, itsY, itsSize / 2 );

		Bool createRight = itsX + itsSize / 2 < SeqParams( )->getPicWidth( );
		Bool createLower = itsY + itsSize / 2 < SeqParams( )->getPicHeight( );

		if( createRight )
			itsSubTrees[ UPPERRIGHT ] = createSubTree( itsX + ( itsSize / 2 ), itsY, itsSize / 2 );

		if( createLower )
			itsSubTrees[ LOWERLEFT ] = createSubTree( itsX, itsY + ( itsSize / 2 ), itsSize / 2 );

		if( createLower && createRight )
			itsSubTrees[ LOWERRIGHT ] = createSubTree( itsX + ( itsSize / 2 ), itsY + ( itsSize / 2 ), itsSize / 2 );
	}

	/**
	 * @fn	Bool QuadTree::isLeaf( )
	 *
	 * @brief	Sprawdza, czy drzewo jest liœciem.
	 *
	 * @return	true, jeœli drzewo jest liœciem.
	 */

	Bool isLeaf( )
	{
		return itsMode == QTMODE_LEAF;
	}

	/**
	 * @fn	Bool QuadTree::isSplit( )
	 *
	 * @brief	Sprawdza, czy drzewo jest podzielone na poddrzewa.
	 *
	 * @return	true, jeœli drzewo jest podzielone na poddrzewa.
	 */

	Bool isSplit( )
	{
		itsMode == QTMODE_SPLIT;
	}

	/**
	 * @fn	Void QuadTree::printSubTreesDescription( )
	 *
	 * @brief Wypisuje opis poddrzew.
	 *
	 * @return	Void.
	 */

	Void printSubTreesDescription( )
	{
		if( itsSubTrees[ UPPERLEFT ] != nullptr )
		{
			LOG( "PART" ) << "UPPERLEFT" << std::endl;
			itsSubTrees[ UPPERLEFT ]->printDescription( );
		}
		if( itsSubTrees[ UPPERRIGHT ] != nullptr )
		{
			LOG( "PART" ) << "UPPERRIGHT" << std::endl;
			itsSubTrees[ UPPERRIGHT ]->printDescription( );
		}
		if( itsSubTrees[ LOWERLEFT ] != nullptr )
		{
			LOG( "PART" ) << "LOWERLEFT" << std::endl;
			itsSubTrees[ LOWERLEFT ]->printDescription( );
		}
		if( itsSubTrees[ LOWERRIGHT ] != nullptr )
		{
			LOG( "PART" ) << "LOWERRIGHT" << std::endl;
			itsSubTrees[ LOWERRIGHT ]->printDescription( );
		}
	}
};

/**
 * @class	CUQuadTree
 *
 * @brief	A Drzewo jednostek kodowych
 */

class CUQuadTree : public QuadTree<CUIntra, CUQuadTree>
{
private:

public:

	/**
	 * @fn	CUQuadTree::CUQuadTree( CTU* parentCTU, UInt x, UInt y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	Nadrzêdna jednostka CTU.
	 * @param	x									 	Wspó³rzêdna X drzewa.
	 * @param	y									 	Wspó³rzêdna Y drzewa.
	 * @param	size							 	Rozmiar drzewa.
	 */

	CUQuadTree( CTU* parentCTU, UInt x, UInt y, UInt size );

	/**
	 * @fn	virtual CUQuadTree::~CUQuadTree( );
	 *
	 * @brief	Destruktor.
	 */

	virtual ~CUQuadTree( );

	/**
	 * @fn	std::shared_ptr<CUIntra> CUQuadTree::getCU( )
	 *
	 * @brief	Dostêp do liœcia.
	 *
	 * @return	WskaŸnik na CU.
	 */

	std::shared_ptr<CUIntra> getCU( )
	{
		return itsUnit;
	}

	/**
	 * @fn	virtual std::shared_ptr<CUQuadTree > CUQuadTree::createSubTree( UInt x, UInt y, UInt size )
	 *
	 * @brief	Tworzy poddrzewo CU.
	 *
	 * @param	x			Wspó³rzêdna X poddrzewa.
	 * @param	y			Wspó³rzêdna X poddrzewa.
	 * @param	size	Rozmiar poddrzewa CU.
	 *
	 * @return	Nowe poddrzewo.
	 */

	virtual std::shared_ptr<CUQuadTree > createSubTree( UInt x, UInt y, UInt size )
	{
		return std::make_shared<CUQuadTree>( itsParentCTU, x, y, size );
	}

	/**
	 * @fn	virtual Void CUQuadTree::printDescription( );
	 *
	 * @brief	Wypisuje opis drzewa.
	 *
	 * @return	Void.
	 */

	virtual Void printDescription( );

	/**
	 * @fn	Void CUQuadTree::reconstructionLoop( );
	 *
	 * @brief	Pêtla rekonstrukcji.
	 *
	 * @return	Void.
	 */

	Void reconstructionLoop( );

	/**
	 * @fn	Double CUQuadTree::getTotalCost( );
	 *
	 * @brief	Zwraca ca³kowity koszt drzewa.
	 *
	 * @return	Ca³kowity koszt.
	 */

	Double getTotalCost( );
};

/**
 * @class	TUQuadTree
 *
 * @brief Drzewo transformacji.
 */

class TUQuadTree : public QuadTree<TU, TUQuadTree>
{
private:

	/**
	 * @brief	Nadrzêdna jednostka kodowa.
	 */

	CU* itsParentCu;

public:

	/**
	 * @fn	TUQuadTree::TUQuadTree( CU* parentCU, UInt x, UInt y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in,out]	parentCU	If non-null, the parent cu.
	 * @param	x										Wspó³rzêdna X drzewa.
	 * @param	y										Wspó³rzêdna X drzewa.
	 * @param	size								Rozmiar drzewa.
	 */

	TUQuadTree( CU* parentCU, UInt x, UInt y, UInt size );

	/**
	 * @fn	virtual TUQuadTree::~TUQuadTree( );
	 *
	 * @brief	Destruktor.
	 */

	virtual ~TUQuadTree( );

	/**
	 * @fn	std::shared_ptr<TU> TUQuadTree::getTU( )
	 *
	 * @brief	Dostêp do liscia.
	 *
	 * @return	WskaŸnik na liœæ (TU).
	 */

	std::shared_ptr<TU> getTU( )
	{
		return itsUnit;
	}

	/**
	 * @fn	CU* TUQuadTree::getParentCU( )
	 *
	 * @brief	Dostêp do nadrzêdnego CU.
	 *
	 * @return	WskaŸnik na nadrzêdne CU.
	 */

	CU* getParentCU( )
	{
		return itsParentCu;
	}

	/**
	 * @fn	virtual std::shared_ptr<TUQuadTree > TUQuadTree::createSubTree( UInt x, UInt y, UInt size )
	 *
	 * @brief	Tworzy poddrzewo.
	 *
	 * @param	x			Wspó³rzêdna X poddrzewa.
	 * @param	y			Wspó³rzêdna X poddrzewa.
	 * @param	size	Rozmiar poddrzewa.
	 *
	 * @return	Nowe poddrzewo.
	 */

	virtual std::shared_ptr<TUQuadTree > createSubTree( UInt x, UInt y, UInt size )
	{
		return std::make_shared<TUQuadTree>( itsParentCu, x, y, size );
	}

	/**
	 * @fn	virtual Void TUQuadTree::printDescription( );
	 *
	 * @brief	Wypisuje opis struktury drzewa.
	 *
	 * @return	Void.
	 */

	virtual Void printDescription( );

	/**
	 * @fn	Void TUQuadTree::reconstructionLoop( std::shared_ptr<PUIntra> mainPU );
	 *
	 * @brief	Pêtla rekonstrukcji.
	 *
	 * @param	mainPU	Nadrzêdna jednostka predykcji.
	 *
	 * @return	Void.
	 */

	Void reconstructionLoop( std::shared_ptr<PUIntra> mainPU );

	/**
	 * @fn	std::shared_ptr<TU> TUQuadTree::getTuContainingPosition( UInt x, UInt y );
	 *
	 * @brief	Zwraca jednostkê transformacji zawieraj¹c¹ podan¹ próbkê luminancji.
	 *
	 * @param	x	Wspó³rzêdna X próbki luminancji.
	 * @param	y	Wspó³rzêdna Y próbki luminancji.
	 *
	 * @return	TU obejmuj¹ca podan¹ próbkê.
	 */

	std::shared_ptr<TU> getTuContainingPosition( UInt x, UInt y );
};
#endif