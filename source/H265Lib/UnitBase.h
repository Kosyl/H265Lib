/**
 * @file	UnitBase.h
 *
 * @brief	Deklaracja klas bazowych dla jednostek i bloków.
 */

#ifndef UnitBase_h__
#define UnitBase_h__

#include "Utils.h"
#include "Picture.h"

/**
 * @class	UnitBase
 *
 * @brief	Bazowa klasa dla kwadratowych fragmentów obrazu.
 */

class UnitBase
{
protected:

	/**
	 * @brief	Indeks Z-skanowania tej jednostki.
	 */

	UInt itsIdx;

	/**
	 * @brief	Rozmiar jednostki.
	 */

	UShort itsSize;

	/**
	 * @property	UShort itsX, itsY
	 *
	 * @brief	Wspó³rzêdne jednostki w obrazie.
	 */

	UShort itsX, itsY;

	/**
	 * @brief	WskaŸnik na obraz bazowy.
	 */

	Picture* itsParentPicture;

public:

	/**
	 * @fn	UnitBase::UnitBase( Picture* picture, UShort x, UShort y, UShort size )
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in,out]	picture	WskaŸnik na obraz bazowy.
	 * @param	x								 	Wspó³rzêdna X danego fragmentu.
	 * @param	y								 	Wspó³rzêdna Y danego fragmentu.
	 * @param	size						 	Rozmiar fragmentu.
	 */

	UnitBase( Picture* picture, UShort x, UShort y, UShort size ) :
		itsX( x ),
		itsY( y ),
		itsIdx( getZScanIdxIn4x4( x, y ) ),
		itsSize( size )
	{
		assert
			(
			//picture != nullptr &&
			x < SeqParams( )->getPicWidth( ) &&
			x % SeqParams( )->getMinTUSize( ) == 0 &&
			y < SeqParams( )->getPicHeight( ) &&
			y % SeqParams( )->getMinTUSize( ) == 0 &&
			size > 0 && size % 4 == 0
			);
		itsParentPicture = picture;
	}

	/**
	 * @fn	UnitBase::~UnitBase( )
	 *
	 * @brief	Destruktor.
	 */

	~UnitBase( )
	{
	}

#pragma region Akcesory

	/**
	 * @fn	UShort UnitBase::getY( ) const
	 *
	 * @brief Dostêp do wspó³rzêdnej Y.
	 */

	UShort getY( ) const
	{
		return itsY;
	}

	/**
	 * @fn	UShort UnitBase::getX( ) const
	 *
	 * @brief	Dostêp do wspó³rzêdnej X.
	 */

	UShort getX( ) const
	{
		return itsX;
	}

	/**
	 * @fn	UShort UnitBase::getSize( ) const
	 *
	 * @brief	Dostêp do rozmiaru boku.
	 */

	UShort getSize( ) const
	{
		return itsSize;
	}

	/**
	 * @fn	UInt UnitBase::getIdx( ) const
	 *
	 * @brief	Dostêp do indeksu porz¹dkowego.
	 */

	UInt getIdx( ) const
	{
		return itsIdx;
	}

	/**
	 * @fn	Void UnitBase::setIdx( UInt val )
	 *
	 * @brief	Ustawienie indeksu porz¹dkowego.
	 *
	 * @param	val	Nowy indeks.
	 */

	Void setIdx( UInt val )
	{
		itsIdx = val;
	}

	/**
	 * @fn	Picture* UnitBase::getPicture( )
	 *
	 * @brief	Dostêp do aktualnego obrazu. 
	 */

	Picture* getPicture( )
	{
		return itsParentPicture;
	}

#pragma endregion

	/**
	 * @fn	virtual Void UnitBase::printDescription( )
	 *
	 * @brief	Wypisanie opisu elementu.
	 */

	virtual Void printDescription(  )
	{
	}
};

/**
 * @class	CTUComponentBase
 *
 * @brief	Klasa bazowa dla podobiektów znajduj¹cych siê wewn¹trz CTU.
 */

class CTUComponentBase : public UnitBase
{
protected:

	/**
	 * @brief	CTU, w której komponent siê znajduje.
	 */

	CTU* itsParentCTU;

public:

	/**
	 * @fn	CTUComponentBase::CTUComponentBase( Picture* picture, CTU* ctu, UShort x, UShort y, UShort size )
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	picture	Obraz bazowy.
	 * @param [in]	ctu		 	WskaŸnik na ba
	 * @param	x							Wspó³rzêdna X komponentu.
	 * @param	y							Wspó³rzêdna Y komponentu.
	 * @param	size					Rozmiar komponentu.
	 */

	CTUComponentBase( Picture* picture, CTU* ctu, UShort x, UShort y, UShort size ):
		UnitBase(picture, x, y, size ),
		itsParentCTU(ctu)
	{
		//assert( ctu != nullptr );
	}

	/**
	 * @fn	CTU* CTUComponentBase::getParentCTU( )
	 *
	 * @brief	Dostêp do bazowego CTU.
	 */

	CTU* getParentCTU( )
	{
		return itsParentCTU;
	}

	/**
	 * @fn	CTUComponentBase::~CTUComponentBase( )
	 *
	 * @brief	Destruktor.
	 */

	~CTUComponentBase( )
	{
	}

};

#endif // UnitBase_h__