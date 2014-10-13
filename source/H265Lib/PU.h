/**
* @file	PU.h
*
* @brief	Deklaracja klas bloku i jednostki predykcji
*/
#ifndef PB_H
#define PB_H

#include <cassert>
#include "Utils.h"
#include "CU.h"
#include "UnitBase.h"

class PUIntra;
class TB;

/**
 * @class	PBIntra
 *
 * @brief	Klasa bloku predykcji intra.
 */

class PBIntra : public UnitBase
{
private:

	/**
	 * @brief Nadrz�dny blok transformacji.
	 */

	TB *itsParentTB;

	/**
	 * @brief	Sk�aowa obrazu predykowana przez ten blok.
	 */

	ImgComp itsComp;

	/**
	 * @property	Sample const* const *itsPicRecon
	 *
	 * @brief	Wska�nik na macierz rekonstrukcji.
	 *
	 */

	Sample const* const *itsPicRecon;

	/**
	 * @brief	Wska�nik na macierz predykcji.
	 */

	Sample** itsPredictionTarget;

	/**
	 * @brief	Pr�bki referencyjne z lewej i powy�ej tego bloku.
	 */

	Sample** itsSideReferences;

	/**
	 * @brief	Referencyjna pr�bka naro�na.
	 */

	Sample itsCornerReference;

	/**
	 * @brief	Flaga m�wi�ca o gotowo�ci pr�bek referencyjnych.
	 */

	Bool itsReferencesReady;

	/**
	 * @brief	Numer trybu predykcji intra.
	 */

	UInt itsModeIdx;

	/**
	 * @fn	Bool PBIntra::calcPuAvail( const Int x, const Int y) const;
	 *
	 * @brief	Sprawdza dost�pno�� jednostki predykcji pokrywajacej dany piksel luminancji.
	 *
	 * @param	x	Wsp�rz�dna X pr�bki.
	 * @param	y	Wsp�rz�dna Y pr�bki.
	 *
	 * @return	true je�li jednostka jest dost�pna; false w p.p.
	 */

	Bool calcPuAvail( const Int x, const Int y) const;

	/**
	 * @fn	UShort PBIntra::getReferenceValue( const IntraDirection dir, const UInt offset = 0 ) const;
	 *
	 * @brief	Dost�p do pr�bki referencyjnej na zadanej pozycji.
	 *
	 * @param	dir			Strona, z kt�rej pobierana jest pr�bka (lewo/g�ra).
	 * @param	offset	Przesuni�cie wzgl�dem lewego g�rnego rogu.
	 *
	 * @return	Pr�bka referencyjna.
	 */

	UShort getReferenceValue( const IntraDirection dir, const UInt offset = 0 ) const;

	/**
	 * @fn	Void PBIntra::fillMissingReferences( Bool** sideAvailable, Bool cornerAvailable );
	 *
	 * @brief	Wype�nienie brakuj�cych pr�bek referencyjnych.
	 *
	 * @param [in]	sideAvailable	Tablice dost�pno�ci pr�bek referencyjnych.
	 * @param	cornerAvailable				 	Dost�pno�� referencyjnej pr�bki naro�nej.
	 *
	 * @return	Void.
	 */

	Void fillMissingReferences( Bool** sideAvailable, Bool cornerAvailable );

	/**
	 * @brief	Flaga m�wi�ca o gotowo�ci predykcji.
	 */

	Bool itsPredictionDone;

public:

	/**
	 * @fn	PBIntra::PBIntra( TB* parentTB, ImgComp comp, UInt x, UInt y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	parentTB	Nadrz�dny blok transformacji.
	 * @param	comp								Sk�adowa obrazu.
	 * @param	x										Wsp�rz�dna X bloku predykcji.
	 * @param	y										Wsp�rz�dna Y bloku predykcji.
	 * @param	size								Rozmiar bloku.
	 */

	PBIntra( TB* parentTB, ImgComp comp, UInt x, UInt y, UInt size );

	/**
	 * @fn	PBIntra::~PBIntra( );
	 *
	 * @brief	Destruktor.
	 */

	~PBIntra( );

	/**
	 * @fn	Void PBIntra::calcAndWritePredictionToCU( std::shared_ptr<PUIntra> mainPU );
	 *
	 * @brief	Liczy predykcje i zapisuje j� w buforze predykcji.
	 *
	 * @param	mainPU	PU pokrywajaca ten PB - zawiera m.in. tryb predykcji intra.
	 *
	 */

	Void calcAndWritePredictionToCU( std::shared_ptr<PUIntra> mainPU );

	/**
	 * @fn	Void PBIntra::calcReferences( );
	 *
	 * @brief	Obliczenie pr�bek referencyjnych.
	 *
	 * @return	Void
	 */

	Void calcReferences( );

	/**
	 * @fn	ImgComp PBIntra::getImgComp( ) const;
	 *
	 * @brief Informacja o sk�adowej obrazu tego PB.
	 *
	 * @return	Sk�adowa obrazu.
	 */

	ImgComp getImgComp( ) const;

	/**
	 * @fn	UInt PBIntra::getModeIdx( ) const;
	 *
	 * @brief	Dost�p do trybu predykcji intra.
	 *
	 * @return	Numer trybu predykcji.
	 */

	UInt getModeIdx( ) const;

	/**
	 * @fn	UInt PBIntra::getPUIdx( ) const;
	 *
	 * @brief	Indeks Z-skanowania tego PB.
	 *
	 * @return	Indeks PB.
	 */

	UInt getPUIdx( ) const;

	/**
	 * @fn	Void PBIntra::setModeIdx( UInt mode );
	 *
	 * @brief	Ustawia tryb predykcji intra.
	 *
	 * @param	mode	Nowy tyrb predykcji intra.
	 *
	 * @return	Void.
	 */

	Void setModeIdx( UInt mode );

	/**
	 * @fn	Sample PBIntra::getCornerReference( );
	 *
	 * @brief	Dost�p do referencyjnej pr�bki naro�nej.
	 *
	 * @return	Wartos� pr�bki.
	 */

	Sample getCornerReference( );

	/**
	 * @fn	Sample* PBIntra::getSideRefs( const IntraDirection );
	 *
	 * @brief	Dost�p do tablicy pr�bek referencyjnych.
	 *
	 * @param	IntraDirection	Po�o�enie po��danych pr�bek.
	 *
	 * @return	Tablic� pr�bek referencyjnych.
	 */

	Sample* getSideRefs( const IntraDirection );

	/**
	 * @fn	Sample PBIntra::**getPred( );
	 *
	 * @brief	Obliczenie sygna�u predykcji.
	 *
	 * @return	Macierz sygna�u predykcji.
	 */

	Sample **getPred( );

	/**
	 * @fn	Sample PBIntra::**getPredForceRef( Sample* leftRefs, Sample* topRefs, Sample corner );
	 *
	 * @brief	Policzenie predykcji z wymuszeniem pr�bek referencyjnych.
	 *
	 * @param [in]	leftRefs	Pr�bki referencyjne z lewej.
	 * @param [in]	topRefs 	Pr�bki referencyjne u g�ry.
	 * @param	corner							Naro�na pr�bka referencyjna.
	 *
	 * @return	Macierz sygna�u predykcji.
	 */

	Sample **getPredForceRef( Sample* leftRefs, Sample* topRefs, Sample corner );

	/**
	 * @fn	Bool PBIntra::predictionDone( )
	 *
	 * @brief	Sprawdza, czy predykcja zosta�a ju� policzona.
	 *
	 * @return	true, jesli predykcja jest gotowa; false w p.p.
	 */

	Bool predictionDone( )
	{
		return itsPredictionDone;
	}

	/**
	 * @fn	virtual Void PBIntra::printDescription( );
	 *
	 * @brief	Wypisuje struktur� bloku PB.
	 *
	 * @return	Void.
	 */

	virtual Void printDescription( );
};

/**
 * @class	PUIntra
 *
 * @brief Klasa jednostki predykcji
 */

class PUIntra : public CTUComponentBase, public std::enable_shared_from_this<PUIntra>
{
private:

	/**
	 * @brief	Nadrz�dna jednostka CU.
	 */

	CUIntra* itsParentCU;

	/**
	 * @property	UInt itsLumaModeIdx, itsChromaModeIdx
	 *
	 * @brief	Numery tryb�w predykcji intra dla luminancji i chrominancji.
	 *
	 */

	UInt itsLumaModeIdx, itsChromaModeIdx;

	/**
	 * @brief	Podleg�y fragment drzewa transformacji.
	 */

	std::shared_ptr<TUQuadTree> itsTransformArea;

public:

	/**
	 * @fn	PUIntra::PUIntra( CUIntra* newCU, UInt X, UInt Y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	Nadrz�dne CU.
	 * @param	X							 	Wsp�rz�dna X jednostki.
	 * @param	Y							 	Wsp�rz�dna Y jednostki.
	 * @param	size					  Rozmiar jednostki.
	 */

	PUIntra( CUIntra* newCU, UInt X, UInt Y, UInt size );

	/**
	 * @fn	PUIntra::~PUIntra( );
	 *
	 * @brief	Destruktor.
	 */

	~PUIntra( );

	/**
	 * @fn	Void PUIntra::setLumaModeIdx( const UInt modeIdx )
	 *
	 * @brief	Ustawia numer trybu predykcji intra dla luminancji.
	 *
	 * @param	modeIdx	Numer trybu predykcji intra dla luminancji
	 *
	 * @return	Void.
	 */

	Void setLumaModeIdx( const UInt modeIdx )
	{
		assert( modeIdx >= 0 && modeIdx <= 34 );
		itsLumaModeIdx = modeIdx;
		refreshChromaModeIdx( );
	}

	/**
	 * @fn	Void PUIntra::refreshChromaModeIdx( )
	 *
	 * @brief	Od�wie�a tryb predykcji chrominancji po zmianie trybu dla luminancji.
	 *
	 * @return	Void.
	 */

	Void refreshChromaModeIdx( )
	{
		itsChromaModeIdx = PUIntra::getModeForChroma( itsLumaModeIdx, itsParentCU->getIntraChromaPredictionDerivationType( ) );
	}

	/**
	 * @fn	virtual CUIntra* PUIntra::getCu( ) const
	 *
	 * @brief	Dost�p do nadrz�dnej jednostki CU.
	 *
	 * @return	Wska�nik na CU.
	 */

	virtual CUIntra* getCu( ) const
	{
		return itsParentCU;
	}

	/**
	 * @fn	UInt PUIntra::getModeIdx( ImgComp comp ) const
	 *
	 * @brief	Zwraca numer trybu predykcji dla danej sk�adowej obrazu.
	 *
	 * @param	comp	Sk�adowa obrazu.
	 *
	 * @return	Numer trybu predykcji.
	 */

	UInt getModeIdx( ImgComp comp ) const
	{
		if( comp == LUMA )
			return itsLumaModeIdx;
		else
			return itsChromaModeIdx;
	}

	/**
	 * @fn	std::shared_ptr<TUQuadTree> PUIntra::getTransformArea( ) const
	 *
	 * @brief	Zwraca podleg�y obszar drzewa transformacji.
	 *
	 * @return	Obszar drzewa transformacji.
	 */

	std::shared_ptr<TUQuadTree> getTransformArea( ) const
	{
		return itsTransformArea;
	}

	/**
	 * @fn	Void PUIntra::setTransformArea( std::shared_ptr<TUQuadTree> val )
	 *
	 * @brief	Ustawia podleg�y obszar drzewa transformacji.
	 *
	 * @param	val	Obszar drzewa transformacji.
	 *
	 * @return	Void.
	 */

	Void setTransformArea( std::shared_ptr<TUQuadTree> val )
	{
		itsTransformArea = val;
	}

	/**
	 * @fn	Void PUIntra::reconstructionLoop( );
	 *
	 * @brief	P�tla rekonstrukcji.
	 *
	 * @return	Void.
	 */

	Void reconstructionLoop( );

	/**
	 * @fn	static UInt PUIntra::getModeForChroma( UInt modeForLuma, UInt chromaPredictionDerivationMode );
	 *
	 * @brief	Obliczenie trybu predykcji intra dla chrominancji.
	 *
	 * @param	modeForLuma											Tryb predykcji dla luminancji.
	 * @param	chromaPredictionDerivationMode	Numer sposobu doboru trybu chrominancji.
	 *
	 * @return	Numer trybu predykcji chrominancji.
	 */

	static UInt getModeForChroma( UInt modeForLuma, UInt chromaPredictionDerivationMode );

	/**
	 * @fn	virtual Void PUIntra::printDescription( );
	 *
	 * @brief	Wypisuje opis struktury podzia�u.
	 *
	 * @return	Void.
	 */

	virtual Void printDescription( );
};

#endif