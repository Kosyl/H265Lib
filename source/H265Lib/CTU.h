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
	 * @brief	Flagi ��czenia parametr�w filtru SAOz s�siednimi CTU.
	 *
	 */

	Bool itsSAOMergeLeft, itsSAOMergeUp;

	/**
	 * @property	std::vector<std::shared_ptr<SAOInfo> > itsSAOInfo
	 *
	 * @brief	Tr�jelementowy wektor wska�nik�w na informacje o parametrach filtracji SAO dla ka�dej sk�adowej obrazu.
	 *
	 */

	std::vector<std::shared_ptr<SAOInfo> > itsSAOInfo; //dla luma, cb, cr

	/**
	 * @brief	Wska�nik na nadrz�dny kafel.
	 */

	std::shared_ptr<Tile> itsParentTile;	

	/**
	 * @brief	Wska�nik na nadrz�dny plaster.
	 */

	std::shared_ptr<Slice> itsParentSlice;

	/**
	 * @brief	Drzewo jednostek kodowych podlegaj�ce pod dan� CTU.
	 */

	std::shared_ptr<CUQuadTree> itsCUQuadTree;

public:

	/**
	 * @fn	CTU::CTU( Picture* picture, std::shared_ptr<Slice> slice, UShort x, UShort y );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	picture	Wska�nik na nadrz�dny obraz.
	 * @param	slice						 	Wska�nik na nadrz�dny plaster.
	 * @param	x								 	Wsp�rz�dna X jednostki.
	 * @param	y								 	Wsp�rz�dna Y jednostki.
	 */

	CTU( Picture* picture, std::shared_ptr<Slice> slice, UShort x, UShort y );

	/**
	 * @fn	CTU::CTU( Picture* picture, std::shared_ptr<Slice> slice, std::shared_ptr<Tile> tile, UShort x, UShort y );
	 *
	 * @brief	Konstruktor dla CTU z obs�ug� kafli.
	 *
	 * @param [in,out]	picture	If non-null, the picture.
	 * @param	slice						 	Wska�nik na nadrz�dny plaster.
	 * @param	tile						 	Wska�nik na nadrz�dny kafel.
	 * @param	x								 	Wsp�rz�dna X jednostki.
	 * @param	y								 	Wsp�rz�dna Y jednostki.
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
	 * @brief	Pobiera wska�nik na struktur� informacji o filtracji SAO dla danej sk�adowej obrazu.
	 *
	 * @param	comp	Sk�adowa obrazu
	 *
	 * @return	Wska�nik na struktur� SAO
	 */

	std::shared_ptr<SAOInfo> getSAOInfo( ImgComp comp )
	{
		return itsSAOInfo[comp];
	}

	/**
	 * @fn	std::shared_ptr<CUQuadTree> CTU::getCUQuadTree( )
	 *
	 * @brief	Zwraca korze� drzewa jednostek kodowych.
	 *
	 * @return	Korze� drzewa jednostek kodowych.
	 */

	std::shared_ptr<CUQuadTree> getCUQuadTree( )
	{
		return itsCUQuadTree;
	}

	/**
	 * @fn	std::shared_ptr<Tile> CTU::getParentTile( ) const
	 *
	 * @brief	Zwraca wska�nik na aktualny kafel.
	 *
	 * @return	Wska�nik na aktualny kafel.
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
	 * @param	val	Wska�nik na nowy kafel.
	 */

	void setParentTile( std::shared_ptr<Tile> val )
	{
		itsParentTile = val;
	}

#pragma endregion

	/**
	 * @fn	Bool CTU::isPartitioned( )
	 *
	 * @brief	Sprawdza czy ta jednostka ma ju� drzewo jednostek kodowych.
	 *
	 * @return	Prawda je�li istnieje drzewo jednostek kodowych; fa�sz w p.p.
	 */

	Bool isPartitioned( )
	{
		return itsCUQuadTree != nullptr;
	}

	/**
	 * @fn	std::shared_ptr<Slice> CTU::getParentSlice( ) const
	 *
	 * @brief	Zwraca wska�nik na aktualny plaster.
	 *
	 * @return	Wska�nik na plaster, do kt�rego nale�y dana CTU.
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
	 * @param	val	Wska�nik na nowy plaster.
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
	 * @brief	Przeprowadza p�tl� rekonstrukcji dla wszystkich sk�adowych podjednostek.
	 *
	 * @return	Void.
	 */

	Void reconstructionLoop( );
};
#endif // CTU_h__