/**
 * @file	CTU.h
 *
 * @brief	Deklaracja klasy jednostki drzewa kodowego CTU.
 */

#ifndef CTU_h__
#define CTU_h__

#include "Utils.h"
#include "SAOInfo.h"
#include "Picture.h"
#include "UnitBase.h"
#include "QuadTree.h"

/**
 * @class	CTU
 *
 * @brief Struktura jednostki drzewa kodowego CTU
 */

class CTU : public UnitBase
{
private:

	/**
	 * @property	Bool itsSAOMergeLeft, itsSAOMergeUp
	 *
	 * @brief	Flagi ³¹czenia parametrów filtru SAOz s¹siednimi CTU.
	 *
	 */

	Bool itsSAOMergeLeft, itsSAOMergeUp;

	/**
	 * @property	std::vector<std::shared_ptr<SAOInfo> > itsSAOInfo
	 *
	 * @brief	Trójelementowy wektor wskaŸników na informacje o parametrach filtracji SAO dla ka¿dej sk³adowej obrazu.
	 *
	 */

	std::vector<std::shared_ptr<SAOInfo> > itsSAOInfo; //dla luma, cb, cr

	/**
	 * @brief	WskaŸnik na nadrzêdny kafel.
	 */

	std::shared_ptr<Tile> itsParentTile;	

	/**
	 * @brief	WskaŸnik na nadrzêdny plaster.
	 */

	std::shared_ptr<Slice> itsParentSlice;

	/**
	 * @brief	Drzewo jednostek kodowych podlegaj¹ce pod dan¹ CTU.
	 */

	std::shared_ptr<CUQuadTree> itsCUQuadTree;

public:

	/**
	 * @fn	CTU::CTU( Picture* picture, std::shared_ptr<Slice> slice, UShort x, UShort y );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	picture	WskaŸnik na nadrzêdny obraz.
	 * @param	slice						 	WskaŸnik na nadrzêdny plaster.
	 * @param	x								 	Wspó³rzêdna X jednostki.
	 * @param	y								 	Wspó³rzêdna Y jednostki.
	 */

	CTU( Picture* picture, std::shared_ptr<Slice> slice, UShort x, UShort y );

	/**
	 * @fn	CTU::CTU( Picture* picture, std::shared_ptr<Slice> slice, std::shared_ptr<Tile> tile, UShort x, UShort y );
	 *
	 * @brief	Konstruktor dla CTU z obs³ug¹ kafli.
	 *
	 * @param [in,out]	picture	If non-null, the picture.
	 * @param	slice						 	WskaŸnik na nadrzêdny plaster.
	 * @param	tile						 	WskaŸnik na nadrzêdny kafel.
	 * @param	x								 	Wspó³rzêdna X jednostki.
	 * @param	y								 	Wspó³rzêdna Y jednostki.
	 */

	CTU( Picture* picture, std::shared_ptr<Slice> slice, std::shared_ptr<Tile> tile, UShort x, UShort y );

	/**
	 * @fn	virtual CTU::~CTU( );
	 *
	 * @brief	Destruktor.
	 */

	virtual ~CTU( );

#pragma region Akcesory

	/**
	 * @fn	std::shared_ptr<SAOInfo> CTU::getSAOInfo( ImgComp comp )
	 *
	 * @brief	Pobiera wskaŸnik na strukturê informacji o filtracji SAO dla danej sk³adowej obrazu.
	 *
	 * @param	comp	Sk³adowa obrazu
	 *
	 * @return	WskaŸnik na strukturê SAO
	 */

	std::shared_ptr<SAOInfo> getSAOInfo( ImgComp comp )
	{
		return itsSAOInfo[comp];
	}

	/**
	 * @fn	std::shared_ptr<CUQuadTree> CTU::getCUQuadTree( )
	 *
	 * @brief	Zwraca korzeñ drzewa jednostek kodowych.
	 *
	 * @return	Korzeñ drzewa jednostek kodowych.
	 */

	std::shared_ptr<CUQuadTree> getCUQuadTree( )
	{
		return itsCUQuadTree;
	}

	/**
	 * @fn	std::shared_ptr<Tile> CTU::getParentTile( ) const
	 *
	 * @brief	Zwraca wskaŸnik na aktualny kafel.
	 *
	 * @return	WskaŸnik na aktualny kafel.
	 */

	std::shared_ptr<Tile> getParentTile( ) const
	{
		return itsParentTile;
	}

	/**
	 * @fn	void CTU::setParentTile( std::shared_ptr<Tile> val )
	 *
	 * @brief	Ustawia kafel dla tej CTU.
	 *
	 * @param	val	WskaŸnik na nowy kafel.
	 */

	void setParentTile( std::shared_ptr<Tile> val )
	{
		itsParentTile = val;
	}

#pragma endregion

	/**
	 * @fn	Bool CTU::isPartitioned( )
	 *
	 * @brief	Sprawdza czy ta jednostka ma ju¿ drzewo jednostek kodowych.
	 *
	 * @return	Prawda jeœli istnieje drzewo jednostek kodowych; fa³sz w p.p.
	 */

	Bool isPartitioned( )
	{
		return itsCUQuadTree != nullptr;
	}

	/**
	 * @fn	std::shared_ptr<Slice> CTU::getParentSlice( ) const
	 *
	 * @brief	Zwraca wskaŸnik na aktualny plaster.
	 *
	 * @return	WskaŸnik na plaster, do którego nale¿y dana CTU.
	 */

	std::shared_ptr<Slice> getParentSlice( ) const
	{
		return itsParentSlice;
	}

	/**
	 * @fn	void CTU::setParentSlice( std::shared_ptr<Slice> val )
	 *
	 * @brief	Ustawia plaster dla danej CTU.
	 *
	 * @param	val	WskaŸnik na nowy plaster.
	 */

	void setParentSlice( std::shared_ptr<Slice> val )
	{
		itsParentSlice = val;
	}

	/**
	 * @fn	virtual Void CTU::printDescription( );
	 *
	 * @brief	Wypisuje opis struktury do pliku logu.
	 *
	 * @return	Void.
	 */

	virtual Void printDescription( );

	/**
	 * @fn	Void CTU::reconstructionLoop( );
	 *
	 * @brief	Przeprowadza pêtlê rekonstrukcji dla wszystkich sk³adowych podjednostek.
	 *
	 * @return	Void.
	 */

	Void reconstructionLoop( );
};
#endif // CTU_h__