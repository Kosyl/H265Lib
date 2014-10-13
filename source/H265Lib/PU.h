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
	 * @brief Nadrzêdny blok transformacji.
	 */

	TB *itsParentTB;

	/**
	 * @brief	Sk³aowa obrazu predykowana przez ten blok.
	 */

	ImgComp itsComp;

	/**
	 * @property	Sample const* const *itsPicRecon
	 *
	 * @brief	WskaŸnik na macierz rekonstrukcji.
	 *
	 */

	Sample const* const *itsPicRecon;

	/**
	 * @brief	WskaŸnik na macierz predykcji.
	 */

	Sample** itsPredictionTarget;

	/**
	 * @brief	Próbki referencyjne z lewej i powy¿ej tego bloku.
	 */

	Sample** itsSideReferences;

	/**
	 * @brief	Referencyjna próbka naro¿na.
	 */

	Sample itsCornerReference;

	/**
	 * @brief	Flaga mówi¹ca o gotowoœci próbek referencyjnych.
	 */

	Bool itsReferencesReady;

	/**
	 * @brief	Numer trybu predykcji intra.
	 */

	UInt itsModeIdx;

	/**
	 * @fn	Bool PBIntra::calcPuAvail( const Int x, const Int y) const;
	 *
	 * @brief	Sprawdza dostêpnoœæ jednostki predykcji pokrywajacej dany piksel luminancji.
	 *
	 * @param	x	Wspó³rzêdna X próbki.
	 * @param	y	Wspó³rzêdna Y próbki.
	 *
	 * @return	true jeœli jednostka jest dostêpna; false w p.p.
	 */

	Bool calcPuAvail( const Int x, const Int y) const;

	/**
	 * @fn	UShort PBIntra::getReferenceValue( const IntraDirection dir, const UInt offset = 0 ) const;
	 *
	 * @brief	Dostêp do próbki referencyjnej na zadanej pozycji.
	 *
	 * @param	dir			Strona, z której pobierana jest próbka (lewo/góra).
	 * @param	offset	Przesuniêcie wzglêdem lewego górnego rogu.
	 *
	 * @return	Próbka referencyjna.
	 */

	UShort getReferenceValue( const IntraDirection dir, const UInt offset = 0 ) const;

	/**
	 * @fn	Void PBIntra::fillMissingReferences( Bool** sideAvailable, Bool cornerAvailable );
	 *
	 * @brief	Wype³nienie brakuj¹cych próbek referencyjnych.
	 *
	 * @param [in]	sideAvailable	Tablice dostêpnoœci próbek referencyjnych.
	 * @param	cornerAvailable				 	Dostêpnoœæ referencyjnej próbki naro¿nej.
	 *
	 * @return	Void.
	 */

	Void fillMissingReferences( Bool** sideAvailable, Bool cornerAvailable );

	/**
	 * @brief	Flaga mówi¹ca o gotowoœci predykcji.
	 */

	Bool itsPredictionDone;

public:

	/**
	 * @fn	PBIntra::PBIntra( TB* parentTB, ImgComp comp, UInt x, UInt y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	parentTB	Nadrzêdny blok transformacji.
	 * @param	comp								Sk³adowa obrazu.
	 * @param	x										Wspó³rzêdna X bloku predykcji.
	 * @param	y										Wspó³rzêdna Y bloku predykcji.
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
	 * @brief	Liczy predykcje i zapisuje j¹ w buforze predykcji.
	 *
	 * @param	mainPU	PU pokrywajaca ten PB - zawiera m.in. tryb predykcji intra.
	 *
	 */

	Void calcAndWritePredictionToCU( std::shared_ptr<PUIntra> mainPU );

	/**
	 * @fn	Void PBIntra::calcReferences( );
	 *
	 * @brief	Obliczenie próbek referencyjnych.
	 *
	 * @return	Void
	 */

	Void calcReferences( );

	/**
	 * @fn	ImgComp PBIntra::getImgComp( ) const;
	 *
	 * @brief Informacja o sk³adowej obrazu tego PB.
	 *
	 * @return	Sk³adowa obrazu.
	 */

	ImgComp getImgComp( ) const;

	/**
	 * @fn	UInt PBIntra::getModeIdx( ) const;
	 *
	 * @brief	Dostêp do trybu predykcji intra.
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
	 * @brief	Dostêp do referencyjnej próbki naro¿nej.
	 *
	 * @return	Wartosæ próbki.
	 */

	Sample getCornerReference( );

	/**
	 * @fn	Sample* PBIntra::getSideRefs( const IntraDirection );
	 *
	 * @brief	Dostêp do tablicy próbek referencyjnych.
	 *
	 * @param	IntraDirection	Po³o¿enie po¿¹danych próbek.
	 *
	 * @return	Tablicê próbek referencyjnych.
	 */

	Sample* getSideRefs( const IntraDirection );

	/**
	 * @fn	Sample PBIntra::**getPred( );
	 *
	 * @brief	Obliczenie sygna³u predykcji.
	 *
	 * @return	Macierz sygna³u predykcji.
	 */

	Sample **getPred( );

	/**
	 * @fn	Sample PBIntra::**getPredForceRef( Sample* leftRefs, Sample* topRefs, Sample corner );
	 *
	 * @brief	Policzenie predykcji z wymuszeniem próbek referencyjnych.
	 *
	 * @param [in]	leftRefs	Próbki referencyjne z lewej.
	 * @param [in]	topRefs 	Próbki referencyjne u góry.
	 * @param	corner							Naro¿na próbka referencyjna.
	 *
	 * @return	Macierz sygna³u predykcji.
	 */

	Sample **getPredForceRef( Sample* leftRefs, Sample* topRefs, Sample corner );

	/**
	 * @fn	Bool PBIntra::predictionDone( )
	 *
	 * @brief	Sprawdza, czy predykcja zosta³a ju¿ policzona.
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
	 * @brief	Wypisuje strukturê bloku PB.
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
	 * @brief	Nadrzêdna jednostka CU.
	 */

	CUIntra* itsParentCU;

	/**
	 * @property	UInt itsLumaModeIdx, itsChromaModeIdx
	 *
	 * @brief	Numery trybów predykcji intra dla luminancji i chrominancji.
	 *
	 */

	UInt itsLumaModeIdx, itsChromaModeIdx;

	/**
	 * @brief	Podleg³y fragment drzewa transformacji.
	 */

	std::shared_ptr<TUQuadTree> itsTransformArea;

public:

	/**
	 * @fn	PUIntra::PUIntra( CUIntra* newCU, UInt X, UInt Y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	Nadrzêdne CU.
	 * @param	X							 	Wspó³rzêdna X jednostki.
	 * @param	Y							 	Wspó³rzêdna Y jednostki.
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
	 * @brief	Odœwie¿a tryb predykcji chrominancji po zmianie trybu dla luminancji.
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
	 * @brief	Dostêp do nadrzêdnej jednostki CU.
	 *
	 * @return	WskaŸnik na CU.
	 */

	virtual CUIntra* getCu( ) const
	{
		return itsParentCU;
	}

	/**
	 * @fn	UInt PUIntra::getModeIdx( ImgComp comp ) const
	 *
	 * @brief	Zwraca numer trybu predykcji dla danej sk³adowej obrazu.
	 *
	 * @param	comp	Sk³adowa obrazu.
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
	 * @brief	Zwraca podleg³y obszar drzewa transformacji.
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
	 * @brief	Ustawia podleg³y obszar drzewa transformacji.
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
	 * @brief	Pêtla rekonstrukcji.
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
	 * @brief	Wypisuje opis struktury podzia³u.
	 *
	 * @return	Void.
	 */

	virtual Void printDescription( );
};

#endif