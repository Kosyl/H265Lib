/**
 * @file	Slice.h
 *
 * @brief	Deklaracja klasy plastra.
 */

#ifndef Slice_h__
#define Slice_h__

#include "Utils.h"
#include <vector>
#include "Picture.h"

/**
 * @class	Slice
 *
 * @brief	Klasa plastra.
 */

class Slice
{
private:

	/**
	 * @brief	Typ plastra (I/P/B).
	 */

	SliceMode itsSliceType;

	/**
	 * @property	Bool itsSAOLumaEnabled, itsSAOChromaEnabled
	 *
	 * @brief	Zezwolenie na filtracjê SAO próbek luminancji i chrominancji.
	 */

	Bool itsSAOLumaEnabled, itsSAOChromaEnabled;

	/**
	 * @brief	Zezwolenie na u¿ycie filtru deblokuj¹cego.
	 */

	Bool itsDeblockingFilterEnabled;

	/**
	 * @brief	Zezwolenie na u¿ycie filtru deblokuj¹cego oraz SAO.
	 */

	Bool itsLoopFilterEnabled;

	/**
	 * @brief	Indeks porz¹dkowy.
	 */

	UInt itsSliceIdx;

	/**
	 * @brief	Zmiana QP wzglêdem wartoœci bazowej z parametrów obrazu.
	 */

	Short itsQPDelta;

	/**
	 * @property	Short itsQPOffsetForCr, itsQPOffsetForCb
	 *
	 * @brief	Offsety QP dla chrominancji.
	 */

	Short itsQPOffsetForCr, itsQPOffsetForCb;

	/**
	 * @brief	W³¹cza/wy³¹cza synchronizacjê kodera entropijnego na tym plastrze.
	 */

	Bool itsECSyncEnabled;

	/**
	 * @brief	Zmiana parametru Beta w filtrze deblokuj¹cym.
	 */

	Short itsBetaOffset;

	/**
	 * @brief	Zmiana parametru tc w filtrze deblokuj¹cym.
	 */

	Short itsTcOffset;

	/**
	 * @property	std::vector<std::weak_ptr<CTU> > itsCTUs
	 *
	 * @brief CTU zawieraj¹ce siê w plastrze.
	 *
	 */

	std::vector<std::weak_ptr<CTU> > itsCTUs;

	/**
	 * @brief	Nadrzêdny obraz plastra.
	 */

	Picture* itsParentPicture;

public:

	/**
	 * @fn	Slice::Slice( Picture* picture, SliceMode type = SliceMode::SLICE_I );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	Obraz, w którym jest plaster.
	 * @param	type						 	Rodzaj plastra.
	 */

	Slice( Picture* picture, SliceMode type = SliceMode::SLICE_I );

	/**
	 * @fn	Slice::~Slice( );
	 *
	 * @brief	Destruktor.
	 */

	~Slice( );

#pragma region Akcesory

	/**
	 * @fn	Picture* Slice::getParentPicture( ) const
	 *
	 * @brief	Zwraca obraz w kórym le¿y plaster.
	 */

	Picture* getParentPicture( ) const
	{
		return itsParentPicture;
	}

	/**
	 * @fn	SliceMode Slice::getType( ) const
	 *
	 * @brief	Rodzaj plastra.
	 */

	SliceMode getType( ) const
	{
		return itsSliceType;
	}

	/**
	 * @fn	Void Slice::setSliceType( SliceMode type )
	 *
	 * @brief	Ustawia tryb plastra.
	 *
	 * @param	type	Typ plastra.
	 *
	 * @return	Void.
	 */

	Void setSliceType( SliceMode type )
	{
		itsSliceType = type;
	}

	/**
	 * @fn	Bool Slice::getSAOChromaEnabled( ) const
	 *
	 * @brief	Zezwolenie na u¿ycie SAO dla chrominancji.
	 */

	Bool getSAOChromaEnabled( ) const
	{
		return itsSAOChromaEnabled;
	}

	/**
	 * @fn	Void Slice::setSAOChromaEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na u¿ycie SAO dla chrominancji.
	 *
	 * @param	val	true zezwala, false blokuje SAO dla chrominancji.
	 *
	 * @return	Void.
	 */

	Void setSAOChromaEnabled( Bool val )
	{
		itsSAOChromaEnabled = val;
	}

	/**
	 * @fn	Bool Slice::getSAOLumaEnabled( ) const
	 *
	 * @brief	Zezwolenie na u¿ycie SAO dla luminancji.
	 */

	Bool getSAOLumaEnabled( ) const
	{
		return itsSAOLumaEnabled;
	}

	/**
	 * @fn	Void Slice::setSAOLumaEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na u¿ycie SAO dla luminancji.
	 *
	 * @param	val	true zezwala, false blokuje SAO dla luminancji.
	 *
	 * @return	Void.
	 */

	Void setSAOLumaEnabled( Bool val )
	{
		itsSAOLumaEnabled = val;
	}

	/**
	 * @fn	Bool Slice::getsLoopFilterEnabled( ) const
	 *
	 * @brief		Zezwolenie na u¿ycie FDB i SAO.
	 */

	Bool getsLoopFilterEnabled( ) const
	{
		return itsLoopFilterEnabled;
	}

	/**
	 * @fn	Void Slice::setLoopFilterEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na u¿ycie FDB i SAO.
	 *
	 * @param	val	true zezwala, false u¿ycie FDB i SAO.
	 *
	 * @return	Void.
	 */

	Void setLoopFilterEnabled( Bool val )
	{
		itsLoopFilterEnabled = val;
	}

	/**
	 * @fn	Bool Slice::getDeblockingFilterEnabled( ) const
	 *
	 * @brief	Zezwolenie na u¿ycie FDB.
	 */

	Bool getDeblockingFilterEnabled( ) const
	{
		return itsDeblockingFilterEnabled;
	}

	/**
	 * @fn	Void Slice::setDeblockingFilterEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na u¿ycie FDB.
	 *
	 * @param	val	true zezwala, false u¿ycie FDB.
	 *
	 * @return	Void.
	 */

	Void setDeblockingFilterEnabled( Bool val )
	{
		itsDeblockingFilterEnabled = val;
	}

	/**
	 * @fn	Short Slice::getQPDelta( ImgComp comp )
	 *
	 * @brief	Zwraca offset QP wnoszony przez plaster dla danej sk³adowej obrazu.
	 *
	 * @param	comp	Sk³adowa obrazu.
	 *
	 * @return	Offset wartoœci QP.
	 */

	Short getQPDelta( ImgComp comp )
	{
		switch( comp )
		{
		case CB:
			return itsQPDelta + itsQPOffsetForCb;
		case CR:
			return itsQPDelta + itsQPOffsetForCr;
		case LUMA:
		default:
			return itsQPDelta;
		}
	}

	/**
	 * @fn	UInt Slice::getSliceIdx( ) const
	 *
	 * @brief	Indeks plastra.
	 */

	UInt getSliceIdx( ) const
	{
		return itsSliceIdx;
	}

	/**
	 * @fn	void Slice::setSliceIdx( UInt val )
	 *
	 * @brief	Ustawia indeks plastra.
	 *
	 * @param	val	Nowy indeks.
	 */

	void setSliceIdx( UInt val )
	{
		itsSliceIdx = val;
	}

	/**
	 * @fn	Short Slice::getBetaOffset( ) const
	 *
	 * @brief	Offset wartoœci Beta w FDB.
	 */

	Short getBetaOffset( ) const
	{
		return itsBetaOffset;
	}

	/**
	 * @fn	void Slice::setBetaOffset( Short val )
	 *
	 * @brief	Ustawia wartoœæ offsetu Beta w FDB.
	 *
	 * @param	val	Nowa wartoœæ offsetu Beta.
	 */

	void setBetaOffset( Short val )
	{
		itsBetaOffset = val;
	}

	/**
	 * @fn	Short Slice::getTcOffset( ) const
	 *
	 * @brief	Offset wartoœci tc w FDB.
	 */

	Short getTcOffset( ) const
	{
		return itsTcOffset;
	}

	/**
	 * @fn	void Slice::setTcOffset( Short val )
	 *
	 * @brief	Ustawia wartoœæ offsetu tc w FDB.
	 *
	 * @param	val	Nowa wartoœæ offsetu tc.
	 */

	void setTcOffset( Short val )
	{
		itsTcOffset = val;
	}
#pragma endregion

	/**
	 * @fn	Void Slice::appendCTU( std::shared_ptr<CTU> ctu );
	 *
	 * @brief	Dodaje do plastra nowe CTU.
	 *
	 * @param	ctu	Nowe CTU.
	 *
	 * @return	Void.
	 */

	Void appendCTU( std::shared_ptr<CTU> ctu );
};
#endif // Slice_h__
