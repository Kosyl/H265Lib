/**
 * @file	SeqPicParameters.h
 *
 * @brief	Deklaracje klas parametr�w.
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
	 * @brief	Warto�ci indeks�w.
	 */

	static Int** itsValues;

	/**
	 * @property	static UInt itsSizeX, itsSizeY
	 *
	 * @brief	Rozmiar macierzy indeks�w.
	 *
	 */

	static UInt itsSizeX, itsSizeY;

public:

	/**
	 * @fn	static Void ZScanArray::reset( );
	 *
	 * @brief	Resetuje macierz indeks�w.
	 *
	 * @return	Void.
	 */

	static Void reset( );

	/**
	 * @fn	static Int ZScanArray::get( UInt x, UInt y );
	 *
	 * @brief	Pobiera indeks Z-skanowania dla danej pozycji (wyra�onej w blokach 4x4).
	 *
	 * @param	x	Wsp�rz�dna X bloku 4x4.
	 * @param	y	Wsp�rz�dna Y bloku 4x4.
	 *
	 * @return	Indeks Z-skanowania.
	 */

	static Int get( UInt x, UInt y );

	/**
	 * @fn	static Void ZScanArray::print( std::ostream& out );
	 *
	 * @brief	Wypisuje macierz indeks�w Z-skanowania dla obrazu.
	 *
	 * @param [in]	Strumie� wyj�ciowy
	 *
	 * @return	Void.
	 */

	static Void print( std::ostream& out );
};

/**
 * @class	SequenceParameters
 *
 * @brief	Klasa parametr�w sekwencji.
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
	 * @brief	Konstruktor domy�lny.
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
	 * @brief	Logarytmy dw�jkowe z rozmiar�w poszczeg�lnych blok�w.
	 *
	 */

	UShort itsLog2CTUSize, itsLog2MaxCUSize, itsLog2MinCUSize, itsLog2MaxTUSize, itsLog2MinTUSize;

	/**
	 * @property	UShort itsBitDepthLuma, itsBitDepthChroma
	 *
	 * @brief	G��bie bitowe pr�bek sk�adowych obrazu.
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
	 * @brief	W��czenie/wy�aczenie filtru SAO.
	 */

	Bool itsSAOEnabled;

	/**
	 * @brief	W��czenie/wy��czenie silnej filtracji pr�bek w predykcji intra
	 */

	Bool itsIntraSmoothingEnabled;

public:

	/**
	 * @fn	static SequenceParameters* SequenceParameters::getInstance( );
	 *
	 * @brief	Dost�p do instancji parametr�w sekwencji.
	 *
	 * @return	Wska�nik na instancj�.
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
	 * @brief	Szeroko�� obrazu.
	 */

	UShort getPicWidth( ) const
	{
		return itsPicWidth;
	}

	/**
	 * @fn	UShort SequenceParameters::getPicWidthInCTUs( ) const
	 *
	 * @brief	Szeroko�� obrazu wyra�ona w CTU.
	 */

	UShort getPicWidthInCTUs( ) const
	{
		return itsPicWidthInCTUs;
	}

	/**
	 * @fn	Void SequenceParameters::setPicWidth( UShort val )
	 *
	 * @brief	Ustawia szeroko�� obrazu.
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
	 * @brief	Wysokos� obrazu.
	 */

	UShort getPicHeight( ) const
	{
		return itsPicHeight;
	}

	/**
	 * @fn	UShort SequenceParameters::getPicHeightInCTUs( ) const
	 *
	 * @brief	Wysoko�� obrazu wyra�ona w CTU.
	 */

	UShort getPicHeightInCTUs( ) const
	{
		return itsPicHeightInCTUs;
	}

	/**
	 * @fn	Void SequenceParameters::setPicHeight( UShort val )
	 *
	 * @brief Ustawia wysokos� obrazu.
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
	 * @brief	G��bia bitowa chrominancji.
	 */

	UShort getBitDepthChroma( ) const
	{
		return itsBitDepthChroma;
	}

	/**
	 * @fn	Void SequenceParameters::setBitDepthChroma( UShort val )
	 *
	 * @brief	Ustawia g��bie bitow� chrominancji.
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
	 * @brief	G��bia bitowa luminancji.
	 */

	UShort getBitDepthLuma( ) const
	{
		return itsBitDepthLuma;
	}

	/**
	 * @fn	Void SequenceParameters::setBitDepthLuma( UShort val )
	 *
	 * @brief	Ustawia g�ebi� bitow� luminancji.
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
	 * @brief	G��bia bitowa dla sk�adowej obrazu.
	 *
	 * @param	comp	Sk��dowa obrazu.
	 *
	 * @return	G��bia bitowa.
	 */

	UShort getBitDepth( ImgComp comp ) const
	{
		return comp == LUMA ? itsBitDepthLuma : itsBitDepthChroma;
	}

	/**
	 * @fn	Void SequenceParameters::setBitDepth( ImgComp comp, UShort val )
	 *
	 * @brief	Ustawia g��bi� bitow�.
	 *
	 * @param	comp	Sk�adowa obrazu.
	 * @param	val 	Warto�� g��bii bitowej.
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
	 * @brief	Pozwolenie na u�ycie SAO
	 */

	Bool getSAOEnabled( ) const
	{
		return itsSAOEnabled;
	}

	/**
	 * @fn	Void SequenceParameters::setSAOEnabled( Bool val )
	 *
	 * @brief	Ustawia pozwolenie na u�ycie SAO.
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
	 * @brief	Najwi�kszy rozmiar CU.
	 */

	UShort getMaxCUSize( ) const
	{
		return itsMaxCUSize;
	}

	/**
	 * @fn	Void SequenceParameters::setMaxCUSize( UShort val )
	 *
	 * @brief	Ustawia najwi�kszy rozmiar CU.
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
	 * @brief	Zezwolenie na u�ycie silnej filtracji w predykcji intra.
	 */

	Bool getIntraSmoothingEnabled( ) const
	{
		return itsIntraSmoothingEnabled;
	}

	/**
	 * @fn	Void SequenceParameters::setIntraSmoothingEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na u�ycie silnej filtracji w predykcji intra.
	 *
	 * @param	val	true pozwala na siln� filtracj�; false nie.
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
	 * @brief	najwi�kszy rozmiar TU.
	 */

	UShort getMaxTUSize( ) const
	{
		return itsMaxTUSize;
	}

	/**
	 * @fn	Void SequenceParameters::setMaxTUSize( UShort val )
	 *
	 * @brief	Ustawia najwi�kszy mo�liwy rozmiar TU.
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
	 * @brief	Obcina podana warto�� do zakresu odpowiadaj�cemu g��bii bitowej wybranej sk�adowej obrazu.
	 *
	 * @param	comp 	Sk�adowa obrazu.
	 * @param	value	Warto�� do obci�cia.
	 *
	 * @return Obci�ta warto��.
	 */

	Sample clip( ImgComp comp, Int value )
	{
		UShort maxVal = comp == LUMA ? 1 << itsBitDepthLuma : 1 << itsBitDepthChroma;
		return clipToRange<Sample>( 0, maxVal, value );
	}

	/**
	 * @fn	Sample SequenceParameters::getDefaultSampleValue( ImgComp comp )
	 *
	 * @brief Domy�lna warto�� pr�bki danej sk�adowej obrazu.
	 *
	 * @param	comp	Sk�adowa obrazu.
	 * 							
	 * @return Domy�lna warto�� pr�bki dla danej sk�adowej.
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
	 * @brief	Logarytm dw�jkowy z najmniejszego rozmiaru TU
	 */

	UShort getLog2MinTUSize( ) const
	{
		return itsLog2MinTUSize;
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2MaxTUSize( ) const
	 *
	 * @brief	Logarytm dw�jkowy z najwi�kszego rozmiaru TU
	 */

	UShort getLog2MaxTUSize( ) const
	{
		return itsLog2MaxTUSize;
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2MinCUSize( ) const
	 *
	 * @brief	Logarytm dw�jkowy z najmniejszego rozmiaru CU
	 */

	UShort getLog2MinCUSize( ) const
	{
		return itsLog2MinCUSize;
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2MaxCUSize( ) const
	 *
	 * @brief	Logarytm dw�jkowy z najwi�kszego rozmiaru CU
	 */

	UShort getLog2MaxCUSize( ) const
	{
		return itsLog2MaxCUSize;
	}

	/**
	 * @fn	UShort SequenceParameters::getLog2CTUSize( ) const
	 *
	 * @brief	Logarytm dw�jkowy z rozmiaru CTU
	 */

	UShort getLog2CTUSize( ) const
	{
		return itsLog2CTUSize;
	}

	/**
	 * @fn	Void SequenceParameters::initWithDefaults( );
	 *
	 * @brief	Inicjalizacja parametr�w domy�lnymi warto�ciami (CTU size = CUmax size = 64, CUmin size = 8, TUmax size = 32, TUmin size = 4, LumaDepth = ChromaDepth = 8).
	 *
	 * @return	Void.
	 */

	Void initWithDefaults( );
};

/**
 * @class	PictureParameters
 *
 * @brief Klasa parametr�w obrazu.
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
	 * @brief	Konstruktor domy�lny.
	 */

	PictureParameters( );

	/**
	 * @brief	Flaga w��czenia trybu SDH.
	 */

	Bool itsSDHEnabled;

	/**
	 * @brief	Parametr kwantyzacji.
	 */

	UShort itsQP;

	/**
	 * @property	Short itsQPOffsetForCb, itsQPOffsetForCr
	 *
	 * @brief	Offsety parametru kwantyzacji dla sk�adowych chrominancji.
	 *
	 */

	Short itsQPOffsetForCb, itsQPOffsetForCr;

	/**
	 * @brief	Flaga zezwolenia na tryb TransformSkip.
	 */

	Bool itsTransformSkipEnabled;

	/**
	 * @brief	Zezwolenie na u�ycie kafli.
	 */

	Bool itsTilesEnabled;

	/**
	 * @brief	Zezwolenie na kontrol� filtru deblokuj�cego przez plastry.
	 */

	Bool itsDeblockingFilterControlEnabled;

	/**
	 * @property	Bool itsLoopFilterOnSlicesEnabled, itsLoopFilterOnTilesEnabled
	 *
	 * @brief	Flagi zezwalaj�ce na filtracj� deblokuj�c� mi�dzy plastrami i kaflami.
	 *
	 */

	Bool itsLoopFilterOnSlicesEnabled, itsLoopFilterOnTilesEnabled;

public:

	/**
	 * @fn	static PictureParameters* PictureParameters::getInstance( );
	 *
	 * @brief Dost�p do instancji parametr�w sekwencji.
	 *
	 * @return	Wska�nik na instancj� parametr�w obrazu.
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
	 * @brief	Zezwolenie na FDB na granicach mi�dzy kaflami.
	 */

	Bool getLoopFilterOnTilesEnabled( ) const
	{
		return itsLoopFilterOnTilesEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setLoopFilterOnTilesEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na FDB na granicach mi�dzy kaflami.
	 *
	 * @param	val	true w��cza, false wy��cza FDB.
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
	 * @brief	Zezwolenie na FDB na granicach mi�dzy plastrami.
	 *
	 */

	Bool getLoopFilterOnSlicesEnabled( ) const
	{
		return itsLoopFilterOnSlicesEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setLoopFilterOnSlicesEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na FDB na granicach mi�dzy plastrami.
	 *
	 * @param	val	true w��cza, false wy��cza FDB.
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
	 * @brief	Zezwolenie na kontrol� FDB przez plastry.
	 */

	Bool getDeblockingFilterControlEnabled( ) const
	{
		return itsDeblockingFilterControlEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setDeblockingFilterControlEnabled( Bool val )
	 *
	 * @brief		Ustawia zezwolenie na kontrol� FDB przez plastry.
	 *
	 * @param	val	true w��cza, false wy��cza kontrol� FDB.
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
	 * @param	val	true w��cza, false wy��cza tryb SDH.
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
	 * @brief	Zwraca QP dla danej sk�adowej obrazu.
	 *
	 * @param	comp	Sk�adowa obrazu.
	 *
	 * @return	Warto�� QP na poziomie parametr�w obrazu.
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
	 * @brief	Ustawia bazow� warto�� QP.
	 *
	 * @param	val	Nowa warto�� QP.
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
	 * @param	val wybrany offset wzgl�dem warto�ci bazowej.
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
	 * @param	val wybrany offset wzgl�dem warto�ci bazowej.
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
	 * @brief	Zezwolenie na u�ycie kafli.
	 */

	Bool getTilesEnabled( ) const
	{
		return itsTilesEnabled;
	}

	/**
	 * @fn	Void PictureParameters::setTilesEnabled( Bool val )
	 *
	 * @brief	Ustawia zezwolenie na u�ycie kafli.
	 *
	 * @param	val	true zezwala, false blokuje u�ycie kafli.
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
	 * @brief	Inicjalizacja p�l domy�lnymi warto�ciami.
	 *
	 * @return	Void.
	 */

	Void initWithDefaults( );
};

/**
 * @fn	PictureParameters* PicParams( );
 *
 * @brief	Globalna funkcja dost�pu do singletona PictureParameters.
 *
 * @return	Wska�nik na instancj� PictureParameters.
 */

PictureParameters* PicParams( );

/**
 * @fn	SequenceParameters* SeqParams( );
 *
 * @brief	Globalna funkcja dost�pu do singletona SequenceParameters.
 *
 * @return	Wska�nik na instancj� SequenceParameters.
 */

SequenceParameters* SeqParams( );

/**
 * @fn	UInt getRasterIdxIn4x4( UInt x, UInt y );
 *
 * @brief	Indeks bloku 4x4 w kolejnosci rastrowej na podst. po�o�enia pr�bki luminancji.
 *
 * @param	x	Wsp�rz�dna X pr�bki.
 * @param	y	Wsp�rz�dna Y pr�bki.
 *
 * @return	Indeks kolejno�ci rastrowej.
 */

UInt getRasterIdxIn4x4( UInt x, UInt y );

/**
 * @fn	UInt getZScanIdxIn4x4( const UInt puX, const UInt puY );
 *
 * @brief	Indeks bloku 4x4 w kolejnosci Z-skanowania na podst. po�o�enia bloku predykcji.
 *
 * @param	puX	Wsp�rz�dna X bloku predykcji.
 * @param	puY	Wsp�rz�dna X bloku predykcji.
 *
 * @return	Indeks kolejno�ci z-skanowania.
 */

UInt getZScanIdxIn4x4( const UInt puX, const UInt puY );
#endif // SeqPicParameters_h__