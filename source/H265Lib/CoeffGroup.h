/**
 * @file	CoeffGroup.h
 *
 * @brief	Deklaracja klasy grupy wsp�czynnik�w.
 */

#ifndef _COEFFGROUP_H_
#define _COEFFGROUP_H_

#include "Utils.h"
#include <boost/dynamic_bitset.hpp>

/**
 * @class	CoeffGroup
 *
 * @brief	Reprezentacja grupy wsp�czynnik�w 4x4 w obr�bie bloku transformacji.
 */

class CoeffGroup
{
private:

	/**
	 * @brief	Uporz�dkowane wsp�czynniki.
	 */

	Coeff itsScan[ 16 ];

	/**
	 * @brief	Indeks pierwszego niezerowego wsp�czynnika w kolejno�ci skanowania.
	 */

	Short itsFirstPosInScan;

	/**
	 * @brief	Indeks ostatniego niezerowego wsp�czynnika w kolejno�ci skanowania.
	 */

	Short itsLastPosInScan;

	/**
	 * @brief	Mapa flag wsp�czynnik�w znacz�cych ( 1 gdy wsp�czynnik > 0, 0 w p.p.).
	 */

	boost::dynamic_bitset<> itsSignificantCoeffFlags;

	/**
	 * @brief	Mapa flag wsp�czynnik�w C1 (1 gdy wsp�czynnik > 1, 0 gdy wsp�czynnik == 1).
	 */

	boost::dynamic_bitset<> itsC1Group;

	/**
	 * @brief	Bit m�wi�cy, czy pierwszy wsp��zynnik wi�kszy od 1 jest wi�kszy tak�e od 2.
	 */

	Bool itsC2;

	/**
	 * @brief	Mapa znak�w ( + -> 0, - -> 1 )
	 */

	boost::dynamic_bitset<> itsCoeffSigns;

public:

	/**
	 * @fn	CoeffGroup::CoeffGroup( );
	 *
	 * @brief	Konstruktor domy�lny.
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
	 * @brief	Operator indeksowania, zwraca odpowiedni wsp�czynnik w kolejno�ci skanowania.
	 *
	 * @param	index	Indeks wsp�czynnika w kolejno�ci skanowania.
	 *
	 * @return Wsp�czynnik na danej pozycji.
	 */

	Coeff& operator[]( const int index )
	{
		assert( index >= 0 && index <= 15 );
		return itsScan[ index ];
	}

	/**
	 * @fn	Short CoeffGroup::getLastPosInScan( ) const
	 *
	 * @brief	Zwraca indeks ostatniego niezerowegeo wsp�czynnika w grupie.
	 *
	 * @return	Indeks ostatniego niezerowegeo wsp�czynnika w grupie.
	 */

	Short getLastPosInScan( ) const
	{
		return itsLastPosInScan;
	}

	/**
	 * @fn	void CoeffGroup::setLastPosInScan( Short val )
	 *
	 * @brief	Ustawia indeks ostatniego niezerowegeo wsp�czynnika w grupie.
	 *
	 * @param	val	Indeks ostatniego niezerowegeo wsp�czynnika w grupie.
	 */

	void setLastPosInScan( Short val )
	{
		itsLastPosInScan = val;
	}

	/**
	 * @fn	Short CoeffGroup::getFirstPosInScan( ) const
	 *
	 * @brief Zwraca indeks pierwszego niezerowegeo wsp�czynnika w grupie.
	 *
	 * @return	Indeks pierwszego niezerowegeo wsp�czynnika w grupie.
	 */

	Short getFirstPosInScan( ) const
	{
		return itsFirstPosInScan;
	}

	/**
	 * @fn	void CoeffGroup::setFirstPosInScan( Short val )
	 *
	 * @brief	Ustawia indeks pierwszego niezerowegeo wsp�czynnika w grupie.
	 *
	 * @param	val	Indeks pierwszego niezerowegeo wsp�czynnika w grupie.
	 */

	void setFirstPosInScan( Short val )
	{
		itsFirstPosInScan = val;
	}	

	/**
	 * @fn	boost::dynamic_bitset<> CoeffGroup::getCoeffSigns( ) const
	 *
	 * @brief	Zwraca map� znak�w.
	 *
	 * @return	Map� znak�w.
	 */

	boost::dynamic_bitset<> getCoeffSigns( ) const
	{
		return itsCoeffSigns;
	}

	/**
	 * @fn	void CoeffGroup::setCoeffSigns( boost::dynamic_bitset<> val )
	 *
	 * @brief Ustawia map� znak�w.
	 *
	 * @param	val	Nowa mapa znak�w.
	 */

	void setCoeffSigns( boost::dynamic_bitset<> val )
	{
		itsCoeffSigns = val;
	}

	/**
	 * @fn	boost::dynamic_bitset<> CoeffGroup::getC1Group( ) const
	 *
	 * @brief	Zwraca map� C1.
	 *
	 * @return Map� C1.
	 */

	boost::dynamic_bitset<> getC1Group( ) const
	{
		return itsC1Group;
	}

	/**
	 * @fn	void CoeffGroup::setC1Group( boost::dynamic_bitset<> val )
	 *
	 * @brief	Ustawia map� C1.
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
	 * @brief	Zwraca map� wsp�czynnik�w znacz�cych.
	 *
	 * @return	Map� wsp�czynnik�w znacz�cych.
	 */

	boost::dynamic_bitset<> getSignificantCoeffFlags( ) const
	{
		return itsSignificantCoeffFlags;
	}

	/**
	 * @fn	void CoeffGroup::setSignificantCoeffFlags( boost::dynamic_bitset<> val )
	 *
	 * @brief	Ustawia map� wsp�czynnik�w znacz�cych.
	 *
	 * @param	val	Mapa wsp�czynnik�w znacz�cych.
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