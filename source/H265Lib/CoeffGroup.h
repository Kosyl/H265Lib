/**
 * @file	CoeffGroup.h
 *
 * @brief	Deklaracja klasy grupy wspó³czynników.
 */

#ifndef _COEFFGROUP_H_
#define _COEFFGROUP_H_

#include "Utils.h"
#include <boost/dynamic_bitset.hpp>

/**
 * @class	CoeffGroup
 *
 * @brief	Reprezentacja grupy wspó³czynników 4x4 w obrêbie bloku transformacji.
 */

class CoeffGroup
{
private:

	/**
	 * @brief	Uporz¹dkowane wspó³czynniki.
	 */

	Coeff itsScan[ 16 ];

	/**
	 * @brief	Indeks pierwszego niezerowego wspó³czynnika w kolejnoœci skanowania.
	 */

	Short itsFirstPosInScan;

	/**
	 * @brief	Indeks ostatniego niezerowego wspó³czynnika w kolejnoœci skanowania.
	 */

	Short itsLastPosInScan;

	/**
	 * @brief	Mapa flag wspó³czynników znacz¹cych ( 1 gdy wspó³czynnik > 0, 0 w p.p.).
	 */

	boost::dynamic_bitset<> itsSignificantCoeffFlags;

	/**
	 * @brief	Mapa flag wspó³czynników C1 (1 gdy wspó³czynnik > 1, 0 gdy wspó³czynnik == 1).
	 */

	boost::dynamic_bitset<> itsC1Group;

	/**
	 * @brief	Bit mówi¹cy, czy pierwszy wspó³æzynnik wiêkszy od 1 jest wiêkszy tak¿e od 2.
	 */

	Bool itsC2;

	/**
	 * @brief	Mapa znaków ( + -> 0, - -> 1 )
	 */

	boost::dynamic_bitset<> itsCoeffSigns;

public:

	/**
	 * @fn	CoeffGroup::CoeffGroup( );
	 *
	 * @brief	Konstruktor domyœlny.
	 */

	CoeffGroup( );

	/**
	 * @fn	CoeffGroup::~CoeffGroup( );
	 *
	 * @brief	Destruktor.
	 */

	~CoeffGroup( );

	/**
	 * @fn	Coeff& CoeffGroup::operator[]( const int index )
	 *
	 * @brief	Operator indeksowania, zwraca odpowiedni wspó³czynnik w kolejnoœci skanowania.
	 *
	 * @param	index	Indeks wspó³czynnika w kolejnoœci skanowania.
	 *
	 * @return Wspó³czynnik na danej pozycji.
	 */

	Coeff& operator[]( const int index )
	{
		assert( index >= 0 && index <= 15 );
		return itsScan[ index ];
	}

	/**
	 * @fn	Short CoeffGroup::getLastPosInScan( ) const
	 *
	 * @brief	Zwraca indeks ostatniego niezerowegeo wspó³czynnika w grupie.
	 *
	 * @return	Indeks ostatniego niezerowegeo wspó³czynnika w grupie.
	 */

	Short getLastPosInScan( ) const
	{
		return itsLastPosInScan;
	}

	/**
	 * @fn	void CoeffGroup::setLastPosInScan( Short val )
	 *
	 * @brief	Ustawia indeks ostatniego niezerowegeo wspó³czynnika w grupie.
	 *
	 * @param	val	Indeks ostatniego niezerowegeo wspó³czynnika w grupie.
	 */

	void setLastPosInScan( Short val )
	{
		itsLastPosInScan = val;
	}

	/**
	 * @fn	Short CoeffGroup::getFirstPosInScan( ) const
	 *
	 * @brief Zwraca indeks pierwszego niezerowegeo wspó³czynnika w grupie.
	 *
	 * @return	Indeks pierwszego niezerowegeo wspó³czynnika w grupie.
	 */

	Short getFirstPosInScan( ) const
	{
		return itsFirstPosInScan;
	}

	/**
	 * @fn	void CoeffGroup::setFirstPosInScan( Short val )
	 *
	 * @brief	Ustawia indeks pierwszego niezerowegeo wspó³czynnika w grupie.
	 *
	 * @param	val	Indeks pierwszego niezerowegeo wspó³czynnika w grupie.
	 */

	void setFirstPosInScan( Short val )
	{
		itsFirstPosInScan = val;
	}	

	/**
	 * @fn	boost::dynamic_bitset<> CoeffGroup::getCoeffSigns( ) const
	 *
	 * @brief	Zwraca mapê znaków.
	 *
	 * @return	Mapê znaków.
	 */

	boost::dynamic_bitset<> getCoeffSigns( ) const
	{
		return itsCoeffSigns;
	}

	/**
	 * @fn	void CoeffGroup::setCoeffSigns( boost::dynamic_bitset<> val )
	 *
	 * @brief Ustawia mapê znaków.
	 *
	 * @param	val	Nowa mapa znaków.
	 */

	void setCoeffSigns( boost::dynamic_bitset<> val )
	{
		itsCoeffSigns = val;
	}

	/**
	 * @fn	boost::dynamic_bitset<> CoeffGroup::getC1Group( ) const
	 *
	 * @brief	Zwraca mapê C1.
	 *
	 * @return Mapê C1.
	 */

	boost::dynamic_bitset<> getC1Group( ) const
	{
		return itsC1Group;
	}

	/**
	 * @fn	void CoeffGroup::setC1Group( boost::dynamic_bitset<> val )
	 *
	 * @brief	Ustawia mapê C1.
	 *
	 * @param	val	Nowa mapa C1.
	 */

	void setC1Group( boost::dynamic_bitset<> val )
	{
		itsC1Group = val;
	}

	/**
	 * @fn	boost::dynamic_bitset<> CoeffGroup::getSignificantCoeffFlags( ) const
	 *
	 * @brief	Zwraca mapê wspó³czynników znacz¹cych.
	 *
	 * @return	Mapê wspó³czynników znacz¹cych.
	 */

	boost::dynamic_bitset<> getSignificantCoeffFlags( ) const
	{
		return itsSignificantCoeffFlags;
	}

	/**
	 * @fn	void CoeffGroup::setSignificantCoeffFlags( boost::dynamic_bitset<> val )
	 *
	 * @brief	Ustawia mapê wspó³czynników znacz¹cych.
	 *
	 * @param	val	Mapa wspó³czynników znacz¹cych.
	 */

	void setSignificantCoeffFlags( boost::dynamic_bitset<> val )
	{
		itsSignificantCoeffFlags = val;
	}

	/**
	 * @fn	Bool CoeffGroup::getC2( ) const
	 *
	 * @brief	Zwraca bit C2.
	 *
	 * @return	Bit C2.
	 */

	Bool getC2( ) const
	{
		return itsC2;
	}

	/**
	 * @fn	void CoeffGroup::setC2( Bool val )
	 *
	 * @brief	Ustawia bit C2.
	 *
	 * @param	val	Nowy bit C2.
	 */

	void setC2( Bool val )
	{
		itsC2 = val;
	}
};


#endif