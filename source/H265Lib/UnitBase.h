/**
 * @file	UnitBase.h
 *
 * @brief	Deklaracja klas bazowych dla jednostek i blok�w.
 */

#ifndef UnitBase_h__
#define UnitBase_h__

#include "Utils.h"
#include "Picture.h"

/**
 * @class	UnitBase
 *
 * @brief	Bazowa klasa dla kwadratowych fragment�w obrazu.
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
	 * @brief	Wsp�rz�dne jednostki w obrazie.
	 */

	UShort itsX, itsY;

	/**
	 * @brief	Wska�nik na obraz bazowy.
	 */

	Picture* itsParentPicture;

public:

	/**
	 * @fn	UnitBase::UnitBase( Picture* picture, UShort x, UShort y, UShort size )
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in,out]	picture	Wska�nik na obraz bazowy.
	 * @param	x								 	Wsp�rz�dna X danego fragmentu.
	 * @param	y								 	Wsp�rz�dna Y danego fragmentu.
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
	 * @brief Dost�p do wsp�rz�dnej Y.
	 */

	UShort getY( ) const
	{
		return itsY;
	}

	/**
	 * @fn	UShort UnitBase::getX( ) const
	 *
	 * @brief	Dost�p do wsp�rz�dnej X.
	 */

	UShort getX( ) const
	{
		return itsX;
	}

	/**
	 * @fn	UShort UnitBase::getSize( ) const
	 *
	 * @brief	Dost�p do rozmiaru boku.
	 */

	UShort getSize( ) const
	{
		return itsSize;
	}

	/**
	 * @fn	UInt UnitBase::getIdx( ) const
	 *
	 * @brief	Dost�p do indeksu porz�dkowego.
	 */

	UInt getIdx( ) const
	{
		return itsIdx;
	}

	/**
	 * @fn	Void UnitBase::setIdx( UInt val )
	 *
	 * @brief	Ustawienie indeksu porz�dkowego.
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
	 * @brief	Dost�p do aktualnego obrazu. 
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
 * @brief	Klasa bazowa dla podobiekt�w znajduj�cych si� wewn�trz CTU.
 */

class CTUComponentBase : public UnitBase
{
protected:

	/**
	 * @brief	CTU, w kt�rej komponent si� znajduje.
	 */

	CTU* itsParentCTU;

public:

	/**
	 * @fn	CTUComponentBase::CTUComponentBase( Picture* picture, CTU* ctu, UShort x, UShort y, UShort size )
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	picture	Obraz bazowy.
	 * @param [in]	ctu		 	Wska�nik na ba
	 * @param	x							Wsp�rz�dna X komponentu.
	 * @param	y							Wsp�rz�dna Y komponentu.
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
	 * @brief	Dost�p do bazowego CTU.
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