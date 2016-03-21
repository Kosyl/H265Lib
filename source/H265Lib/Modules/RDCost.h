///** \file     RDCost.h
//    \brief    Klasa kwantyzatora (header)
//*/
//#ifndef _DRCOST_H_
//#define _RDCOST_H_
//
//#include "Utils.h"
//
///**
// * @class	RDCost
// *
// * @brief	Klasa estymatora kosztu.
// */
//
//class RDCost
//{
//private:
//
//	/**
//	 * @brief		Stabelaryzowane warto�ci parametru lambda(QP)
//	 */
//
//	const static Double lambda[53];
//
//	/**
//	 * @brief	Wybrana funkcja licz�ca zniekszta�cenie mi�dzy obrazami.
//	 */
//
//	Long( *itsDistortionFunction )( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//
//	/**
//	 * @brief	Instancja kalkulatora kosztu.
//	 */
//
//	static RDCost* itsInstance;
//
//	/**
//	 * @fn	RDCost::RDCost( );
//	 *
//	 * @brief	Konstruktor domy�lny.
//	 */
//
//	RDCost( );
//
//public:
//
//	/**
//	 * @fn	static RDCost* RDCost::getInstance( );
//	 *
//	 * @brief	Zwraca instancj� kalkulatora kosztu.
//	 *
//	 * @return	Wska�nik na instancj� kalkulatora kosztu.
//	 */
//
//	static RDCost* getInstance( );
//
//	/**
//	 * @fn	RDCost::~RDCost( );
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	~RDCost( );
//
//	/**
//	 * @fn	static Long RDCost::SSD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//	 *
//	 * @brief	Funkcja licz�ca zniekszta�cenie SSD.
//	 *
//	 * @param	picOrig 	Macierz obrazu oryginalnego.
//	 * @param	picRecon	Macierz obrazu zniekszta�conego.
//	 *
//	 * @return	Warto�� zniekszta�cenia.
//	 */
//
//	static Long SSD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//
//	/**
//	 * @fn	static Long RDCost::SAD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//	 *
//	 * @brief	Funkcja licz�ca zniekszta�cenie SSD.
//	 *
//	 * @param	picOrig 	Macierz obrazu oryginalnego.
//	 * @param	picRecon	Macierz obrazu zniekszta�conego.
//	 *
//	 * @return	Warto�� zniekszta�cenia.
//	 */
//
//	static Long SAD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//
//	/**
//	 * @fn	void RDCost::setDistortionFunction( Long( *newFunc )( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon ) );
//	 *
//	 * @brief	Wyb�r funkcji licz�cej zniekszta�cenie.
//	 *
//	 * @param [in,out]	newFunc	Wska�nik na funkcj� licz�c� zniekszta�cenie.
//	 *
//	 * @return	void.
//	 */
//
//	void setDistortionFunction( Long( *newFunc )( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon ) );
//
//	/**
//	 * @fn	Double RDCost::calcCost( int estBits, int QP, Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//	 *
//	 * @brief Obliczenie kosztu.
//	 *
//	 * @param	estBits 	Estymowana liczba bit�w.
//	 * @param	QP				Wartos� parametru QP.
//	 * @param	picOrig 	Pr�bki oryginalne.
//	 * @param	picRecon	Pr�bki zrekonstruowane.
//	 *
//	 * @return	Obliczony koszt.
//	 */
//
//	Double calcCost( int estBits, int QP, Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//
//	/**
//	 * @fn	Double RDCost::calcCost( int estBits, int distortion, int QP );
//	 *
//	 * @brief	Obliczenie kosztu.
//	 *
//	 * @param	estBits			Estymowana liczba bit�w.
//	 * @param	distortion	Zniekszta�cenie.
//	 * @param	QP					Wartos� parametru QP.
//	 *
//	 * @return	Obliczony koszt.
//	 */
//
//	Double calcCost( int estBits, int distortion, int QP );
//
//	/**
//	 * @fn	int RDCost::calcDistortion( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//	 *
//	 * @brief	Oblicza zniekszta�cenie wybrana funkcj�.
//	 *
//	 * @param	picOrig 	Macierz obrazu oryginalnego.
//	 * @param	picRecon	Macierz obrazu zniekszta�conego.
//	 *
//	 * @return	Obliczone zniekszta�cenie.
//	 */
//
//	int calcDistortion( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
//};
//
//#endif