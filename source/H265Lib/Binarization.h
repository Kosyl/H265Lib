/**
* @file	Binarization.h
*
* @brief	Deklaracja klasy kalkulatora binaryzacji.
*/
#ifndef BINARIZATION_H
#define BINARIZATION_H

#include "Utils.h"
#include <bitset>
#include <boost\dynamic_bitset.hpp>

class Binarization;

class CoeffGroup;

class HorizontalScanningMode;

class VerticalScanningMode;

class DiagonalScanningMode;

/**
* @class	ScanningMode
*
* @brief	Klasa bazowa tryb�w skanowania
*/

class ScanningMode
{
public:
	static const Int CG_SIZE;

	/**
	* @fn	ScanningMode::ScanningMode( );
	*
	* @brief	Konstruktor domy�lny
	*/

	ScanningMode( );

	/**
	* @fn	virtual ScanningMode::~ScanningMode( );
	*
	* @brief	Destruktor.
	*/

	virtual ~ScanningMode( );

	/**
	* @fn	virtual std::vector<std::shared_ptr<CoeffGroup> > ScanningMode::scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY ) = 0;
	*
	* @brief	Scans.
	*
	* @param [in]	coeffs					Macierz wsp�czynnik�w
	* @param	size									Rozmiar macierzy wsp�czynnik�w
	* @param [out]	out_lastCoeffX	Wsp�rz�dna X ostatniego niezerowego wsp�czynnika
	* @param [out]	out_lastCoeffY	Wsp�rz�dna Y ostatniego niezerowego wsp�czynnika
	*
	* @return	Wektor wska�nik�w na obiekty CoeffGroup reprezentuj�ce podbloki 4x4
	*/

	virtual std::vector<std::shared_ptr<CoeffGroup> > scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY ) = 0;

};

/**
* @class	HorizontalScanningMode
*
* @brief	Strategia skanowania poziomego
*/

class HorizontalScanningMode : public ScanningMode
{
public:
	/**
	* @fn	ScanningMode::ScanningMode( );
	*
	* @brief	Konstruktor domy�lny
	*/

	HorizontalScanningMode( );
	/**
	* @fn	virtual ScanningMode::~ScanningMode( );
	*
	* @brief	Destruktor.
	*/

	~HorizontalScanningMode( );

	/**
	* @fn	virtual std::vector<std::shared_ptr<CoeffGroup> > ScanningMode::scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY ) = 0;
	*
	* @brief	Scans.
	*
	* @param [in]	coeffs					Macierz wsp�czynnik�w
	* @param	size									Rozmiar macierzy wsp�czynnik�w
	* @param [out]	out_lastCoeffX	Wsp�rz�dna X ostatniego niezerowego wsp�czynnika
	* @param [out]	out_lastCoeffY	Wsp�rz�dna Y ostatniego niezerowego wsp�czynnika
	*
	* @return	Wektor wska�nik�w na obiekty CoeffGroup reprezentuj�ce podbloki 4x4
	*/

	std::vector<std::shared_ptr<CoeffGroup> > scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY );

};

/**
* @class	VerticalScanningMode
*
* @brief	A vertical scanning mode.
*/

class VerticalScanningMode : public ScanningMode
{
public:
	/**
	* @fn	ScanningMode::ScanningMode( );
	*
	* @brief	Konstruktor domy�lny
	*/

	VerticalScanningMode( );
	/**
	* @fn	virtual ScanningMode::~ScanningMode( );
	*
	* @brief	Destruktor.
	*/

	~VerticalScanningMode( );

	/**
	* @fn	virtual std::vector<std::shared_ptr<CoeffGroup> > ScanningMode::scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY ) = 0;
	*
	* @brief	Scans.
	*
	* @param [in]	coeffs					Macierz wsp�czynnik�w
	* @param	size									Rozmiar macierzy wsp�czynnik�w
	* @param [out]	out_lastCoeffX	Wsp�rz�dna X ostatniego niezerowego wsp�czynnika
	* @param [out]	out_lastCoeffY	Wsp�rz�dna Y ostatniego niezerowego wsp�czynnika
	*
	* @return	Wektor wska�nik�w na obiekty CoeffGroup reprezentuj�ce podbloki 4x4
	*/

	std::vector<std::shared_ptr<CoeffGroup> > scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY );

};

/**
* @class	DiagonalScanningMode
*
* @brief	A diagonal scanning mode.
*/

class DiagonalScanningMode : public ScanningMode
{
public:
	/**
	* @fn	ScanningMode::ScanningMode( );
	*
	* @brief	Konstruktor domy�lny
	*/

	DiagonalScanningMode( );
	/**
	* @fn	virtual ScanningMode::~ScanningMode( );
	*
	* @brief	Destruktor.
	*/

	~DiagonalScanningMode( );

	const static UInt coeffIdxInCG[ 2 ][ 16 ];

	/**
	* @fn	virtual std::vector<std::shared_ptr<CoeffGroup> > ScanningMode::scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY ) = 0;
	*
	* @brief	Scans.
	*
	* @param [in]	coeffs					Macierz wsp�czynnik�w
	* @param	size									Rozmiar macierzy wsp�czynnik�w
	* @param [out]	out_lastCoeffX	Wsp�rz�dna X ostatniego niezerowego wsp�czynnika
	* @param [out]	out_lastCoeffY	Wsp�rz�dna Y ostatniego niezerowego wsp�czynnika
	*
	* @return	Wektor wska�nik�w na obiekty CoeffGroup reprezentuj�ce podbloki 4x4
	*/

	std::vector<std::shared_ptr<CoeffGroup> > scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY );

};

/**
* @class	Binarization
*
* @brief	Kalkulator binaryzacji element�w i zliczania bin�w
*/

class Binarization
{
private:

	static const UInt MinInGroup[ 10 ];

	static const UInt GroupIdx[ 32 ];

	/**
	 * @brief	Instancja singletona.
	 */

	static Binarization* instance;

	/**
	 * @brief	Flaga m�wi�ca, czy obliczenia dla TB maj� zak�ada� mo�liwo�� u�ycia trybu TransQuantBypass
	 */

	Bool itsCurrentCUUsesTransQuantBypass;

	/**
	 * @brief	Flaga m�wi�ca, czy obliczany TB pomija transformat� i kwantyzacj�
	 */

	Bool itsCurrentTUUsesTransformSkip;

	/**
	* @fn	Binarization::Binarization( );
	*
	* @brief	Konstruktor domy��ny.
	*/

	Binarization( );
public:

	/**
	 * @brief	Pr�g zmiany kodowania reszty wsp�czynnika na kod Golomba.
	 */

	static const Int COEFF_REMAIN_THRESHOLD;

	/**
	* @fn	static Binarization* Binarization::getInstance( );
	*
	* @brief	Dost�p do instancji kalkulatora
	*
	* @return Wska�nik na instancj�
	*/

	static Binarization* getInstance( );

	/**
	* @fn	Binarization::~Binarization( );
	*
	* @brief	Destruktor.
	*/

	~Binarization( );

	/**
	* @fn	UInt Binarization::countBinsInTB( Coeff** coefficients, UInt size, ScanningMode* scanningMode );
	*
	* @brief	Funkcja zlicza ilo�� bin�w potrzebnych do zakodowania przekazanego bloku wsp�czynnik�w transformacji
	*
	* @param [in]	coefficients	Macierz wsp�czynnik�w
	* @param	size								Rozmiar macierzy wsp�czynnik�w.
	* @param [in]	scanningMode	Wska�nik na strategi� skanowania macierzy.
	*
	* @return	Ilos� bin�w kt�ra zostanie podana na wej�cie kodera arytmetycznego w wyniku kodowania przekazanego bloku transformacji.
	*/

	UInt countBinsInTB( Coeff** coefficients, UInt size, ScanningMode* scanningMode );

	/**
	* @fn	UInt Binarization::countNonZeroCoeffsAndSigns( std::shared_ptr<CoeffGroup> cg );
	*
	* @brief	Zlicza wsp�czynnik�w niezerowych i ich znak�w w obr�bie CG
	*
	* @param	[out] cg	Wska�nik na CG. Jego wewn�trzne mapy wsp. znacz�cych i znak�w s� w funkcji modyfikowane.
	* @param isDCGroup Flaga m�wi, czy obacna CG jest lew� g�rn� w bloku transformacji.
	*
	* @return	Suma bin�w wsp�czynnik�w znacz�cych i ich znak�w.
	*/

	UInt countNonZeroCoeffsAndSigns( std::shared_ptr<CoeffGroup> cg, bool isDCGroup );

	/**
	* @fn	UInt Binarization::calcC1andC2( std::shared_ptr<CoeffGroup> cg, Coeff* out_remains, UInt numNonZero );
	*
	* @brief	Oblicza ilo�� bin�w grup C1 i C2 w obr�bie CG.
	*
	* @param	[out] cg							Wska�nik na CG. Jego wewn�trzne mapy C1 i C2 s� modyfikowane
	* @param [out]	out_remains	    Tablica reszt wsp�czynnik�w, kt�re b�dzie trzeba zakodowa� (modyfikowana w ciele metody)
	* @param	numNonZero					 	Ilo�� wsp�czynnik�w niezerowych w CG.
	*
	* @return	Sum� bin�w C1 i C2 w CG.
	*/

	UInt calcC1andC2( std::shared_ptr<CoeffGroup> cg, Coeff* out_remains, UInt numNonZero );

	/**
	* @fn	UInt Binarization::binarizeCoefficientRemainingLevel_NumBits( UInt symbol, UInt &rParam );
	*
	* @brief	Oblicza ilo�� bin�w potrzebn� do zakodowania reszty.
	*
	* @param	symbol						Reszta do zakodowania
	* @param [in]	rParam	Parametr Rice'a
	*
	* @return	Ilo�� bit�w potrzebn� do zakodowania reszty.
	*/

	UInt binarizeCoefficientRemainingLevel_NumBits( UInt symbol, UInt &rParam );

	/**
	* @fn	UInt Binarization::binarizeTR_NumBits( UInt val, UInt maxLenBeforeSuffix, UInt riceParam );
	*
	* @brief	Ilo�� bin�w, na kt�rej zosta�aby zakodowana podana warto�� w trybie TR
	*
	* @param	val									Warto�� do zakodowania.
	* @param	maxLenBeforeSuffix	Najwi�ksza d�ugos� kodu bez sufiksu.
	* @param	riceParam						Parametr Rice'a.
	*
	* @return	D�ugo�� kodu TR dla podanych parametr�w.
	*/

	UInt binarizeTR_NumBits( UInt val, UInt maxLenBeforeSuffix, UInt riceParam );

	/**
	* @fn	boost::dynamic_bitset<> Binarization::binarizeTR( UInt val, UInt maxLenBeforeSuffix, UInt riceParam );
	*
	* @brief	Zakodowanie warto�ci metod� TR
	*
	* @param	val									Warto�� do zakodowania.
	* @param	maxLenBeforeSuffix	Najwi�ksza d�ugos� kodu bez sufiksu.
	* @param	riceParam						Parametr Rice'a.
	*
	* @return	Bitset zer i jedynek reprezentacji TR podanej liczby.
	*/

	boost::dynamic_bitset<> binarizeTR( UInt val, UInt maxLenBeforeSuffix, UInt riceParam );

	/**
	* @fn	UInt Binarization::binarizeEGk_NumBits( Int val, UInt k );
	*
	* @brief	Ilo�� bin�w potrzebna do zakodowania podanej warto�ci eksponencjalnym kodem Golomba.
	*
	* @param	val Warto�� do zakodowania
	* @param	k	 	Rz�d kodu Golomba
	*
	* @return	D�ugo�� kodu Golomba dla podanej liczby.
	*/

	UInt binarizeEGk_NumBits( Int val, UInt k );

	/**
	* @fn	boost::dynamic_bitset<> Binarization::binarizeFL( UInt val, UInt len );
	*
	* @brief	Reprezentacja binowa podanej liczby w kodzie FL.
	*
	* @param	val	Warto�� do zakodowania
	* @param	len	D�ugo�� kodu FL.
	*
	* @return	Map� zer i jedynek b�d�c� reprezentacj� FL podanej liczby.
	*/

	boost::dynamic_bitset<> binarizeFL( UInt val, UInt len );

	/**
	* @fn	UInt Binarization::binarizeLastSignificantXY_NumBits( UInt XorY, UInt transformSize );
	*
	* @brief	Oblicza ilo�� bin�w potrzebn� do zakodowania wsp�rz�dnej ostatniego wsp�czynnika.
	*
	* @param	XorY				 	Wsp�rz�dna (X lub Y) do zakodowania
	* @param	transformSize	Rozmiar bloku transformacji
	*
	* @return	Ilo�� bin�w potrzebn� do zakodowania wsp�rz�dnej.
	*/

	UInt binarizeLastSignificantXY_NumBits( UInt XorY, UInt transformSIze );

	/**
	* @fn	Void Binarization::binarizeLastSignificantXY_GetPrefixAndSuffix( UInt position, UInt transformSize, UInt& out_prefix, UInt& out_suffix );
	*
	* @brief	Oblicza warto�ci prefiksu i sufiksu wsp�rz�dnej ostatniego niezerowago wsp�czynnika i zwraca je przez referencje.
	*
	* @param	position							Pozycja (X/Y) wsp�czynnika w bloku
	* @param	transformSize					Rozmiar bloku
	* @param [out]	out_prefix	Obliczony prefiks
	* @param [out]	out_suffix	Obliczony sufiks
	*
	*/

	Void binarizeLastSignificantXY_GetPrefixAndSuffix( UInt position, UInt transformSize, UInt& out_prefix, UInt& out_suffix );

	/**
	* @fn	boost::dynamic_bitset<> Binarization::binarizeLastSignificantXYPrefix( UInt prefix, UInt transformSize );
	*
	* @brief	Binaryzacja prefiksu wsp�rz�dnej ostatniego niezerowego wsp�czynnika w bloku.
	*
	* @param	prefix			 	Prefiks
	* @param	transformSize	Rozmiar bloku
	*
	* @return	Binarny kod prefiksu.
	*/

	boost::dynamic_bitset<> binarizeLastSignificantXYPrefix( UInt prefix, UInt transformSize );

	/**
	* @fn	boost::dynamic_bitset<> Binarization::binarizeLastSignificantXYSuffix( UInt suffix, UInt prefix );
	*
	* @brief	Binaryzacja sufiksu wsp�rz�dnej ostatniego niezerowego wsp�czynnika w bloku.
	*
	* @param	suffix	Sufiks.
	* @param	prefix	Prefiks wsp�rz�dnej.
	*
	* @return	Binarny kod sufiksu.
	*/

	boost::dynamic_bitset<> binarizeLastSignificantXYSuffix( UInt suffix, UInt prefix );

	/**
	* @fn	Bool Binarization::useSDH( );
	*
	* @brief	M�wi, czy u�ywany jest tryb ukrywania znaku SDH.
	*
	* @return	true, je�li SDH jest u�ywane; false w p.p.
	*/

	Bool useSDH( );

	/**
	* @fn	Bool Binarization::getCurrentCUUsesTransQuantBypass( ) const
	*
	* @brief	M�wi, czy aktualna CU u�ywa trybu TransQuantBypass.
	*
	* @return	true, je�li TransQuantBypass jest u�ywane; false w p.p.
	*/

	Bool getCurrentCUUsesTransQuantBypass( ) const
	{
		return itsCurrentCUUsesTransQuantBypass;
	}

	/**
	* @fn	void Binarization::setCurrentCUUsesTransQuantBypass( Bool val )
	*
	* @brief	Ustawia flag� m�wi�c� o u�ywniu SDH (nale�y to zrobi� przed binaryzacj�).
	*
	* @param	val	Flaga w��czenia trybu TransQuantBypass.
	*/

	void setCurrentCUUsesTransQuantBypass( Bool val )
	{
		itsCurrentCUUsesTransQuantBypass = val;
	}

	/**
	* @fn	Bool Binarization::getCurrentTUUsesTransformSkip( ) const
	*
	* @brief	M�wi, czy aktualna TU u�ywa trybu pomijania transformacji.
	*
	* @return	true, je�li TransformSkip jest u�ywane; false w p.p.
	*/

	Bool getCurrentTUUsesTransformSkip( ) const
	{
		return itsCurrentTUUsesTransformSkip;
	}

	/**
	* @fn	void Binarization::setCurrentTUUsesTransformSkip( Bool val )
	*
	* @brief	Ustawia flag� m�wi�c� o u�ywniu trybu TransformSkip (nale�y to zrobi� przed binaryzacj�).
	*
	* @param	val	Flaga w��czenia trybu TransformSkip.
	*/

	void setCurrentTUUsesTransformSkip( Bool val )
	{
		itsCurrentTUUsesTransformSkip = val;
	}
};

#endif