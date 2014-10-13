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
	 * @brief	Sk�adowa obrazu.
	 */

	ImgComp itsComp;

	/**
	 * @brief	Nadrz�dna jednostka transformacji. 
	 */

	TU* itsParentTU;

	/**
	 * @brief	Macierz pr�bek oryginalnych.
	 */

	Submatrix<Sample> itsPictureSamples;

	/**
	 * @brief	Wska�nik na macierz predykcji.
	 */

	Submatrix<Sample> itsPrediction;

	/**
	 * @brief	Pr�bki r�nicowe poddawane transformacji.
	 */

	Sample** itsResiduals;

	/**
	 * @brief	Macierz rekonstrukcji.
	 */

	Submatrix<Sample> itsReconstructionTarget;

	/**
	 * @brief	Skwantowane wsp��zynniki transformacji.
	 */

	Coeff** itsCoefficients;

	/**
	 * @brief	Podleg�y blok predykcji.
	 */

	std::shared_ptr<PBIntra> itsHelperPB;

	/**
	 * @brief	Wska�nik na strategi� skanowania wsp�czynnik�w.
	 */

	ScanningMode* itsScanStrategy;

	/**
	 * @property	UInt itsLastSignificantCoeffX, itsLastSignificantCoeffY
	 *
	 * @brief	Wsp�rz�dne ostatniego niezerowego wsp��zynnika.
	 */

	UInt itsLastSignificantCoeffX, itsLastSignificantCoeffY;

	/**
	 * @brief Flagi znacz�cych grup CG.
	 */

	boost::dynamic_bitset<> itsCodedSubBlockFlags;

	/**
	 * @brief	Zniekszta�cenie w tym bloku.
	 */

	UInt itsDistortion;

	/**
	 * @brief	Ilo�� bin�w reprezentuj�cych ten blok.
	 */

	UInt itsNumBins;

public:

	/**
	 * @fn	TB::TB( TU* parentTU, UShort x, UShort y, UShort size, ImgComp comp = LUMA );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	parentTU	Wska�nik na nadrz�dn� jednostk� transformacji.
	 * @param	x										Wsp�rz�dna X bloku.
	 * @param	y										Wsp�rz�dna Y bloku.
	 * @param	size								Rozmiar bloku.
	 * @param	comp								Sk�adowa obrazu.
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
	 * @brief	P�tla rekonstrukcji bloku.
	 *
	 * @param	mainPU	PU, w kt�rej znajduje si� obecny blok.
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
	 * @brief	Zwraca przypisan� temu blokowi strategi� skanowania.
	 *
	 * @return	Wska�nik na strategi� skanowania.
	 */

	ScanningMode* getScanStrategy( ) const
	{
		return itsScanStrategy;
	}

	/**
	 * @fn	std::shared_ptr<PBIntra> TB::getHelperPB( ) const
	 *
	 * @brief	Dost�p do pomocniczego bloku predykcji dla tego TB.
	 *
	 * @return	Wska�nik na podleg�y PB.
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
	 * @brief	Zwraca zniekszta�cenie w bloku.
	 *
	 * @return	Warto�� zniekszta�cenia.
	 */

	UInt getDistortion( )
	{
			calcDistortion( );
		return itsDistortion;
	}

	/**
	 * @fn	UInt TB::getNumBins( )
	 *
	 * @brief	Zwraca liczb� bin�w reprezentuj�cych blok.
	 *
	 * @return	Liczba bin�w.
	 */

	UInt getNumBins( )
	{
			calcNumBins( );
		return itsNumBins;
	}

	/**
	 * @fn	Void TB::calcDistortion( )
	 *
	 * @brief	Oblicza zniekszta�cenie w bloku.
	 */

	Void calcDistortion( )
	{
		itsDistortion = RDCost::getInstance( )->calcDistortion( itsPictureSamples, itsReconstructionTarget );
	}

	/**
	 * @fn	Void TB::calcNumBins( )
	 *
	 * @brief	Oblicza ilo�� bin�w potrzebn� do zakodowania bloku.
	 */

	Void calcNumBins( )
	{
		assignScanStrategy( );
		itsNumBins = Binarization::getInstance()->countBinsInTB( itsCoefficients, itsSize, itsScanStrategy );
	}

	/**
	 * @fn	Coeff** TB::getCoefficients( ) const
	 *
	 * @brief	Dost�p do macierzy wsp��zynnik�w transformacty.
	 *
	 * @return	Macierz wsp��zynnik�w transformaty.
	 */

	Coeff** getCoefficients( ) const
	{
		return itsCoefficients;
	}

	/**
	 * @fn	void TB::forceCoefficients( Coeff** val )
	 *
	 * @brief	Nadpisuje macierz wps�czynnik�w transformaty.
	 *
	 * @param [in]	val Nowa macierz wsp�czynnik�w.
	 */

	void forceCoefficients( Coeff** val )
	{
		itsCoefficients = val;
	}

	/**
	 * @fn	Bool TB::hasAtLeastOneNonZeroCoeff( );
	 *
	 * @brief	Sprawdza, czy blok zawiera co najmniej jeden niezerowy wsp�czynnik.
	 *
	 * @return	true, je�li blok zawiera co najmniej jeden niezerowy wsp�czynnik; false w p.p.
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
	 * @brief	Zmiana QP w stosunku do warto�ci na poziomie CU.
	 */

	Int itsQPDeltaFromCU;

	/**
	 * @brief	Wynikowa warto�� QP dla jednostki.
	 */

	UInt itsQP;

	/**
	 * @brief	G��bia po�o�enia w drzewie transformacji.
	 */

	UInt transformDepth;

	/**
	 * @brief	Flagi m�wi�ce, czy blok danej sk�adowej obrazu ma jakie� niezerowe wsp��zynniki transformaty.
	 */

	Bool itsCBFs[ 3 ];

	/**
	 * @property	std::vector<std::shared_ptr<TB> > itsTBs
	 *
	 * @brief	Lista blok�w transformacji podleg�a pod t� jednostk�.
	 */

	std::vector<std::shared_ptr<TB> > itsTBs;

	/**
	 * @brief	Nadrz�dna jednostka CU.
	 */

	CU* itsParentCU;

public:

	/**
	 * @fn	TU::TU( CU* parentCU, UInt x, UInt y, UInt size );
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	parentCU	Wska�nik na nadrz�dn� jednostk� CU.
	 * @param	x										Wsp�rz�dna X jednostki TU.
	 * @param	y										Wsp�rz�dna Y jednostki TU.
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
	 * @brief	P�tla rekonstrukcji dla jednostki TU.
	 *
	 * @param	mainPU	Wska�nik na aktualn� jednostk� predykcji.
	 */

	Void reconstructionLoop( std::shared_ptr<PUIntra> mainPU )
	{
		for( UInt i = 0; i < itsTBs.size( ); ++i )
			itsTBs[ i ]->transformAndReconstruct( mainPU );
	}

	/**
	 * @fn	CU* TU::getParentCU( )
	 *
	 * @brief	Dost�p do nadrz�dnej jednostki CU.
	 *
	 * @return	Wska�nik na CU-rodzica.
	 */

	CU* getParentCU( )
	{
		return itsParentCU;
	}

	/**
	 * @fn	std::shared_ptr<TB> TU::getTB(ImgComp comp ) const
	 *
	 * @brief	Dost�p do blok�w transformacji.
	 *
	 * @param	comp	Sk�adowa obrazu.
	 *
	 * @return	Wska�nik na odpowiedni blok transformacji.
	 */

	std::shared_ptr<TB> getTB(ImgComp comp ) const
	{
		return itsTBs[comp];
	}

};
#endif // TU_h__