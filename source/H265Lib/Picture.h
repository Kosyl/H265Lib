/**
* @file	Picture.h
*
* @brief	Deklaracja klasy obrazu (ramki)
*/
#ifndef _PICTURE_H_
#define _PICTURE_H_

#include "TypeDef.h"
#include "Logger.h"
#include "Utils.h"
#include "SeqPicParameters.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string.h>

class Tile;
class CTU;
class CU;
class Slice;
class PartitioningStrategy;

/**
 * @class	Picture
 *
 * @brief	Deklaracja klasy obrazu.
 */

class Picture
{
protected:

	/**
	 * @brief	Bufor oryginalnych próbek luminancji.
	 */

	Sample** itsSamplesY;

	/**
	 * @brief	Bufor oryginalnych próbek chrominancji Cb.
	 */

	Sample** itsSamplesCb;

	/**
	 * @brief	Bufor oryginalnych próbek chrominancji Cr.
	 */

	Sample** itsSamplesCr;

	/**
	 * @brief	Bufor zrekonstruowanych próbek luminancji.
	 */

	Sample** itsReconY;

	/**
	 * @brief	Bufor zrekonstruowanych próbek chrominancji Cb.
	 */

	Sample** itsReconCb;

	/**
	 * @brief	Bufor zrekonstruowanych próbek chrominancji Cr.
	 */

	Sample** itsReconCr;

	/**
	 * @property	std::vector<std::shared_ptr<Tile> > itsTiles
	 *
	 * @brief	Lista kafli w obrêbie obrazu.
	 *
	 */

	std::vector<std::shared_ptr<Tile> > itsTiles;

	/**
	 * @property	std::vector<std::shared_ptr<Slice> > itsSlices
	 *
	 * @brief	Lista plastrów w obrêbie obrazu.
	 *
	 */

	std::vector<std::shared_ptr<Slice> > itsSlices;

	/**
	 * @property	std::vector<std::shared_ptr<CTU> > itsCTUs
	 *
	 * @brief	Lista CTU w obrêbie obrazu.
	 *
	 */

	std::vector<std::shared_ptr<CTU> > itsCTUs;

	/**
	 * @brief	Aktualny (np. aktualnie przetwarzany) kafel.
	 */

	std::shared_ptr<Tile> itsCurrentTile;

	/**
	 * @brief	Aktualny (np. aktualnie przetwarzany) plaster.
	 */

	std::shared_ptr<Slice> itsCurrentSlice;

	/**
	 * @brief	Aktualna (np. aktualnie przetwarzana) jednostka CTU.
	 */

	std::shared_ptr<CTU> itsCurrentCTU;

	/**
	 * @fn	Void Picture::deleteMatrices( );
	 *
	 * @brief	Skasowanie buforów próbek.
	 *
	 * @return	Void.
	 */

	Void deleteMatrices( );

	/**
	 * @fn	Void Picture::setSize( UInt width, UInt height );
	 *
	 * @brief	Ustawia rozmiar obrazu i rezerwuje bufory próbek.
	 *
	 * @param	width 	Szerokoœæ obrazu.
	 * @param	height	D³ugoœæ obrazu.
	 *
	 * @return	Void.
	 */

	Void setSize( UInt width, UInt height );

public:

	/**
	 * @fn	Picture::Picture( );
	 *
	 * @brief Konstruktor domyœlny.
	 */

	Picture( );

	/**
	 * @fn	Picture::~Picture( );
	 *
	 * @brief	Destruktor.
	 */

	~Picture( );

	/**
	 * @fn	Void Picture::initFrame( );
	 *
	 * @brief	Nadanie rozmiaru (na podstawie SequenceParameters) i zarezerwowanie buforów.
	 *
	 * @return	Void.
	 */

	Void initFrame( );

	/**
	 * @fn	Void Picture::loadFrameFromYuv( std::ifstream& yuvStream );
	 *
	 * @brief	Wczytanie macierzy próbek oryginalncyh ze strumienia.
	 *
	 * @param [in]	yuvStream	Strumieñ do pliku yuv.
	 *
	 */

	Void loadFrameFromYuv( std::ifstream& yuvStream );

#pragma region Akcesory

	/**
	 * @fn	Sample** Picture::getSamples( const ImgComp comp ) const;
	 *
	 * @brief	Dostêp do próbek oryginalnych danej sk³adowej obrazu.
	 *
	 * @param	comp	Wybrana sk³adowa obrazu.
	 *
	 * @return	Macierz próbek oryginalnych.
	 */

	Sample** getSamples( const ImgComp comp ) const;

	/**
	 * @fn	Sample** Picture::getReconMatrix( const ImgComp comp ) const;
	 *
	 * @brief	Dostêp do próbek zrekonstruowanych danej sk³adowej obrazu.
	 *
	 * @param	comp	Wybrana sk³adowa obrazu.
	 *
	 * @return	Macierz próbek zrekonstruowanych.
	 */

	Sample** getReconMatrix( const ImgComp comp ) const;

	/**
	 * @fn	std::vector<std::shared_ptr<Tile> > & Picture::getTiles( )
	 *
	 * @brief	Zwraca listê kafli.
	 *
	 * @return	Lista kafli.
	 */

	std::vector<std::shared_ptr<Tile> > & getTiles( )
	{
		return itsTiles;
	}

	/**
	 * @fn	std::vector<std::shared_ptr<Slice> > & Picture::getSlices( )
	 *
	 * @brief	Zwraca listê plastrów.
	 *
	 * @return	Lista plastrów.
	 */

	std::vector<std::shared_ptr<Slice> > & getSlices( )
	{
		return itsSlices;
	}

	/**
	 * @fn	std::shared_ptr<CTU> Picture::getCTU( UInt ctuX, UInt ctuY )
	 *
	 * @brief	Dostêp do CTU na podstawie jej wspó³rzêdnych (wyra¿onej w jednostkach CTU od lewego górnego rogu obrazu).
	 *
	 * @param	ctuX	Numer kolumny jendostek CTU.
	 * @param	ctuY	Numer wiersza jednostek CTU
	 *
	 * @return	CTU na danej pozycji.
	 */

	std::shared_ptr<CTU> getCTU( UInt ctuX, UInt ctuY )
	{
		return itsCTUs[ ctuX + ctuY * SeqParams( )->getPicWidthInCTUs( ) ];
	}

	/**
	 * @fn	std::shared_ptr<CTU> Picture::getCTUBySamplePosition( UInt sampleX, UInt sampleY )
	 *
	 * @brief	Dostêp do CTU na podstawie wspó³rzêdnych próbki luminancji, która wchodzi w jego obrêb.
	 *
	 * @param	sampleX	Wspó³rzêdna X próbki.
	 * @param	sampleY	Wspó³rzêdna Y próbki.
	 *
	 * @return	CTU zawieraj¹ca dan¹ próbkê.
	 */

	std::shared_ptr<CTU> getCTUBySamplePosition( UInt sampleX, UInt sampleY )
	{
		UInt ctuX = sampleX / SeqParams( )->getCTUSize( ), ctuY = sampleY / SeqParams( )->getCTUSize( );

		return getCTU( ctuX, ctuY );
	}

	/**
	 * @fn	std::shared_ptr<Tile> Picture::getCurrentTile( )
	 *
	 * @brief	Dostêp do aktualnego kafla.
	 *
	 * @return	Aktualny kafel.
	 */

	std::shared_ptr<Tile> getCurrentTile( )
	{
		return itsCurrentTile;
	}

	/**
	 * @fn	std::shared_ptr<Slice> Picture::getCurrentSlice( )
	 *
	 * @brief	Dostêp do aktualnego plastra.
	 *
	 * @return	Aktualny plaster.
	 */

	std::shared_ptr<Slice> getCurrentSlice( )
	{
		return itsCurrentSlice;
	}

	/**
	 * @fn	std::shared_ptr<CTU> Picture::getCurrentCTU( )
	 *
	 * @brief	Dostêp do aktualnego CTU.
	 *
	 * @return	Aktualny CTU.
	 */

	std::shared_ptr<CTU> getCurrentCTU( )
	{
		return itsCurrentCTU;
	}

#pragma endregion

	/**
	 * @fn	Void Picture::reset( );
	 *
	 * @brief	Usuwa informacjê o podziale i kasuje bufory próbek.
	 *
	 * @return	Void.
	 */

	Void reset( );

	/**
	 * @fn	Void Picture::initPartitions( PartitioningStrategy* strategy );
	 *
	 * @brief	Dzieli ka¿de CTU wed³ug danej strategii.
	 *
	 * @param [in]	strategy	WskaŸnik na strategiê podzia³u.
	 *
	 * @return	Void.
	 */

	Void initPartitions( PartitioningStrategy* strategy );

	/**
	 * @fn	Void Picture::initCTUs( );
	 *
	 * @brief	Tworzy listê CTU pokrywaj¹cyh obraz w jednym plastrze i (ew.) kaflu.
	 *
	 * @return	Void.
	 */

	Void initCTUs( );

	/**
	 * @fn	std::shared_ptr<CU> Picture::getCuContainingPosition( UInt x, UInt y );
	 *
	 * @brief	Dostêp do CU na podstawie wspó³rzêdnych próbki luminancji, która wchodzi w jego obrêb.
	 *
	 * @param	x	The Wspó³rzêdna X próbki.
	 * @param	y	The Wspó³rzêdna Y próbki.
	 *
	 * @return	CU obejmuj¹ce dan¹ próbkê.
	 */

	std::shared_ptr<CU> getCuContainingPosition( UInt x, UInt y );

	/**
	 * @fn	Void Picture::printDescription( );
	 *
	 * @brief	Wypisuje opis struktury obrazu.
	 *
	 * @return	Void.
	 */

	Void printDescription( );

	/**
	 * @fn	Void Picture::reconstructionLoop( );
	 *
	 * @brief	Pêtla rekonstrukcji dla obrazu (wywo³anie przekazane do ni¿szych piêter).
	 *
	 * @return	Void.
	 */

	Void reconstructionLoop( );

	/**
	 * @brief	Log.
	 */

	Logger log;
};

#endif