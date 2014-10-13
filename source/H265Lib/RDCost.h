/** \file     RDCost.h
    \brief    Klasa kwantyzatora (header)
*/
#ifndef _DRCOST_H_
#define _RDCOST_H_

#include "Utils.h"

/**
 * @class	RDCost
 *
 * @brief	Klasa estymatora kosztu.
 */

class RDCost
{
private:

	/**
	 * @brief		Stabelaryzowane wartoœci parametru lambda(QP)
	 */

	const static Double lambda[53];

	/**
	 * @brief	Wybrana funkcja licz¹ca zniekszta³cenie miêdzy obrazami.
	 */

	Long( *itsDistortionFunction )( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );

	/**
	 * @brief	Instancja kalkulatora kosztu.
	 */

	static RDCost* itsInstance;

	/**
	 * @fn	RDCost::RDCost( );
	 *
	 * @brief	Konstruktor domyœlny.
	 */

	RDCost( );

public:

	/**
	 * @fn	static RDCost* RDCost::getInstance( );
	 *
	 * @brief	Zwraca instancjê kalkulatora kosztu.
	 *
	 * @return	WskaŸnik na instancjê kalkulatora kosztu.
	 */

	static RDCost* getInstance( );

	/**
	 * @fn	RDCost::~RDCost( );
	 *
	 * @brief	Destruktor.
	 */

	~RDCost( );

	/**
	 * @fn	static Long RDCost::SSD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
	 *
	 * @brief	Funkcja licz¹ca zniekszta³cenie SSD.
	 *
	 * @param	picOrig 	Macierz obrazu oryginalnego.
	 * @param	picRecon	Macierz obrazu zniekszta³conego.
	 *
	 * @return	Wartoœæ zniekszta³cenia.
	 */

	static Long SSD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );

	/**
	 * @fn	static Long RDCost::SAD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
	 *
	 * @brief	Funkcja licz¹ca zniekszta³cenie SSD.
	 *
	 * @param	picOrig 	Macierz obrazu oryginalnego.
	 * @param	picRecon	Macierz obrazu zniekszta³conego.
	 *
	 * @return	Wartoœæ zniekszta³cenia.
	 */

	static Long SAD( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );

	/**
	 * @fn	Void RDCost::setDistortionFunction( Long( *newFunc )( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon ) );
	 *
	 * @brief	Wybór funkcji licz¹cej zniekszta³cenie.
	 *
	 * @param [in,out]	newFunc	WskaŸnik na funkcjê licz¹c¹ zniekszta³cenie.
	 *
	 * @return	Void.
	 */

	Void setDistortionFunction( Long( *newFunc )( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon ) );

	/**
	 * @fn	Double RDCost::calcCost( UInt estBits, UInt QP, Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
	 *
	 * @brief Obliczenie kosztu.
	 *
	 * @param	estBits 	Estymowana liczba bitów.
	 * @param	QP				Wartosæ parametru QP.
	 * @param	picOrig 	Próbki oryginalne.
	 * @param	picRecon	Próbki zrekonstruowane.
	 *
	 * @return	Obliczony koszt.
	 */

	Double calcCost( UInt estBits, UInt QP, Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );

	/**
	 * @fn	Double RDCost::calcCost( UInt estBits, UInt distortion, UInt QP );
	 *
	 * @brief	Obliczenie kosztu.
	 *
	 * @param	estBits			Estymowana liczba bitów.
	 * @param	distortion	Zniekszta³cenie.
	 * @param	QP					Wartosæ parametru QP.
	 *
	 * @return	Obliczony koszt.
	 */

	Double calcCost( UInt estBits, UInt distortion, UInt QP );

	/**
	 * @fn	UInt RDCost::calcDistortion( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
	 *
	 * @brief	Oblicza zniekszta³cenie wybrana funkcj¹.
	 *
	 * @param	picOrig 	Macierz obrazu oryginalnego.
	 * @param	picRecon	Macierz obrazu zniekszta³conego.
	 *
	 * @return	Obliczone zniekszta³cenie.
	 */

	UInt calcDistortion( Submatrix<Sample> picOrig, Submatrix<Sample> picRecon );
};

#endif