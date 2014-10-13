/**
 * @file	SeqPicParameters.h
 *
 * @brief	Deklaracje klas parametrów.
 */

#ifndef SeqPicParameters_h__
#define SeqPicParameters_h__

#include "Utils.h"

/**
 * @class	ZScanArray
 *
 * @brief	Klasa indeksu Z-skanowania.
 */

class ZScanArray
{
	/**
	 * @brief	Wartoœci indeksów.
	 */

	static Int** itsValues;

	/**
	 * @property	static UInt itsSizeX, itsSizeY
	 *
	 * @brief	Rozmiar macierzy indeksów.
	 *
	 */

	static UInt itsSizeX, itsSizeY;

public:

	/**
	 * @fn	static Void ZScanArray::reset( );
	 *
	 * @brief	Resetuje macierz indeksów.
	 *
	 * @return	Void.
	 */

	static Void reset( );

	/**
	 * @fn	static Int ZScanArray::get( UInt x, UInt y );
	 *
	 * @brief	Pobiera indeks Z-skanowania dla danej pozycji (wyra¿onej w blokach 4x4).
	 *
	 * @param	x	Wspó³rzêdna X bloku 4x4.
	 * @param	y	Wspó³rzêdna Y bloku 4x4.
	 *
	 * @return	Indeks Z-skanowania.
	 */

	static Int get( UInt x, UInt y );

	/**
	 * @fn	static Void ZScanArray::print( std::ostream& out );
	 *
	 * @brief	Wypisuje macierz indeksów Z-skanowania dla obrazu.
	 *
	 * @param [in]	Strumieñ wyjœciowy
	 *
	 * @return	Void.
	 */

	static Void print( std::ostream& out );
};

/**
 * @class	SequenceParameters
 *
 * @brief	Klasa parametrów sekwencji.
 */

class SequenceParameters
{
private:

	/**
	 * @brief	instancja.
	 */

	static SequenceParameters *itsInstance;

	/**
	 * @fn	SequenceParameters::SequenceParameters( );
	 *
	 * @brief	Konstruktor domyœlny.
	 */

	SequenceParameters( );

	/**
	 * @property	UShort itsPicWidth, itsPicHeight, itsPicWidthInCTUs, itsPicHeightInCTUs
	 *
	 * @brief Wymiary obrazu
	 *
	 */

	UShort itsPicWidth, itsPicHeight, itsPicWidthInCTUs, itsPicHeightInCTUs;

	/**
	 * @property	UShort itsLog2CTUSize, itsLog2MaxCUSize, itsLog2MinCUSize, itsLog2MaxTUSize, itsLog2MinTUSize
	 *
	 * @brief	Logarytmy dwójkowe z rozmiarów poszczególnych bloków.
	 *
	 */

	UShort itsLog2CTUSize, itsLog2MaxCUSize, itsLog2MinCUSize, itsLog2MaxTUSize, itsLog2MinTUSize;

	/**
	 * @property	UShort itsBitDepthLuma, itsBitDepthChroma
	 *
	 * @brief	G³êbie bitowe próbek sk³adowych obrazu.
	 *
	 */

	UShort itsBitDepthLuma, itsBitDepthChroma;

	/**
	 * @property	UShort itsMaxCUSize, itsMinCUSize
	 *
	 * @brief	Limity na rozmiar CU.
	 *
	 */

	UShort itsMaxCUSize, itsMinCUSize; //def. 64 i 8

	/**
	 * @property	UShort itsMaxTUSize, itsMinTUSize
	 *
	 * @brief	Limity na rozmiar TU.
	 */

	UShort itsMaxTUSize, itsMinTUSize; // def. 32 i 4

	/**
	 * @brief	W³¹czenie/wy³aczenie filtru SAO.
	 */

	Bool itsSAOEnabled;

	/**
	 * @brief	W³¹czenie/wy³¹czenie silnej filtracji próbek w predykcji intra
	 */

	Bool itsIntraSmoothingEnabled;

public:

	/**
	 * @fn	static SequenceParameters* SequenceParameters::getInstance( );
	 *
	 * @brief	Dostêp do instancji parametrów sekwencji.
	 *
	 * @return	WskaŸnik na instancjê.
	 */

	static SequenceParameters* getInstance( );

	/**
	 * @fn	SequenceParameters::~SequenceParameters( );
	 *
	 * @brief	Destruktor.
	 */

	~SequenceParameters( );

	/**
	 * @fn	UShort SequenceParameters::getPicWidth( ) const
	 *
	 * @brief	Szerokoœæ obrazu.
	 */

	UShort getPicWidth( ) const
	{
		return itsPicWidth;
	}

	/**
	 * @fn	UShort SequenceParameters::getPicWidthInCTUs( ) const
	 *
	 * @brief	Szerokoœæ obrazu wyra¿ona w CTU.
	 */

	UShort getPicWidthInCTUs( ) const
	{
		return itsPicWidthInCTUs;
	}

	/**
	 * @fn	Void SequenceParameters::setPicWidth( UShort val )
	 *
	 * @brief	Ustawia szerokoœæ obrazu.
	 *
	 * @return	Void.
	 */

	Void setPicWidth( UShort val )
	{
		itsPicWidth = val;
		itsPicWidthInCTUs = ( val - 1 ) / itsMaxCUSize + 1;
	}

	/**
	 * @fn	UShort SequenceParameters::getPicHeight( ) const
	 *
	 * @brief	Wysokosæ obrazu.
	 */

	UShort getPicHeight( ) const
	{
		return itsPicHeight;
	}

	/**
	 * @fn	UShort SequenceParameters::getPicHeightInCTUs( ) const
	 *
	 * @brief	Wysokoœæ obrazu wyra¿ona w CTU.
	 */

	UShort getPicHeightInCTUs( ) const
	{
		return itsPicHeightInCTUs;
	}

	/**
	 * @fn	Void SequenceParameters::setPicHeight( UShort val )
	 *
	 * @brief Ustawia wysokosæ obrazu.
	 *
	 * @return	Void.
	 */

	Void setPicHeight( UShort val )
	{
		itsPicHeight = val;
		itsPicHeightInCTUs = ( val - 1 ) / itsMaxCUSize + 1;
	}

	/**
	 * @fn	UShort SequenceParameters::getBitDepthChroma( ) const
	 *
	 * @brief	G³êbia bitowa chrominancji.
	 */

	UShort getBitDepthChroma( ) const
	{
		return itsBitDepthChroma;
	}

	/**
	 * @fn	Void SequenceParameters::setBitDepthChroma( UShort val )
	 *
	 * @brief	Ustawia g³êbie bitow¹ chrominancji.
	 *
	 * @return	Void.
	 */

	Void setBitDepthChroma( UShort val )
	{
		itsBitDepthChroma = val;
	}

	/**
	 * @fn	UShort SequenceParameters::getBitDepthLuma( ) const
	 *
	 * @brief	G³êbia bitowa luminancji.
	 */

	UShort getBitDepthLuma( ) const
	{
		return itsBitDepthLuma;
	}

	/**
	 * @fn	Void SequenceParameters::setBitDepthLuma( UShort val )
	 *
	 * @brief	Ustawia g³ebiê bitow¹ luminancji.
	 *
	 * @return	Void.
	 */

	Void setBitDepthLuma( UShort val )
	{
		itsBitDepthLuma = val;
	}

	/**
	 * @fn	UShort SequenceParameters::getBitDepth( ImgComp comp ) const
	 *
	 * @brief	G³êbia bitowa dla sk³adowej obrazu.
	 *
	 * @param	comp	Sk³¹dowa obrazu.
	 *
	 * @return	G³êbia bitowa.
	 */

	UShort getBitDepth( ImgComp comp ) const
	{
		return comp == LUMA ? itsBitDepthLuma : itsBitDepthChroma;
	}

	/**
	 * @fn	Void SequenceParameters::setBitDepth( ImgComp comp, UShort val )
	 *
	 * @brief	Ustawia g³êbiê bitow¹.
	 *
	 * @param	comp	Sk³adowa obrazu.
	 * @param	val 	Wartoœæ g³êbii bitowej.
	 *
	 * @return	Void.
	 */

	Void setBitDepth( ImgComp comp, UShort val )
	{
		if( comp == ImgComp::LUMA )
			itsBitDepthLuma = val;
		else
			itsBitDepthChroma = val;
	}

	/**
	 * @fn	Bool SequenceParameters::getSAOEnabled( ) const
	 *
	 * @brief	Pozwolenie na u¿ycie SAO
	 */

	Bool getSAOEnabled( ) const
	{
		return itsSAOEnabled;
	}

	/**
	 * @fn	Void SequenceParameters::setSAOEnabled( Bool val )
	 *
	 * @brief	Ustawia pozwolenie na u¿ycie SAO.
	 *
	 * @param	val	Zezwolenie na SAO.
	 *
	 * @return	Void.
	 */

	Void setSAOEnabled( Bool val )
	{
		itsSAOEnabled = val;
	}

	/**
	 * @fn	UShort SequenceParameters::getMinCUSize( ) const
	 *
	 * @brief	Najmniejszy rozmiar CU.
	 */

	UShort getMinCUSize( ) const
	{
		return itsMinCUSize;
	}

	/**
	 * @fn	Void SequenceParameters::setMinCUSize( UShort val )
	 *
	 * @brief	Ustawia najmniejszy rozmiar CU.
	 *
	 * @param	val	Wybrany rozmiar.
	 *
	 * @return	Void.
	 */

	Void setMinCUSize( UShort val )
	{
		itsMinCUSize = val;
		itsLog2MinCUSize = log2Int( val );
	}

	/**
	 * @fn	UShort SequenceParameters::getMaxCUSize( ) const
	 *
	 * @brief	Najwiêkszy rozmiar CU.
	 */

	UShort getMaxCUSize( ) const
	{
		return itsMaxCUSize;
	}

	/**
	 * @fn	Void SequenceParameters::setMaxCUSize( UShort val )
	 *
	 * @brief	Ustawia najwiêkszy rozmiar CU.
	 *
	 * @param	val	Wybrany rozmiar.
	 *
	 * @return	Void.
	 */

	Void setMaxCUSize( UShort val )
	{
		itsMaxCUSize = val;
		itsLog2MaxCUSize = log2Int( val );
		itsLog2CTUSize = log2Int( val );
	}

	/**
	 * @fn	UShort SequenceParameters::getCTUSize( ) const
	 *
	 * @brief	Rozmiar CTU.
	 */

	UShort getCTUSize( ) const
	{
		return getMaxCUSize( );
	}

	/**
	 * @fn	Bool SequenceParameters::getIntraSmoothingEnabled( ) const
	 *
	 * @brief	Zezwolenie na u¿ycie silnej filtracji w predykcji intra.
	 */

	Bool getIntraSmoothingEnabled( ) const
	{
		return itsIntraSmoothingEnabled;
	}

	/**
	 * @fn	Void SequenceParameters::setIntraSmoothingEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na u¿ycie silnej filtracji w predykcji intra.
	 *
	 * @param	val	true pozwala na siln¹ filtracjê; false nie.
	 *
	 * @return	A Void.
	 */

	Void setIntraSmoothingEnabled( Bool val )
	{
		itsIntraSmoothingEnabled = val;
	}

	/**
	 * @fn	UShort SequenceParameters::getMinTUSize( ) const
	 *
	 * @brief	Najmniejszy rozmiar TU.
	 */

	UShort getMinTUSize( ) const
	{
		return itsMinTUSize;
	}

	/**
	 * @fn	Void SequenceParameters::setMinTUSize( UShort val )
	 *
	 * @brief	Ustawia najmniejszy rozmiar TU.
	 *
	 * @param	val	Wybrany rozmiar.
	 *
	 * @return	Void.
	 */

	Void setMinTUSize( UShort val )
	{
		itsMinTUSize = val;
		itsLog2MinTUSize = log2Int( val );
	}

	/**
	 * @fn	UShort SequenceParameters::getMaxTUSize( ) const
	 *
	 * @brief	najwiêkszy rozmiar TU.
	 */

	UShort getMaxTUSize( ) const
	{
		return itsMaxTUSize;
	}

	/**
	 * @fn	Void SequenceParameters::setMaxTUSize( UShort val )
	 *
	 * @brief	Ustawia najwiêkszy mo¿liwy rozmiar TU.
	 *
	 * @param	val	Wybrany rozmiar.
	 *
	 * @return	Void.
	 */

	Void setMaxTUSize( UShort val )
	{
		itsMaxTUSize = val;
		itsLog2MaxTUSize = log2Int( val );
	}

	/**
	 * @fn	Sample SequenceParameters::clip( ImgComp comp, Int value )
	 *
	 * @brief	Obcina podana wartoœæ do zakresu odpowiadaj¹cemu g³êbii bitowej wybranej sk³adowej obrazu.
	 *
	 * @param	comp 	Sk³adowa obrazu.
	 * @param	value	Wartoœæ do obciêcia.
	 *
	 * @return Obciêta wartoœæ.
	 */

	Sample clip( ImgComp comp, Int value )
	{
		UShort maxVal = comp == LUMA ? 1 << itsBitDepthLuma : 1 << itsBitDepthChroma;
		return clipToRange<Sample>( 0, maxVal, value );
	}

	/**
	 * @fn	Sample SequenceParameters::getDefaultSampleValue( ImgComp comp )
	 *
	 * @brief Domyœlna wartoœæ próbki danej sk³adowej obrazu.
	 *
	 * @param	comp	Sk³adowa obrazu.
	 * 							
	 * @return Domyœlna wartoœæ próbki dla danej sk³adowej.
	 */

	Sample getDefaultSampleValue( ImgComp comp )
	{
		if( comp == LUMA )
			return 1 << ( itsBitDepthLuma - 1 );
		else
			return 1 << ( itsBitDepthChroma - 1 );
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2MinTUSize( ) const
	 *
	 * @brief	Logarytm dwójkowy z najmniejszego rozmiaru TU
	 */

	UShort getLog2MinTUSize( ) const
	{
		return itsLog2MinTUSize;
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2MaxTUSize( ) const
	 *
	 * @brief	Logarytm dwójkowy z najwiêkszego rozmiaru TU
	 */

	UShort getLog2MaxTUSize( ) const
	{
		return itsLog2MaxTUSize;
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2MinCUSize( ) const
	 *
	 * @brief	Logarytm dwójkowy z najmniejszego rozmiaru CU
	 */

	UShort getLog2MinCUSize( ) const
	{
		return itsLog2MinCUSize;
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2MaxCUSize( ) const
	 *
	 * @brief	Logarytm dwójkowy z najwiêkszego rozmiaru CU
	 */

	UShort getLog2MaxCUSize( ) const
	{
		return itsLog2MaxCUSize;
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2CTUSize( ) const
	 *
	 * @brief	Logarytm dwójkowy z rozmiaru CTU
	 */

	UShort getLog2CTUSize( ) const
	{
		return itsLog2CTUSize;
	}

	/**
	 * @fn	Void SequenceParameters::initWithDefaults( );
	 *
	 * @brief	Inicjalizacja parametrów domyœlnymi wartoœciami (CTU size = CUmax size = 64, CUmin size = 8, TUmax size = 32, TUmin size = 4, LumaDepth = ChromaDepth = 8).
	 *
	 * @return	Void.
	 */

	Void initWithDefaults( );
};

/**
 * @class	PictureParameters
 *
 * @brief Klasa parametrów obrazu.
 */

class PictureParameters
{
private:

	/**
	 * @brief	Instancja.
	 */

	static PictureParameters *itsInstance;

	/**
	 * @fn	PictureParameters::PictureParameters( );
	 *
	 * @brief	Konstruktor domyœlny.
	 */

	PictureParameters( );

	/**
	 * @brief	Flaga w³¹czenia trybu SDH.
	 */

	Bool itsSDHEnabled;

	/**
	 * @brief	Parametr kwantyzacji.
	 */

	UShort itsQP;

	/**
	 * @property	Short itsQPOffsetForCb, itsQPOffsetForCr
	 *
	 * @brief	Offsety parametru kwantyzacji dla sk³adowych chrominancji.
	 *
	 */

	Short itsQPOffsetForCb, itsQPOffsetForCr;

	/**
	 * @brief	Flaga zezwolenia na tryb TransformSkip.
	 */

	Bool itsTransformSkipEnabled;

	/**
	 * @brief	Zezwolenie na u¿ycie kafli.
	 */

	Bool itsTilesEnabled;

	/**
	 * @brief	Zezwolenie na kontrolê filtru deblokuj¹cego przez plastry.
	 */

	Bool itsDeblockingFilterControlEnabled;

	/**
	 * @property	Bool itsLoopFilterOnSlicesEnabled, itsLoopFilterOnTilesEnabled
	 *
	 * @brief	Flagi zezwalaj¹ce na filtracjê deblokuj¹c¹ miêdzy plastrami i kaflami.
	 *
	 */

	Bool itsLoopFilterOnSlicesEnabled, itsLoopFilterOnTilesEnabled;

public:

	/**
	 * @fn	static PictureParameters* PictureParameters::getInstance( );
	 *
	 * @brief Dostêp do instancji parametrów sekwencji.
	 *
	 * @return	WskaŸnik na instancjê parametrów obrazu.
	 */

	static PictureParameters* getInstance( );

	/**
	 * @fn	PictureParameters::~PictureParameters( );
	 *
	 * @brief	Destruktor.
	 */

	~PictureParameters( );

#pragma region Akcesory

	/**
	 * @fn	Bool PictureParameters::getLoopFilterOnTilesEnabled( ) const
	 *
	 * @brief	Zezwolenie na FDB na granicach miêdzy kaflami.
	 */

	Bool getLoopFilterOnTilesEnabled( ) const
	{
		return itsLoopFilterOnTilesEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setLoopFilterOnTilesEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na FDB na granicach miêdzy kaflami.
	 *
	 * @param	val	true w³¹cza, false wy³¹cza FDB.
	 *
	 * @return	Void.
	 */

	Void setLoopFilterOnTilesEnabled( Bool val )
	{
		itsLoopFilterOnTilesEnabled = val;
	}

	/**
	 * @fn	Bool PictureParameters::getLoopFilterOnSlicesEnabled( ) const
	 *
	 * @brief	Zezwolenie na FDB na granicach miêdzy plastrami.
	 *
	 */

	Bool getLoopFilterOnSlicesEnabled( ) const
	{
		return itsLoopFilterOnSlicesEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setLoopFilterOnSlicesEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na FDB na granicach miêdzy plastrami.
	 *
	 * @param	val	true w³¹cza, false wy³¹cza FDB.
	 *
	 * @return	Void.
	 */

	Void setLoopFilterOnSlicesEnabled( Bool val )
	{
		itsLoopFilterOnSlicesEnabled = val;
	}

	/**
	 * @fn	Bool PictureParameters::getDeblockingFilterControlEnabled( ) const
	 *
	 * @brief	Zezwolenie na kontrolê FDB przez plastry.
	 */

	Bool getDeblockingFilterControlEnabled( ) const
	{
		return itsDeblockingFilterControlEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setDeblockingFilterControlEnabled( Bool val )
	 *
	 * @brief		Ustawia zezwolenie na kontrolê FDB przez plastry.
	 *
	 * @param	val	true w³¹cza, false wy³¹cza kontrolê FDB.
	 *
	 * @return	Void.
	 */

	Void setDeblockingFilterControlEnabled( Bool val )
	{
		itsDeblockingFilterControlEnabled = val;
	}

	/**
	 * @fn	Bool PictureParameters::getSDHEnabled( ) const
	 *
	 * @brief	Zezwolenie na tryb SDH.
	 *
	 */

	Bool getSDHEnabled( ) const
	{
		return itsSDHEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setSDHEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na tryb SDH.
	 *
	 * @param	val	true w³¹cza, false wy³¹cza tryb SDH.
	 *
	 * @return	Void.
	 */

	Void setSDHEnabled( Bool val )
	{
		itsSDHEnabled = val;
	}

	/**
	 * @fn	UShort PictureParameters::getQP( ImgComp comp = ImgComp::LUMA ) const
	 *
	 * @brief	Zwraca QP dla danej sk³adowej obrazu.
	 *
	 * @param	comp	Sk³adowa obrazu.
	 *
	 * @return	Wartoœæ QP na poziomie parametrów obrazu.
	 */

	UShort getQP( ImgComp comp = ImgComp::LUMA ) const
	{
		if( comp == ImgComp::LUMA )
			return itsQP;
		else if( comp == ImgComp::CB )
			return itsQP + itsQPOffsetForCb;
		else
			return itsQP + itsQPOffsetForCr;
	}

	/**
	 * @fn	Void PictureParameters::setQP( UShort val )
	 *
	 * @brief	Ustawia bazow¹ wartoœæ QP.
	 *
	 * @param	val	Nowa wartoœæ QP.
	 *
	 * @return	Void.
	 */

	Void setQP( UShort val )
	{
		itsQP = val;
	}

	/**
	 * @fn	Short PictureParameters::getQpOffsetForCr( ) const
	 *
	 * @brief	Zwraca offset QP dla chromiancji Cr.
	 */

	Short getQpOffsetForCr( ) const
	{
		return itsQPOffsetForCr;
	}

	/**
	 * @fn	Void PictureParameters::setQpOffsetForCr( Short val )
	 *
	 * @brief	Ustawia offset QP dla chromiancji Cr.
	 *
	 * @param	val wybrany offset wzglêdem wartoœci bazowej.
	 *
	 * @return	Void.
	 */

	Void setQpOffsetForCr( Short val )
	{
		itsQPOffsetForCr = val;
	}

	/**
	 * @fn	Short PictureParameters::getQPOffsetForCb( ) const
	 *
	 * @brief	Zwraca offset QP dla chromiancji Cb.
	 */

	Short getQPOffsetForCb( ) const
	{
		return itsQPOffsetForCb;
	}

	/**
	 * @fn	Void PictureParameters::setQPOffsetForCb( Short val )
	 *
	 * @brief	Ustawia offset QP dla chromiancji Cb.
	 *
	 * @param	val wybrany offset wzglêdem wartoœci bazowej.
	 *
	 * @return	Void.
	 */

	Void setQPOffsetForCb( Short val )
	{
		itsQPOffsetForCb = val;
	}

	/**
	 * @fn	Bool PictureParameters::getTransformSkipEnabled( ) const
	 *
	 * @brief	Zezwolenie na tryb TransformSkip.
	 */

	Bool getTransformSkipEnabled( ) const
	{
		return itsTransformSkipEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setTransformSkipEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na tryb TransformSkip.
	 *
	 * @param	val	true zezwala, false blokuje tryb TransformSkip.
	 *
	 * @return	Void.
	 */

	Void setTransformSkipEnabled( Bool val )
	{
		itsTransformSkipEnabled = val;
	}

	/**
	 * @fn	Bool PictureParameters::getTilesEnabled( ) const
	 *
	 * @brief	Zezwolenie na u¿ycie kafli.
	 */

	Bool getTilesEnabled( ) const
	{
		return itsTilesEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setTilesEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na u¿ycie kafli.
	 *
	 * @param	val	true zezwala, false blokuje u¿ycie kafli.
	 *
	 * @return	Void.
	 */

	Void setTilesEnabled( Bool val )
	{
		itsTilesEnabled = val;
	}
	
#pragma endregion Akcesory

	/**
	 * @fn	Void PictureParameters::initWithDefaults( );
	 *
	 * @brief	Inicjalizacja pól domyœlnymi wartoœciami.
	 *
	 * @return	Void.
	 */

	Void initWithDefaults( );
};

/**
 * @fn	PictureParameters* PicParams( );
 *
 * @brief	Globalna funkcja dostêpu do singletona PictureParameters.
 *
 * @return	WskaŸnik na instancjê PictureParameters.
 */

PictureParameters* PicParams( );

/**
 * @fn	SequenceParameters* SeqParams( );
 *
 * @brief	Globalna funkcja dostêpu do singletona SequenceParameters.
 *
 * @return	WskaŸnik na instancjê SequenceParameters.
 */

SequenceParameters* SeqParams( );

/**
 * @fn	UInt getRasterIdxIn4x4( UInt x, UInt y );
 *
 * @brief	Indeks bloku 4x4 w kolejnosci rastrowej na podst. po³o¿enia próbki luminancji.
 *
 * @param	x	Wspó³rzêdna X próbki.
 * @param	y	Wspó³rzêdna Y próbki.
 *
 * @return	Indeks kolejnoœci rastrowej.
 */

UInt getRasterIdxIn4x4( UInt x, UInt y );

/**
 * @fn	UInt getZScanIdxIn4x4( const UInt puX, const UInt puY );
 *
 * @brief	Indeks bloku 4x4 w kolejnosci Z-skanowania na podst. po³o¿enia bloku predykcji.
 *
 * @param	puX	Wspó³rzêdna X bloku predykcji.
 * @param	puY	Wspó³rzêdna X bloku predykcji.
 *
 * @return	Indeks kolejnoœci z-skanowania.
 */

UInt getZScanIdxIn4x4( const UInt puX, const UInt puY );
#endif // SeqPicParameters_h__