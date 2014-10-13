/**
 * @file	Quant.cpp
 *
 * @brief	Implementacja kalkulatora kwantyzacji
 */

#include "Quant.h"
#include <assert.h>
#include <algorithm>

/** Konstruktor domyslny, wylaczone logowanie
 *
 */
Quant::Quant( ) :
log( )
{
}

Quant* Quant::instance = nullptr;

Quant* Quant::getInstance( )
{
	if( instance == nullptr )
		instance = new Quant( );
	return instance;
}

/** Konstruktor przyjmuje sciezke do logu i rozkaz wlaczenia/nie wlaczania logu
 *	\param path sciezka do pliku logu
 \param isLogging pisze log, jesli true
 */
void Quant::initLog( std::string path, bool isLogging )
{
	log.changeLogPath( path );
	log.setLogging( isLogging );
}

Quant::~Quant( )
{
}

/** Kwantowanie podanego bloku
 *	\param inputCoeff tablica 2D probek wejsciowych
 *	\param result pusta tablica 2D wspolczynnikow wyjsciowych
 *	\param QP parametr kwantyzacji
 *	\param bitDepth glebia bitowa probek (8 bitow oznacza zakres <-256;255> )
 *	\param totalSize rozmiar blokow wejsciowych/wyjsciowych
 *	\param trSize rozmiar transformaty - tylko na potrzeby logowania, jesli np. transformata 4x4 w bloku 8x8 to funkcja moze zalogowac 4x4
 *	\return void
 */
void Quant::quant( Coeff** const inputCoeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize )
{
	Short log2TrSize;
	switch( trSize )
	{
	case 4:
		log2TrSize = 2;
		break;
	case 8:
		log2TrSize = 3;
		break;
	case 16:
		log2TrSize = 4;
		break;
	case 32:
		log2TrSize = 5;
		break;
	default:
		assert( 0 );
	}

	Int Q = forfardQuantCoeffs[ QP % 6 ];
	//Int offset = 1 << (20 + QP/6 - log2TrSize - (bitDepth-8) ); //offset, ALE w modelu jest wzor offset = 171 << (12+QP/6-log2TrSize-(bitDepth-8)); daje podobne wyniki
	Int offset = 171 << ( 12 + QP / 6 - log2TrSize - ( bitDepth - 8 ) );
	Int shift = ( 21 + QP / 6 - log2TrSize - ( bitDepth - 8 ) );

	for( int i = 0; i < totalSize; ++i )
	for( int j = 0; j < totalSize; ++j )
	{
		Int tmp;
		Int sign = inputCoeff[ i ][ j ] < 0 ? -1 : 1;
		tmp = ( ( ( (long long)( inputCoeff[ i ][ j ] ) ) *Q + ( sign == 1 ? offset : 2 * offset) ) >> shift );
		result[ i ][ j ] = std::min<Int>( std::max<Int>( -32768, tmp ), 32767 );
	}

	if( log.isLogging( ) )
	{
		log << "*** coefficients " << trSize << "x" << trSize << " QP=" << QP << " ***\n";
		if( log.includeDetails )
		{
			log << "*** bitDepth: " << bitDepth;

			log << "*** QP: " << QP << ", Q: " << Q << ", offset: " << offset << ", shift: " << shift << std::endl;
		}
		for( Int i = 0; i < totalSize; ++i )
		{
			for( Int j = 0; j < totalSize; ++j )
			{
				log << inputCoeff[ i ][ j ] << ( ( j == totalSize - 1 ) ? "" : " " );
			}
			log << std::endl;
		}
		log << "*** quantization " << trSize << "x" << trSize << " QP=" << QP << " ***\n";
		for( int i = 0; i < totalSize; ++i )
		{
			for( int j = 0; j < totalSize; ++j )
			{
				log << result[ i ][ j ] << ( ( j == totalSize - 1 ) ? "" : " " );
			}
			log << std::endl;
		}
	}
}

void Quant::quant( Coeff** const inputCoeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& trSize )
{
	quant( inputCoeff, result, QP, bitDepth, trSize, trSize );
}

/** Dekwantyzacja podanego bloku
 *	\param inputCoeff tablica 2D probek wejsciowych
 *	\param result pusta tablica 2D wspolczynnikow wyjsciowych
 *	\param QP parametr kwantyzacji
 *	\param bitDepth glebia bitowa probek (8 bitow oznacza zakres <-256;255> )
 *	\param totalSize rozmiar blokow wejsciowych/wyjsciowych
 *	\param trSize rozmiar transformaty - tylko na potrzeby logowania, jesli np. transformata 4x4 w bloku 8x8 to funkcja moze zalogowac 4x4
 *	\return void
 */
void Quant::deQuant( Coeff** const inputCoeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize )
{
	Short log2TrSize;
	switch( trSize )
	{
	case 4:
		log2TrSize = 2;
		break;
	case 8:
		log2TrSize = 3;
		break;
	case 16:
		log2TrSize = 4;
		break;
	case 32:
		log2TrSize = 5;
		break;
	default:
		assert( 0 );
	}
	Int IQ = inverseQuantCoeffs[ QP % 6 ];
	Int offset = 1 << ( log2TrSize - 2 + ( bitDepth - 8 ) );
	Int shift = log2TrSize - 1 + ( bitDepth - 8 );

	for( int i = 0; i < totalSize; ++i )
	{
		for( int j = 0; j < totalSize; ++j )
		{
			result[ i ][ j ] = std::min<Int>( std::max<Int>( -32768, inputCoeff[ i ][ j ] ), 32767 );

			result[ i ][ j ] = ( ( ( ( result[ i ][ j ] * IQ ) << QP / 6 ) + offset ) >> shift );
			result[ i ][ j ] = std::min<Int>( std::max<Int>( -32768, result[ i ][ j ] ), 32767 );
		}
	}
	if( log.isLogging( ) )
	{
		log << "*** dequantization " << trSize << "x" << trSize << " QP=" << QP << " ***\n";
		if( log.includeDetails )
		{
			log << "+++ IQ: " << IQ << ", offset: " << offset << ", shift: " << shift << " ***\n";
		}

		for( int i = 0; i < totalSize; ++i )
		{
			for( int j = 0; j < totalSize; ++j )
			{
				log << result[ i ][ j ] << ( ( j == totalSize - 1 ) ? "" : " " );
			}
			log << std::endl;
		}
	}
}
void Quant::deQuant( Coeff** const inputCoeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& trSize )
{
	deQuant( inputCoeff, result, QP, bitDepth, trSize, trSize );
}

const Short Quant::forfardQuantCoeffs[ 6 ] = { 26214, 23302, 20560, 18396, 16384, 14564 };
const Short Quant::inverseQuantCoeffs[ 6 ] = { 40, 45, 51, 57, 64, 72 };