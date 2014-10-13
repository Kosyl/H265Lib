/** \file     Quant.h
    \brief    Klasa kwantyzatora (header)
*/
#ifndef _QUANT_
#define _QUANT_

#include "TypeDef.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string.h>

/**
 * @class	Quant
 *
 * @brief	Kalkulator kwantyzacji.
 */

class Quant
{
public:

	/**
	 * @fn	static Quant Quant::*getInstance();
	 *
	 * @brief	Dostêp do instancji kalkulatora kwantyzacji.
	 *
	 * @return	WskaŸnik na singleton.
	 */

	static Quant *getInstance();

	/**
	 * @fn	Quant::~Quant();
	 *
	 * @brief	Destruktor.
	 */

	~Quant();

	/**
	 * @fn	void Quant::quant( Coeff** const coeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize );
	 *
	 * @brief	Proces kwantyzacji.
	 *
	 * @param [in]	coeff			Macierz wspó³czynników wejœciowych.
	 * @param [out]	result	 	Macierz wynikowa.
	 * @param	QP						 	Parametr QP.
	 * @param	bitDepth			 	G³êbia bitowa próbek.
	 * @param	totalSize			 	Ca³kowity rozmiar macierzy wejœciowej.
	 * @param	trSize				 	Rozmiar bloku transformacji.
	 */

	void quant( Coeff** const coeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize );

	/**
	 * @fn	void Quant::deQuant( Coeff** const input, Coeff** block, const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize );
	 *
	 * @brief	Proces dekwantyzacji.
	 *
	 * @param [in]	input			Macierz wspó³czynników wejœciowych.
	 * @param [out]	block	 	Macierz wynikowa.
	 * @param	QP						 	Parametr QP.
	 * @param	bitDepth			 	G³êbia bitowa próbek.
	 * @param	totalSize			 	Ca³kowity rozmiar macierzy wejœciowej.
	 * @param	trSize				 	Rozmiar bloku transformacji.
	 */

	void deQuant( Coeff** const input, Coeff** block, const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize );

	/**
	 * @fn	void Quant::quant( Coeff** const coeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& trSize );
	 *
	 * @brief	Quants.
	 *
	 * @param [in]	coeff			Macierz wspó³czynników wejœciowych.
	 * @param [out]	result	 	Macierz wynikowa.
	 * @param	QP						 	Parametr QP.
	 * @param	bitDepth			 	G³êbia bitowa próbek.
	 * @param	trSize				 	Rozmiar bloku transformacji.
	 */

	void quant( Coeff** const coeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& trSize );

	/**
	 * @fn	void Quant::deQuant( Coeff** const input, Coeff** block, const Int& QP, const Int& bitDepth, const Int& trSize );
	 *
	 * @brief	De quant.
	 *
	 * @param [in]	input			Macierz wspó³czynników wejœciowych.
	 * @param [out]	block	 	Macierz wynikowa.
	 * @param	QP						 	Parametr QP.
	 * @param	bitDepth			 	G³êbia bitowa próbek.
	 * @param	trSize				 	Rozmiar bloku transformacji.
	 */

	void deQuant( Coeff** const input, Coeff** block, const Int& QP, const Int& bitDepth, const Int& trSize );

	/**
	 * @fn	void Quant::initLog(string path, bool isLogging);
	 *
	 * @brief	Inicjalizuje logowanie
	 *
	 * @param	path		 	Œcie¿ka logowania.
	 * @param	isLogging	W³¹czenie logowania.
	 */

	void initLog( std::string path, bool isLogging );

	/**
	 * @brief	Obiekt loguj¹cy.
	 */

	Logger log;
private:

	/**
	 * @fn	Quant::Quant();
	 *
	 * @brief	Konstruktor domyœlny.
	 */

	Quant();

	/**
	 * @brief	Instancja.
	 */

	static Quant *instance;

	/**
	 * @brief	Tablica do wyliczania Q w kwantyzacji.
	 */

	static const Short forfardQuantCoeffs[ 6 ];

	/**
	 * @brief	Tablica do wyliczania IQ w dekwantyzacji.
	 */

	static const Short inverseQuantCoeffs[ 6 ];
};

#endif