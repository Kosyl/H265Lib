/**
 * @file	Transform.h
 *
 * @brief	Deklaracja klasy kalkulatora transformacji.
 */

#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "TypeDef.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string.h>

/**
 * @class	Transform
 *
 * @brief	Kalkulator transformacji.
 */

class Transform
{
public:

	/**
	 * @fn	static Transform* Transform::getInstance();
	 *
	 * @brief	Dost�p do instancji kalkulatora transformacji.
	 */

	static Transform* getInstance( );

	/**
	 * @fn	Transform::~Transform();
	 *
	 * @brief	Destruktor.
	 */

	~Transform( );

	/**
	 * @fn	void Transform::forwardTrans(Sample** const block, Coeff** coeff, const Int& trSize, const Int& bitDepth, const Bool& isIntra4x4LumaDST_hor, const Bool& isIntra4x4LumaDST_ver);
	 *
	 * @brief	Transformacja prosta.
	 *
	 * @param [in]	block			 	Blok wej�ciowy pr�bek.
	 * @param [out]	coeff			 	Wyj�ciowy blok wsp�czynnik�w.
	 * @param	trSize							 	Rozmiar bloku.
	 * @param	bitDepth						 	G��bia bitowa pr�bek.
	 * @param	isIntra4x4LumaDST_hor	U�ycie trybu DST w kierunku poziomym.
	 * @param	isIntra4x4LumaDST_ver	U�ycie trybu DST w kierunku pionowym.
	 */

	void forwardTrans( Sample** const block, Coeff** coeff, const Int& trSize, const Int& bitDepth, const Bool& isIntra4x4LumaDST_hor, const Bool& isIntra4x4LumaDST_ver );

	/**
	 * @fn	void Transform::inverseTrans( Coeff** const coeff, Sample** block, const Int& trSize, const Int& bitDepth, const Bool& isIntra4x4LumaDST_hor, const Bool& isIntra4x4LumaDST_ver );
	 *
	 * @brief	Transformacja odwrotna.
	 *
	 * @param [in,out]	coeff			 	Wej�ciowy blok wsp�czynnik�w.
	 * @param [in,out]	block			 	wyj�ciowy blok pr�bek.
	 * @param	trSize							 	Rozmiar bloku.
	 * @param	bitDepth						 	G��bia bitowa pr�bek.
	 * @param	isIntra4x4LumaDST_hor	U�ycie trybu DST w kierunku poziomym.
	 * @param	isIntra4x4LumaDST_ver	U�ycie trybu DST w kierunku pionowym.
	 */

	void inverseTrans( Coeff** const coeff, Sample** block, const Int& trSize, const Int& bitDepth, const Bool& isIntra4x4LumaDST_hor, const Bool& isIntra4x4LumaDST_ver );

	/**
	 * @brief	Tablica tymczasowa przechowuj�ca wynik transformaty poziomej do wgl�du.
	 */

	Int** afterHor;

	/**
	 * @brief	Obiekt loguj�cy przebieg transformacji.
	 */

	Logger log;

	/**
	 * @fn	void Transform::initLog(string path, bool isLogging);
	 *
	 * @brief	Inicjalizuje logowanie.
	 *
	 * @param	path		 	�cie�ka pliku logu.
	 * @param	isLogging	W��czenie/wy��czenie logowania.
	 */

	void initLog( std::string path, bool isLogging );

private:

	/**
	 * @brief	Instancja kalkulatora transformacji.
	 */

	static Transform* instance;

	/**
	 * @fn	Transform::Transform();
	 *
	 * @brief	Konstruktor domy�lny.
	 */

	Transform( );

	/**
	 * @brief	Tablica wsp�czynnik�w DCT 4x4.
	 */

	static const Short TCOEFF_4[ 4 ][ 4 ];

	/**
	 * @brief	Tablica wsp�czynnik�w DCT 8x8.
	 */

	static const Short TCOEFF_8[ 8 ][ 8 ];

	/**
	 * @brief	Tablica wsp�czynnik�w DCT 16x16.
	 */

	static const Short TCOEFF_16[ 16 ][ 16 ];

	/**
	 * @brief	Tablica wsp�czynnik�w DCT 32x32.
	 */

	static const Short TCOEFF_32[ 32 ][ 32 ];

	/**
	 * @brief	Tablica wsp�czynnik�w DST 4x4.
	 */

	static const Short TCOEFF_DST_4[ 4 ][ 4 ];

	/**
	 * @brief	Przesuni�cie po poziomej transformacie odwrotnej.
	 */

	static const Short SHIFT_INV_1;

	/**
	 * @brief	Przesuni�cie po pionowej transformacie odwrotnej.
	 */

	static const Short SHIFT_INV_2;
};

#endif