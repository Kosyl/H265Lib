/**
 * @file	Utils.cpp
 *
 * @brief Implementacja funkcji pomocniczych.
 */

#include "Utils.h"
#include <cassert>

int log2Int( const int value )
{
	assert( value > 0 );

	int log2 = 0, val = value;
	for( ; val != 1; val >>= 1, log2++ );
	return log2;
}

Int numBits( const int value )
{
	return log2Int( value ) + 1;
}

UInt numBits( const UInt value )
{
	return numBits( (Int)value );
}

QTComponent& operator++( QTComponent& orig )
{
	orig = static_cast<QTComponent>( orig + 1 );
	return orig;
}

boost::dynamic_bitset<> reverseBitset( boost::dynamic_bitset<>& set )
{
	boost::dynamic_bitset<> res;
	for( Int i = set.size( ) - 1; i >= 0; --i )
	{
		res.push_back( set[ i ] );
	}
	return res;
}

Bool goToMarker( std::ifstream& str, std::string key )
{
	std::string s, log, value;
	Bool end = false;
	do
	{
		str >> s;
		if( str.eof( ) || str.fail( ) )
			return false;
		if( s == "$" )
		{
			str >> s;
			if( str.eof( ) || str.fail( ) )
				return false;
			if( s == key )
			{
				end = true;
				break;
			}
		}
	}
	while( !str.eof( ) && !str.fail( ) );
	if( !end )
		return false;

	return true;
}

std::string bitsetToString( boost::dynamic_bitset<>& set )
{
	std::string s;
	boost::to_string( set, s );
	return s;
}
