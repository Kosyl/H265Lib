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
* @brief	Klasa bazowa trybów skanowania
*/

class ScanningMode
{
public:
	static const Int CG_SIZE;

	/**
	* @fn	ScanningMode::ScanningMode( );
	*
	* @brief	Konstruktor domyœlny
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
	* @param [in]	coeffs					Macierz wspó³czynników
	* @param	size									Rozmiar macierzy wspó³czynników
	* @param [out]	out_lastCoeffX	Wspó³rzêdna X ostatniego niezerowego wspó³czynnika
	* @param [out]	out_lastCoeffY	Wspó³rzêdna Y ostatniego niezerowego wspó³czynnika
	*
	* @return	Wektor wskaŸników na obiekty CoeffGroup reprezentuj¹ce podbloki 4x4
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
	* @brief	Konstruktor domyœlny
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
	* @param [in]	coeffs					Macierz wspó³czynników
	* @param	size									Rozmiar macierzy wspó³czynników
	* @param [out]	out_lastCoeffX	Wspó³rzêdna X ostatniego niezerowego wspó³czynnika
	* @param [out]	out_lastCoeffY	Wspó³rzêdna Y ostatniego niezerowego wspó³czynnika
	*
	* @return	Wektor wskaŸników na obiekty CoeffGroup reprezentuj¹ce podbloki 4x4
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
	* @brief	Konstruktor domyœlny
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
	* @param [in]	coeffs					Macierz wspó³czynników
	* @param	size									Rozmiar macierzy wspó³czynników
	* @param [out]	out_lastCoeffX	Wspó³rzêdna X ostatniego niezerowego wspó³czynnika
	* @param [out]	out_lastCoeffY	Wspó³rzêdna Y ostatniego niezerowego wspó³czynnika
	*
	* @return	Wektor wskaŸników na obiekty CoeffGroup reprezentuj¹ce podbloki 4x4
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
	* @brief	Konstruktor domyœlny
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
	* @param [in]	coeffs					Macierz wspó³czynników
	* @param	size									Rozmiar macierzy wspó³czynników
	* @param [out]	out_lastCoeffX	Wspó³rzêdna X ostatniego niezerowego wspó³czynnika
	* @param [out]	out_lastCoeffY	Wspó³rzêdna Y ostatniego niezerowego wspó³czynnika
	*
	* @return	Wektor wskaŸników na obiekty CoeffGroup reprezentuj¹ce podbloki 4x4
	*/

	std::vector<std::shared_ptr<CoeffGroup> > scan( Coeff** coeffs, UInt size, UInt& out_lastCoeffX, UInt& out_lastCoeffY );

};

/**
* @class	Binarization
*
* @brief	Kalkulator binaryzacji elementów i zliczania binów
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
	 * @brief	Flaga mówi¹ca, czy obliczenia dla TB maj¹ zak³adaæ mo¿liwoœæ u¿ycia trybu TransQuantBypass
	 */

	Bool itsCurrentCUUsesTransQuantBypass;

	/**
	 * @brief	Flaga mówi¹ca, czy obliczany TB pomija transformatê i kwantyzacjê
	 */

	Bool itsCurrentTUUsesTransformSkip;

	/**
	* @fn	Binarization::Binarization( );
	*
	* @brief	Konstruktor domyœ³ny.
	*/

	Binarization( );
public:

	/**
	 * @brief	Próg zmiany kodowania reszty wspó³czynnika na kod Golomba.
	 */

	static const Int COEFF_REMAIN_THRESHOLD;

	/**
	* @fn	static Binarization* Binarization::getInstance( );
	*
	* @brief	Dostêp do instancji kalkulatora
	*
	* @return WskaŸnik na instancjê
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
	* @brief	Funkcja zlicza iloœæ binów potrzebnych do zakodowania przekazanego bloku wspó³czynników transformacji
	*
	* @param [in]	coefficients	Macierz wspó³czynników
	* @param	size								Rozmiar macierzy wspó³czynników.
	* @param [in]	scanningMode	WskaŸnik na strategiê skanowania macierzy.
	*
	* @return	Ilosæ binów która zostanie podana na wejœcie kodera arytmetycznego w wyniku kodowania przekazanego bloku transformacji.
	*/

	UInt countBinsInTB( Coeff** coefficients, UInt size, ScanningMode* scanningMode );

	/**
	* @fn	UInt Binarization::countNonZeroCoeffsAndSigns( std::shared_ptr<CoeffGroup> cg );
	*
	* @brief	Zlicza wspó³czynników niezerowych i ich znaków w obrêbie CG
	*
	* @param	[out] cg	WskaŸnik na CG. Jego wewnêtrzne mapy wsp. znacz¹cych i znaków s¹ w funkcji modyfikowane.
	* @param isDCGroup Flaga mówi, czy obacna CG jest lew¹ górn¹ w bloku transformacji.
	*
	* @return	Suma binów wspó³czynników znacz¹cych i ich znaków.
	*/

	UInt countNonZeroCoeffsAndSigns( std::shared_ptr<CoeffGroup> cg, bool isDCGroup );

	/**
	* @fn	UInt Binarization::calcC1andC2( std::shared_ptr<CoeffGroup> cg, Coeff* out_remains, UInt numNonZero );
	*
	* @brief	Oblicza iloœæ binów grup C1 i C2 w obrêbie CG.
	*
	* @param	[out] cg							WskaŸnik na CG. Jego wewnêtrzne mapy C1 i C2 s¹ modyfikowane
	* @param [out]	out_remains	    Tablica reszt wspó³czynników, które bêdzie trzeba zakodowaæ (modyfikowana w ciele metody)
	* @param	numNonZero					 	Iloœæ wspó³czynników niezerowych w CG.
	*
	* @return	Sumê binów C1 i C2 w CG.
	*/

	UInt calcC1andC2( std::shared_ptr<CoeffGroup> cg, Coeff* out_remains, UInt numNonZero );

	/**
	* @fn	UInt Binarization::binarizeCoefficientRemainingLevel_NumBits( UInt symbol, UInt &rParam );
	*
	* @brief	Oblicza iloœæ binów potrzebn¹ do zakodowania reszty.
	*
	* @param	symbol						Reszta do zakodowania
	* @param [in]	rParam	Parametr Rice'a
	*
	* @return	Iloœæ bitów potrzebn¹ do zakodowania reszty.
	*/

	UInt binarizeCoefficientRemainingLevel_NumBits( UInt symbol, UInt &rParam );

	/**
	* @fn	UInt Binarization::binarizeTR_NumBits( UInt val, UInt maxLenBeforeSuffix, UInt riceParam );
	*
	* @brief	Iloœæ binów, na której zosta³aby zakodowana podana wartoœæ w trybie TR
	*
	* @param	val									Wartoœæ do zakodowania.
	* @param	maxLenBeforeSuffix	Najwiêksza d³ugosæ kodu bez sufiksu.
	* @param	riceParam						Parametr Rice'a.
	*
	* @return	D³ugoœæ kodu TR dla podanych parametrów.
	*/

	UInt binarizeTR_NumBits( UInt val, UInt maxLenBeforeSuffix, UInt riceParam );

	/**
	* @fn	boost::dynamic_bitset<> Binarization::binarizeTR( UInt val, UInt maxLenBeforeSuffix, UInt riceParam );
	*
	* @brief	Zakodowanie wartoœci metod¹ TR
	*
	* @param	val									Wartoœæ do zakodowania.
	* @param	maxLenBeforeSuffix	Najwiêksza d³ugosæ kodu bez sufiksu.
	* @param	riceParam						Parametr Rice'a.
	*
	* @return	Bitset zer i jedynek reprezentacji TR podanej liczby.
	*/

	boost::dynamic_bitset<> binarizeTR( UInt val, UInt maxLenBeforeSuffix, UInt riceParam );

	/**
	* @fn	UInt Binarization::binarizeEGk_NumBits( Int val, UInt k );
	*
	* @brief	Iloœæ binów potrzebna do zakodowania podanej wartoœci eksponencjalnym kodem Golomba.
	*
	* @param	val Wartoœæ do zakodowania
	* @param	k	 	Rz¹d kodu Golomba
	*
	* @return	D³ugoœæ kodu Golomba dla podanej liczby.
	*/

	UInt binarizeEGk_NumBits( Int val, UInt k );

	/**
	* @fn	boost::dynamic_bitset<> Binarization::binarizeFL( UInt val, UInt len );
	*
	* @brief	Reprezentacja binowa podanej liczby w kodzie FL.
	*
	* @param	val	Wartoœæ do zakodowania
	* @param	len	D³ugoœæ kodu FL.
	*
	* @return	Mapê zer i jedynek bêd¹c¹ reprezentacj¹ FL podanej liczby.
	*/

	boost::dynamic_bitset<> binarizeFL( UInt val, UInt len );

	/**
	* @fn	UInt Binarization::binarizeLastSignificantXY_NumBits( UInt XorY, UInt transformSize );
	*
	* @brief	Oblicza iloœæ binów potrzebn¹ do zakodowania wspó³rzêdnej ostatniego wspó³czynnika.
	*
	* @param	XorY				 	Wspó³rzêdna (X lub Y) do zakodowania
	* @param	transformSize	Rozmiar bloku transformacji
	*
	* @return	Iloœæ binów potrzebn¹ do zakodowania wspó³rzêdnej.
	*/

	UInt binarizeLastSignificantXY_NumBits( UInt XorY, UInt transformSIze );

	/**
	* @fn	Void Binarization::binarizeLastSignificantXY_GetPrefixAndSuffix( UInt position, UInt transformSize, UInt& out_prefix, UInt& out_suffix );
	*
	* @brief	Oblicza wartoœci prefiksu i sufiksu wspó³rzêdnej ostatniego niezerowago wspó³czynnika i zwraca je przez referencje.
	*
	* @param	position							Pozycja (X/Y) wspó³czynnika w bloku
	* @param	transformSize					Rozmiar bloku
	* @param [out]	out_prefix	Obliczony prefiks
	* @param [out]	out_suffix	Obliczony sufiks
	*
	*/

	Void binarizeLastSignificantXY_GetPrefixAndSuffix( UInt position, UInt transformSize, UInt& out_prefix, UInt& out_suffix );

	/**
	* @fn	boost::dynamic_bitset<> Binarization::binarizeLastSignificantXYPrefix( UInt prefix, UInt transformSize );
	*
	* @brief	Binaryzacja prefiksu wspó³rzêdnej ostatniego niezerowego wspó³czynnika w bloku.
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
	* @brief	Binaryzacja sufiksu wspó³rzêdnej ostatniego niezerowego wspó³czynnika w bloku.
	*
	* @param	suffix	Sufiks.
	* @param	prefix	Prefiks wspó³rzêdnej.
	*
	* @return	Binarny kod sufiksu.
	*/

	boost::dynamic_bitset<> binarizeLastSignificantXYSuffix( UInt suffix, UInt prefix );

	/**
	* @fn	Bool Binarization::useSDH( );
	*
	* @brief	Mówi, czy u¿ywany jest tryb ukrywania znaku SDH.
	*
	* @return	true, jeœli SDH jest u¿ywane; false w p.p.
	*/

	Bool useSDH( );

	/**
	* @fn	Bool Binarization::getCurrentCUUsesTransQuantBypass( ) const
	*
	* @brief	Mówi, czy aktualna CU u¿ywa trybu TransQuantBypass.
	*
	* @return	true, jeœli TransQuantBypass jest u¿ywane; false w p.p.
	*/

	Bool getCurrentCUUsesTransQuantBypass( ) const
	{
		return itsCurrentCUUsesTransQuantBypass;
	}

	/**
	* @fn	void Binarization::setCurrentCUUsesTransQuantBypass( Bool val )
	*
	* @brief	Ustawia flagê mówi¹c¹ o u¿ywniu SDH (nale¿y to zrobiæ przed binaryzacj¹).
	*
	* @param	val	Flaga w³¹czenia trybu TransQuantBypass.
	*/

	void setCurrentCUUsesTransQuantBypass( Bool val )
	{
		itsCurrentCUUsesTransQuantBypass = val;
	}

	/**
	* @fn	Bool Binarization::getCurrentTUUsesTransformSkip( ) const
	*
	* @brief	Mówi, czy aktualna TU u¿ywa trybu pomijania transformacji.
	*
	* @return	true, jeœli TransformSkip jest u¿ywane; false w p.p.
	*/

	Bool getCurrentTUUsesTransformSkip( ) const
	{
		return itsCurrentTUUsesTransformSkip;
	}

	/**
	* @fn	void Binarization::setCurrentTUUsesTransformSkip( Bool val )
	*
	* @brief	Ustawia flagê mówi¹c¹ o u¿ywniu trybu TransformSkip (nale¿y to zrobiæ przed binaryzacj¹).
	*
	* @param	val	Flaga w³¹czenia trybu TransformSkip.
	*/

	void setCurrentTUUsesTransformSkip( Bool val )
	{
		itsCurrentTUUsesTransformSkip = val;
	}
};

#endif