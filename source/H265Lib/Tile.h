/**
 * @file	Tile.h
 *
 * @brief	Deklaracja klasy kafla.
 */

#ifndef _TILE_H_
#define _TILE_H_

#include "Utils.h"
#include <vector>
#include "SeqPicParameters.h"

class CTU;

/**
 * @class	Tile
 *
 * @brief	Klasa kafla..
 */

class Tile
{
private:

	/**
	 * @property	UShort itsX, itsY
	 *
	 * @brief	Wspó³rzêdne kafla.
	 */

	UShort itsX, itsY;

	/**
	 * @property	UShort itsWidth, itsWidthInCTBs, itsHeight, itsHeightInCTBs
	 *
	 * @brief	Rozmiary kafla.
	 */

	UShort itsWidth, itsWidthInCTBs, itsHeight, itsHeightInCTBs;

	/**
	 * @brief	Indeks kafla.
	 */

	UShort itsIdx;

	/**
	 * @property	std::vector<std::shared_ptr<CTU> > itsCTUs
	 *
	 * @brief	CTU w kaflu.
	 */

	std::vector<std::shared_ptr<CTU> > itsCTUs;

public:

	/**
	 * @fn	Tile::Tile( );
	 *
	 * @brief	Konstruktor domyœlny.
	 */

	Tile( );

	/**
	 * @fn	Tile::Tile( UShort x, UShort y, UShort width, UShort height );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param	x				Wspó³rzêdna X kafla.
	 * @param	y				Wspó³rzêdna Y kafla.
	 * @param	width 	Szerokoœæ kafla.
	 * @param	height	Wysokoœæ kafla.
	 */

	Tile( UShort x, UShort y, UShort width, UShort height );

	/**
	 * @fn	Tile::~Tile( );
	 *
	 * @brief	Destruktor.
	 */

	~Tile( );

#pragma region Akcesory

	/**
	 * @fn	UShort Tile::getY( ) const
	 *
	 * @brief	Wspó³rzêdna Y kafla.
	 */

	UShort getY( ) const
	{
		return itsY;
	}

	/**
	 * @fn	UShort Tile::getX( ) const
	 *
	 * @brief	Wspó³rzêdna X kafla.
	 */

	UShort getX( ) const
	{
		return itsX;
	}

	/**
	 * @fn	UShort Tile::getIdx( ) const
	 *
	 * @brief	Indeks kafla.
	 */

	UShort getIdx( ) const
	{
		return itsIdx;
	}

	/**
	 * @fn	Void Tile::setIdx( UShort val )
	 *
	 * @brief	Ustawia indeks kafla.
	 *
	 * @param	val Indeks kafla.
	 *
	 * @return	Void.
	 */

	Void setIdx( UShort val )
	{
		itsIdx = val;
	}

	/**
	 * @fn	UShort Tile::getHeightInCTBs( ) const
	 *
	 * @brief	Wysokoœæ kafla w jednostkach CTB.
	 */

	UShort getHeightInCTBs( ) const
	{
		return itsHeightInCTBs;
	}

	/**
	 * @fn	UShort Tile::getHeight( ) const
	 *
	 * @brief	Wysokoœæ kafla.
	 */

	UShort getHeight( ) const
	{
		return itsHeight;
	}

	/**
	 * @fn	UShort Tile::getWidthInCTBs( ) const
	 *
	 * @brief	Szerokoœæ kafla w jednostkach CTB.
	 */

	UShort getWidthInCTBs( ) const
	{
		return itsWidthInCTBs;
	}

	/**
	 * @fn	UShort Tile::getWidth( ) const
	 *
	 * @brief	Szerokoœæ kafla.
	 */

	UShort getWidth( ) const
	{
		return itsWidth;
	}

#pragma endregion 

	/**
	 * @fn	Void Tile::appendCTU( std::shared_ptr<CTU> ctu );
	 *
	 * @brief	Dodaje CTU do kafla.
	 *
	 * @param	ctu Nowe CTU.
	 *
	 * @return	Void.
	 */

	Void appendCTU( std::shared_ptr<CTU> ctu );
};

#endif