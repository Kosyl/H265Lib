/**
 * @file	CU.h
 *
 * @brief	Deklaracje klas jednostek kodowych
 */

#ifndef CU_H
#define CU_H

#include <cassert>
#include <vector>
#include "Utils.h"
#include "Picture.h"
#include "UnitBase.h"

class PUIntra;
class TUQuadTree;

/**
 * @class	CU
 *
 * @brief	Klasa jednostki kodowej
 */

class CU : public CTUComponentBase
{
protected:

	/**
	 * @brief	Bufor predykcji.
	 */

	Sample*** itsPredictions;

	/**
	 * @brief	Zmiana QP wprowadzana dla tego CU wzgl�dem plastra.
	 */

	Short itsQPDeltaForCU;

	/**
	 * @brief	Flaga pomijania skalowania, transformaty i filtru deblokuj�cego.
	 */

	Bool itsTransQuantBypassEnabled;

	/**
	 * @brief	Typ predykcji intra/inter.
	 */

	PredictionType itsPredictionType;

	/**
	 * @brief	Tryb podzia�u na jednostki predykcji.
	 */

	PartitionMode itsPartitionMode;

	/**
	 * @brief	Drzewo transformacji.
	 */

	std::shared_ptr<TUQuadTree> itsTransformTree;
	
public:

	/**
	 * @fn	CU::CU( CTU* ctu, UShort x, UShort y, UShort size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	ctu	Wska�nik na bazowe CTU.
	 * @param	x						 	Wsp�rz�dna X jednostki.
	 * @param	y						 	Wsp�rz�dna Y jednostki.
	 * @param	size				 	Rozmiar jednostki.
	 */

	CU( CTU* ctu, UShort x, UShort y, UShort size );

	/**
	 * @fn	virtual CU::~CU( );
	 *
	 * @brief	Destruktor.
	 */

	virtual ~CU( );

#pragma region Akcesory

	/**
	 * @fn	std::shared_ptr<TUQuadTree> CU::getTransformTree( ) const
	 *
	 * @brief Zwraca korze� drzewa transformacji.
	 *
	 * @return	Korze� drzewa transformacji.
	 */

	std::shared_ptr<TUQuadTree> getTransformTree( ) const
	{
		return itsTransformTree;
	}

	/**
	 * @fn	Void CU::setTransformTree( std::shared_ptr<TUQuadTree> val )
	 *
	 * @brief	Ustawia drzewo transformacji tej jednostki CU.
	 *
	 * @param	val	Wska�nik na nowe drzewo transformacji.
	 *
	 * @return	Void.
	 */

	Void setTransformTree( std::shared_ptr<TUQuadTree> val )
	{
		itsTransformTree = val;
	}

	/**
	 * @fn	PartitionMode CU::getPartitionMode( ) const
	 *
	 * @brief	Zwraca tryb podzia�u na jednostk predykcji.
	 *
	 * @return	Tryb podzia�u na jednostk predykcji.
	 */

	PartitionMode getPartitionMode( ) const
	{
		return itsPartitionMode;
	}

	/**
	 * @fn	virtual Void CU::setPartitionMode( PartitionMode val )
	 *
	 * @brief	Ustawia tryb podzia�u na jednostk predykcji.
	 *
	 * @param	val	Nowy tryb podzia�u na jednostk predykcji.
	 *
	 * @return	Void.
	 */

	virtual Void setPartitionMode( PartitionMode val )
	{
		itsPartitionMode = val;
	}

	/**
	 * @fn	Sample** CU::getPredictionMatrix( ImgComp comp )
	 *
	 * @brief	Zwraca wska�nik na macierz predykcji danej sk��dowej obrazu.
	 *
	 * @param	comp	Sk�adowa obrazu.
	 *
	 * @return	Wska�nik na maceirz predykcji.
	 */

	Sample** getPredictionMatrix( ImgComp comp )
	{
		return itsPredictions[ comp ];
	}

	/**
	 * @fn	PredictionType CU::getPredictionType( ) const
	 *
	 * @brief	Zwraca tryb predykcji.
	 *
	 * @return	Tryb predykcji.
	 */

	PredictionType getPredictionType( ) const
	{
		return itsPredictionType;
	}

	/**
	 * @fn	Short CU::getAbsoluteQP( );
	 *
	 * @brief	Zwraca wypadkowe QP dla CU.
	 *
	 * @return	Wypadkowe QP dla CU.
	 */

	Short getAbsoluteQP( );

	/**
	 * @fn	Bool CU::getTransQuantBypassEnabled( ) const
	 *
	 * @brief	Zwraca flag� trybu TransQuantBypass.
	 *
	 * @return	Flag� trybu TransQuantBypass.
	 */

	Bool getTransQuantBypassEnabled( ) const
	{
		return itsTransQuantBypassEnabled;
	}

	/**
	 * @fn	void CU::setTransQuantBypassEnabled( Bool val )
	 *
	 * @brief Ustawia flag� trybu TransQuantBypass.
	 *
	 * @param	val	Nowa flaga trybu TransQuantBypass.
	 */

	void setTransQuantBypassEnabled( Bool val )
	{
		itsTransQuantBypassEnabled = val;
	}
#pragma endregion

	/**
	 * @fn	virtual Void CU::printDescription( );
	 *
	 * @brief	Wypisuje opis struktury CU.
	 *
	 * @return Void.
	 */

	virtual Void printDescription( );

	/**
	 * @fn	virtual Void CU::reconstructionLoop( ) = 0;
	 *
	 * @brief	P�tla rekonstrukcji dla jednostki.
	 *
	 * @return	Void.
	 */

	virtual Void reconstructionLoop( ) = 0;

};

/**
 * @class	CUIntra
 *
 * @brief	Jednostka kodowa trybu intra.
 */

class CUIntra : public CU
{
private:

	/**
	 * @brief	Tablica najbardziej prawdopodobnych tryb�w predykcji intra.
	 */

	UShort itsIntraMPMs[ 3 ];

	/**
	 * @brief	Podjednostki predykcji.
	 */

	std::shared_ptr<PUIntra> itsPUs[ 4 ];

	/**
	 * @brief Numer trybu doboru predykcji chrominancji.
	 */

	UShort itsChromaPredictionDerivationType;
	
public:

	/**
	 * @fn	CUIntra::CUIntra( CTU* ctu, UShort x, UShort y, UShort size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	ctu	nadrz�dne CTU.
	 * @param	x						 	Wsp�rz�dna X jednostki.
	 * @param	y						 	Wsp�rz�dna Y jednostki.
	 * @param	size				 	Rozmiar jednostki.
	 */

	CUIntra( CTU* ctu, UShort x, UShort y, UShort size );

	/**
	 * @fn	virtual CUIntra::~CUIntra( );
	 *
	 * @brief	Destruktor.
	 */

	virtual ~CUIntra( );

#pragma region Akcesory

	/**
	 * @fn	UShort CUIntra::getIntraChromaPredictionDerivationType( ) const
	 *
	 * @brief	Zwraca tryb wyboru predykcji chrominancji.
	 *
	 * @return	Tryb wyboru predykcji chrominancji.
	 */

	UShort getIntraChromaPredictionDerivationType( ) const
	{
		return itsChromaPredictionDerivationType;
	}

	/**
	 * @fn	void CUIntra::setIntraChromaPredictionDerivationType( UShort val );
	 *
	 * @brief	Ustawia tryb wyboru predykcji chrominancji.
	 *
	 * @param	val	Nowy tryb wyboru predykcji chrominancji.
	 */

	void setIntraChromaPredictionDerivationType( UShort val );

	/**
	 * @fn	virtual Void CUIntra::setPartitionMode( PartitionMode val )
	 *
	 * @brief Ustawia tryb podzia�u na jednostki predykcji i tworzy je.
	 *
	 * @param	val	Tryb podiza�u.
	 *
	 * @return	Void.
	 */

	virtual Void setPartitionMode( PartitionMode val )
	{
		assert( val == PART_2Nx2N || val == PART_NxN );
		itsPartitionMode = val;
		for( UInt i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i )
		{
			if( itsPUs[ i ] != nullptr )
				itsPUs[ i ].reset( );
		}
	}

	/**
	 * @fn	Void CUIntra::addPU( std::shared_ptr<PUIntra> newPU, QTComponent position = QTCOMPONENT_FIRST )
	 *
	 * @brief	Dodaje now� jednostke predykcji na zadanej pozycji.
	 *
	 * @param	newPU			Nowa jednostka predykcji.
	 * @param	position	Pozycja nowej jednostki predykcji w CU.
	 *
	 * @return	Void.
	 */

	Void addPU( std::shared_ptr<PUIntra> newPU, QTComponent position = QTCOMPONENT_FIRST )
	{
		itsPUs[ position ] = newPU;
	}

	/**
	 * @fn	std::shared_ptr<PUIntra> CUIntra::getPU( QTComponent position = QTCOMPONENT_FIRST )
	 *
	 * @brief	Zwraca PU na danej pozycji.
	 *
	 * @param	position	 Pozycja PU w jednostce kodowej.
	 *
	 * @return	Wska�nik na jednostk� predykcji.
	 */

	std::shared_ptr<PUIntra> getPU( QTComponent position = QTCOMPONENT_FIRST )
	{
		return itsPUs[ position ];
	}

	/**
	 * @fn	Void CUIntra::resetPUs( )
	 *
	 * @brief	Usuwa wszystkie PU.
	 *
	 * @return Void.
	 */

	Void resetPUs( )
	{
		for( Int i = 0; i < 4; ++i )
			itsPUs[ i ] = nullptr;
	}

#pragma endregion

	/**
	 * @fn	Void CUIntra::createPUs( UInt lumaModeIdx = 0 );
	 *
	 * @brief	Tworzy jednostki predykcji w zadanym trybie predykcji intra.
	 *
	 * @param	lumaModeIdx	Tryb predykcji intra..
	 *
	 * @return	Void.
	 */

	Void createPUs( UInt lumaModeIdx = 0 );

	/**
	 * @fn	virtual Void CUIntra::printDescription( );
	 *
	 * @brief	Wypisuje opis struktury CU.
	 *
	 * @return	Void.
	 */

	virtual Void printDescription( );

	/**
	 * @fn	virtual Void CUIntra::reconstructionLoop( );
	 *
	 * @brief	Przeprowadza p�tl� rekonstrukcji na podjednostkach.
	 *
	 * @return	Void.
	 */

	virtual Void reconstructionLoop( );

	/**
	 * @fn	Double CUIntra::getTotalCost( );
	 *
	 * @brief Zwraca sumaryczny koszt zakodowania ca�ego drzewa transformacji.
	 *
	 * @return	Koszt zakodowania ca�ego drzewa transformacji.
	 */

	Double getTotalCost( );

};

#endif