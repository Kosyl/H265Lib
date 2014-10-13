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
	 * @brief	Bufor oryginalnych pr�bek luminancji.
	 */

	Sample** itsSamplesY;

	/**
	 * @brief	Bufor oryginalnych pr�bek chrominancji Cb.
	 */

	Sample** itsSamplesCb;

	/**
	 * @brief	Bufor oryginalnych pr�bek chrominancji Cr.
	 */

	Sample** itsSamplesCr;

	/**
	 * @brief	Bufor zrekonstruowanych pr�bek luminancji.
	 */

	Sample** itsReconY;

	/**
	 * @brief	Bufor zrekonstruowanych pr�bek chrominancji Cb.
	 */

	Sample** itsReconCb;

	/**
	 * @brief	Bufor zrekonstruowanych pr�bek chrominancji Cr.
	 */

	Sample** itsReconCr;

	/**
	 * @property	std::vector<std::shared_ptr<Tile> > itsTiles
	 *
	 * @brief	Lista kafli w obr�bie obrazu.
	 *
	 */

	std::vector<std::shared_ptr<Tile> > itsTiles;

	/**
	 * @property	std::vector<std::shared_ptr<Slice> > itsSlices
	 *
	 * @brief	Lista plastr�w w obr�bie obrazu.
	 *
	 */

	std::vector<std::shared_ptr<Slice> > itsSlices;

	/**
	 * @property	std::vector<std::shared_ptr<CTU> > itsCTUs
	 *
	 * @brief	Lista CTU w obr�bie obrazu.
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
	 * @brief	Skasowanie bufor�w pr�bek.
	 *
	 * @return	Void.
	 */

	Void deleteMatrices( );

	/**
	 * @fn	Void Picture::setSize( UInt width, UInt height );
	 *
	 * @brief	Ustawia rozmiar obrazu i rezerwuje bufory pr�bek.
	 *
	 * @param	width 	Szeroko�� obrazu.
	 * @param	height	D�ugo�� obrazu.
	 *
	 * @return	Void.
	 */

	Void setSize( UInt width, UInt height );

public:

	/**
	 * @fn	Picture::Picture( );
	 *
	 * @brief Konstruktor domy�lny.
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
	 * @brief	Nadanie rozmiaru (na podstawie SequenceParameters) i zarezerwowanie bufor�w.
	 *
	 * @return	Void.
	 */

	Void initFrame( );

	/**
	 * @fn	Void Picture::loadFrameFromYuv( std::ifstream& yuvStream );
	 *
	 * @brief	Wczytanie macierzy pr�bek oryginalncyh ze strumienia.
	 *
	 * @param [in]	yuvStream	Strumie� do pliku yuv.
	 *
	 */

	Void loadFrameFromYuv( std::ifstream& yuvStream );

#pragma region Akcesory

	/**
	 * @fn	Sample** Picture::getSamples( const ImgComp comp ) const;
	 *
	 * @brief	Dost�p do pr�bek oryginalnych danej sk�adowej obrazu.
	 *
	 * @param	comp	Wybrana sk�adowa obrazu.
	 *
	 * @return	Macierz pr�bek oryginalnych.
	 */

	Sample** getSamples( const ImgComp comp ) const;

	/**
	 * @fn	Sample** Picture::getReconMatrix( const ImgComp comp ) const;
	 *
	 * @brief	Dost�p do pr�bek zrekonstruowanych danej sk�adowej obrazu.
	 *
	 * @param	comp	Wybrana sk�adowa obrazu.
	 *
	 * @return	Macierz pr�bek zrekonstruowanych.
	 */

	Sample** getReconMatrix( const ImgComp comp ) const;

	/**
	 * @fn	std::vector<std::shared_ptr<Tile> > & Picture::getTiles( )
	 *
	 * @brief	Zwraca list� kafli.
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
	 * @brief	Zwraca list� plastr�w.
	 *
	 * @return	Lista plastr�w.
	 */

	std::vector<std::shared_ptr<Slice> > & getSlices( )
	{
		return itsSlices;
	}

	/**
	 * @fn	std::shared_ptr<CTU> Picture::getCTU( UInt ctuX, UInt ctuY )
	 *
	 * @brief	Dost�p do CTU na podstawie jej wsp�rz�dnych (wyra�onej w jednostkach CTU od lewego g�rnego rogu obrazu).
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
	 * @brief	Dost�p do CTU na podstawie wsp�rz�dnych pr�bki luminancji, kt�ra wchodzi w jego obr�b.
	 *
	 * @param	sampleX	Wsp�rz�dna X pr�bki.
	 * @param	sampleY	Wsp�rz�dna Y pr�bki.
	 *
	 * @return	CTU zawieraj�ca dan� pr�bk�.
	 */

	std::shared_ptr<CTU> getCTUBySamplePosition( UInt sampleX, UInt sampleY )
	{
		UInt ctuX = sampleX / SeqParams( )->getCTUSize( ), ctuY = sampleY / SeqParams( )->getCTUSize( );

		return getCTU( ctuX, ctuY );
	}

	/**
	 * @fn	std::shared_ptr<Tile> Picture::getCurrentTile( )
	 *
	 * @brief	Dost�p do aktualnego kafla.
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
	 * @brief	Dost�p do aktualnego plastra.
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
	 * @brief	Dost�p do aktualnego CTU.
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
	 * @brief	Usuwa informacj� o podziale i kasuje bufory pr�bek.
	 *
	 * @return	Void.
	 */

	Void reset( );

	/**
	 * @fn	Void Picture::initPartitions( PartitioningStrategy* strategy );
	 *
	 * @brief	Dzieli ka�de CTU wed�ug danej strategii.
	 *
	 * @param [in]	strategy	Wska�nik na strategi� podzia�u.
	 *
	 * @return	Void.
	 */

	Void initPartitions( PartitioningStrategy* strategy );

	/**
	 * @fn	Void Picture::initCTUs( );
	 *
	 * @brief	Tworzy list� CTU pokrywaj�cyh obraz w jednym plastrze i (ew.) kaflu.
	 *
	 * @return	Void.
	 */

	Void initCTUs( );

	/**
	 * @fn	std::shared_ptr<CU> Picture::getCuContainingPosition( UInt x, UInt y );
	 *
	 * @brief	Dost�p do CU na podstawie wsp�rz�dnych pr�bki luminancji, kt�ra wchodzi w jego obr�b.
	 *
	 * @param	x	The Wsp�rz�dna X pr�bki.
	 * @param	y	The Wsp�rz�dna Y pr�bki.
	 *
	 * @return	CU obejmuj�ce dan� pr�bk�.
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
	 * @brief	P�tla rekonstrukcji dla obrazu (wywo�anie przekazane do ni�szych pi�ter).
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