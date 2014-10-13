/**
 * @file	TU.h
 *
 * @brief Deklaracja klas jednostki i bloku transformacji.
 */

#ifndef TU_h__
#define TU_h__

#include "Utils.h"
#include "Binarization.h"
#include "UnitBase.h"
#include "RDCost.h"
#include "Binarization.h"
#include <boost/dynamic_bitset.hpp>

class PUIntra;

class TU;

class CU;

class PBIntra;

/**
 * @class	TB
 *
 * @brief Klasa bloku transformacji.
 */

class TB : public UnitBase
{
	/**
	 * @brief	Sk³adowa obrazu.
	 */

	ImgComp itsComp;

	/**
	 * @brief	Nadrzêdna jednostka transformacji. 
	 */

	TU* itsParentTU;

	/**
	 * @brief	Macierz próbek oryginalnych.
	 */

	Submatrix<Sample> itsPictureSamples;

	/**
	 * @brief	WskaŸnik na macierz predykcji.
	 */

	Submatrix<Sample> itsPrediction;

	/**
	 * @brief	Próbki ró¿nicowe poddawane transformacji.
	 */

	Sample** itsResiduals;

	/**
	 * @brief	Macierz rekonstrukcji.
	 */

	Submatrix<Sample> itsReconstructionTarget;

	/**
	 * @brief	Skwantowane wspó³æzynniki transformacji.
	 */

	Coeff** itsCoefficients;

	/**
	 * @brief	Podleg³y blok predykcji.
	 */

	std::shared_ptr<PBIntra> itsHelperPB;

	/**
	 * @brief	WskaŸnik na strategiê skanowania wspó³czynników.
	 */

	ScanningMode* itsScanStrategy;

	/**
	 * @property	UInt itsLastSignificantCoeffX, itsLastSignificantCoeffY
	 *
	 * @brief	Wspó³rzêdne ostatniego niezerowego wspóæ³zynnika.
	 */

	UInt itsLastSignificantCoeffX, itsLastSignificantCoeffY;

	/**
	 * @brief Flagi znacz¹cych grup CG.
	 */

	boost::dynamic_bitset<> itsCodedSubBlockFlags;

	/**
	 * @brief	Zniekszta³cenie w tym bloku.
	 */

	UInt itsDistortion;

	/**
	 * @brief	Iloœæ binów reprezentuj¹cych ten blok.
	 */

	UInt itsNumBins;

public:

	/**
	 * @fn	TB::TB( TU* parentTU, UShort x, UShort y, UShort size, ImgComp comp = LUMA );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	parentTU	WskaŸnik na nadrzêdn¹ jednostkê transformacji.
	 * @param	x										Wspó³rzêdna X bloku.
	 * @param	y										Wspó³rzêdna Y bloku.
	 * @param	size								Rozmiar bloku.
	 * @param	comp								Sk³adowa obrazu.
	 */

	TB( TU* parentTU, UShort x, UShort y, UShort size, ImgComp comp = LUMA );

	/**
	 * @fn	TB::~TB( );
	 *
	 * @brief	Destruktor.
	 */

	~TB( );

	/**
	 * @fn	Void TB::transformAndReconstruct( std::shared_ptr<PUIntra> mainPU );
	 *
	 * @brief	Pêtla rekonstrukcji bloku.
	 *
	 * @param	mainPU	PU, w której znajduje siê obecny blok.
	 */

	Void transformAndReconstruct( std::shared_ptr<PUIntra> mainPU );

	/**
	 * @fn	Void TB::assignScanStrategy( );
	 *
	 * @brief	Przypisanie strategii skanowania.
	 */

	Void assignScanStrategy( );

	/**
	 * @fn	virtual Void TB::printDescription( );
	 *
	 * @brief	Wypisuje opis struktury bloku.
	 */

	virtual Void printDescription( );

	/**
	 * @fn	ScanningMode* TB::getScanStrategy( ) const
	 *
	 * @brief	Zwraca przypisan¹ temu blokowi strategiê skanowania.
	 *
	 * @return	WskaŸnik na strategiê skanowania.
	 */

	ScanningMode* getScanStrategy( ) const
	{
		return itsScanStrategy;
	}

	/**
	 * @fn	std::shared_ptr<PBIntra> TB::getHelperPB( ) const
	 *
	 * @brief	Dostêp do pomocniczego bloku predykcji dla tego TB.
	 *
	 * @return	WskaŸnik na podleg³y PB.
	 */

	std::shared_ptr<PBIntra> getHelperPB( ) const
	{
		return itsHelperPB;
	}

	/**
	 * @fn	Void TB::setHelperPB( std::shared_ptr<PBIntra> val )
	 *
	 * @brief	Ustawia pomocniczy blok predykcji.
	 *
	 * @param	val	Nowy blok predykcji.
	 */

	Void setHelperPB( std::shared_ptr<PBIntra> val )
	{
		itsHelperPB = val;
	}

	/**
	 * @fn	UInt TB::getDistortion( )
	 *
	 * @brief	Zwraca zniekszta³cenie w bloku.
	 *
	 * @return	Wartoœæ zniekszta³cenia.
	 */

	UInt getDistortion( )
	{
			calcDistortion( );
		return itsDistortion;
	}

	/**
	 * @fn	UInt TB::getNumBins( )
	 *
	 * @brief	Zwraca liczbê binów reprezentuj¹cych blok.
	 *
	 * @return	Liczba binów.
	 */

	UInt getNumBins( )
	{
			calcNumBins( );
		return itsNumBins;
	}

	/**
	 * @fn	Void TB::calcDistortion( )
	 *
	 * @brief	Oblicza zniekszta³cenie w bloku.
	 */

	Void calcDistortion( )
	{
		itsDistortion = RDCost::getInstance( )->calcDistortion( itsPictureSamples, itsReconstructionTarget );
	}

	/**
	 * @fn	Void TB::calcNumBins( )
	 *
	 * @brief	Oblicza iloœæ binów potrzebn¹ do zakodowania bloku.
	 */

	Void calcNumBins( )
	{
		assignScanStrategy( );
		itsNumBins = Binarization::getInstance()->countBinsInTB( itsCoefficients, itsSize, itsScanStrategy );
	}

	/**
	 * @fn	Coeff** TB::getCoefficients( ) const
	 *
	 * @brief	Dostêp do macierzy wspó³æzynników transformacty.
	 *
	 * @return	Macierz wspó³æzynników transformaty.
	 */

	Coeff** getCoefficients( ) const
	{
		return itsCoefficients;
	}

	/**
	 * @fn	void TB::forceCoefficients( Coeff** val )
	 *
	 * @brief	Nadpisuje macierz wpsó³czynników transformaty.
	 *
	 * @param [in]	val Nowa macierz wspó³czynników.
	 */

	void forceCoefficients( Coeff** val )
	{
		itsCoefficients = val;
	}

	/**
	 * @fn	Bool TB::hasAtLeastOneNonZeroCoeff( );
	 *
	 * @brief	Sprawdza, czy blok zawiera co najmniej jeden niezerowy wspó³czynnik.
	 *
	 * @return	true, jeœli blok zawiera co najmniej jeden niezerowy wspó³czynnik; false w p.p.
	 */

	Bool hasAtLeastOneNonZeroCoeff( );

};

/**
 * @class	TU
 *
 * @brief	Klasa jednostki transformacji.
 */

class TU : public CTUComponentBase
{
private:

	/**
	 * @brief	Zmiana QP w stosunku do wartoœci na poziomie CU.
	 */

	Int itsQPDeltaFromCU;

	/**
	 * @brief	Wynikowa wartoœæ QP dla jednostki.
	 */

	UInt itsQP;

	/**
	 * @brief	G³êbia po³o¿enia w drzewie transformacji.
	 */

	UInt transformDepth;

	/**
	 * @brief	Flagi mówi¹ce, czy blok danej sk³adowej obrazu ma jakieœ niezerowe wspó³æzynniki transformaty.
	 */

	Bool itsCBFs[ 3 ];

	/**
	 * @property	std::vector<std::shared_ptr<TB> > itsTBs
	 *
	 * @brief	Lista bloków transformacji podleg³a pod t¹ jednostkê.
	 */

	std::vector<std::shared_ptr<TB> > itsTBs;

	/**
	 * @brief	Nadrzêdna jednostka CU.
	 */

	CU* itsParentCU;

public:

	/**
	 * @fn	TU::TU( CU* parentCU, UInt x, UInt y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	parentCU	WskaŸnik na nadrzêdn¹ jednostkê CU.
	 * @param	x										Wspó³rzêdna X jednostki TU.
	 * @param	y										Wspó³rzêdna Y jednostki TU.
	 * @param	size								Rozmiar jednostki TU.
	 */

	TU( CU* parentCU, UInt x, UInt y, UInt size );

	/**
	 * @fn	TU::~TU( )
	 *
	 * @brief	Destruktor.
	 */

	~TU( )
	{
	}

	/**
	 * @fn	virtual Void TU::printDescription( );
	 *
	 * @brief	Wypisuje opis struktury TU.
	 */

	virtual Void printDescription( );

	/**
	 * @fn	Void TU::reconstructionLoop( std::shared_ptr<PUIntra> mainPU )
	 *
	 * @brief	Pêtla rekonstrukcji dla jednostki TU.
	 *
	 * @param	mainPU	WskaŸnik na aktualn¹ jednostkê predykcji.
	 */

	Void reconstructionLoop( std::shared_ptr<PUIntra> mainPU )
	{
		for( UInt i = 0; i < itsTBs.size( ); ++i )
			itsTBs[ i ]->transformAndReconstruct( mainPU );
	}

	/**
	 * @fn	CU* TU::getParentCU( )
	 *
	 * @brief	Dostêp do nadrzêdnej jednostki CU.
	 *
	 * @return	WskaŸnik na CU-rodzica.
	 */

	CU* getParentCU( )
	{
		return itsParentCU;
	}

	/**
	 * @fn	std::shared_ptr<TB> TU::getTB(ImgComp comp ) const
	 *
	 * @brief	Dostêp do bloków transformacji.
	 *
	 * @param	comp	Sk³adowa obrazu.
	 *
	 * @return	WskaŸnik na odpowiedni blok transformacji.
	 */

	std::shared_ptr<TB> getTB(ImgComp comp ) const
	{
		return itsTBs[comp];
	}

};
#endif // TU_h__