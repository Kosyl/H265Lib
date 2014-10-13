/** \file     SAOInfo.h
\brief    Struktura informacji o filtracji SAO
*/
#ifndef SAOInfo_h__
#define SAOInfo_h__

#include "Utils.h"
#include <bitset>

/**
 * @class	SAOInfo
 *
 * @brief	Struktura inforamcji o filtracji SAO.
 */

class SAOInfo
{
	/**
	 * @brief	Typ SAO.
	 */

	SAOMode itsSAOType;

	/**
	 * @brief	Offsety SAO (4)
	 */

	std::vector<Int> itsSAOOffsets;

	/**
	 * @brief	BandPosition dla trybu 1.
	 */

	Int myBandPosition;

	/**
	 * @brief	Klasa EO dla typu 2.
	 */

	Int myEOClass;
};

#endif // SAOInfo_h__