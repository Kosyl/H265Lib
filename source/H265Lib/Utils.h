/**
 * @file	Utils.h
 *
 * @brief	Proste funkcje uzywane w roznych miejscach (header)
 */

#ifndef _UTILS_
#define _UTILS_

#include <algorithm>
#include "TypeDef.h"
#include "Logger.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 
#include <sstream>
#include <assert.h>
#include <memory>
#include "boost/dynamic_bitset.hpp"

#pragma region Enums

/**
 * @enum	PredictionType
 *
 * @brief	Reprezentacja typu predykcji
 */

enum PredictionType
{
	PREDICTION_INTRA = 0, PREDICTION_INTER = 1
};

/**
 * @enum	ImgComp
 *
 * @brief	Reprezentacja sk�adowej obrazu
 */

enum ImgComp
{
	LUMA = 0, CB = 1, CR = 2
};

/**
 * @enum	EdgeType
 *
 * @brief	Reprezentacja kiernku kraw�dzi
 */

enum EdgeType
{
	EDGE_VER = 0, EDGE_HOR = 1
};

/**
 * @enum	IntraDirection
 *
 * @brief	Reprezentacja kierunku pr�bek referencyjnych intra
 */

enum IntraDirection
{
	INTRA_DIR_LEFT = 0, INTRA_DIR_TOP = 1, INTRA_DIR_CORNER = 2
};

/**
 * @enum	IntraModeType
 *
 * @brief	Reprezentacja typu predykcji intra
 */

enum IntraModeType
{
	INTRAMODE_PLANAR = 0, INTRAMODE_DC = 1, INTRAMODE_LINEAR = 2, INTRAMODE_ANGULAR = 3
};

/**
 * @enum	ScanOrder
 *
 * @brief	Reprezentacja typu skanowania wsp�czynnik�w w bloku
 */

enum ScanOrder // 2 dla predMode 6-14, 1 dla 22-30, 0 wpp
{
	SCAN_DIAGONAL = 0, SCAN_HORIZONTAL = 1, SCAN_VERTICAL = 2
};

/**
 * @enum	SliceMode
 *
 * @brief	Reprezentacja typu plastra
 */

enum SliceMode
{
	SLICE_I = 0, SLCIE_P = 1, SLICE_B = 2
};

/**
 * @enum	SAOMode
 *
 * @brief	Reprezentacja typu SAO
 */

enum SAOMode
{
	SAO_NONE = 0, SAO_BAND = 1, SAO_EDGE = 2
};

/**
 * @enum	QTComponent
 *
 * @brief	Reprezentacja po�o�enia poddrzewa w czterodrzewie
 */

enum QTComponent
{
	UPPERLEFT = 0, UPPERRIGHT = 1, LOWERLEFT = 2, LOWERRIGHT = 3, QTCOMPONENT_FIRST = UPPERLEFT, QTCOMPONENT_LAST = LOWERRIGHT
};

/**
 * @fn	QTComponent& operator++( QTComponent& orig );
 *
 * @brief	Operator inkrementacji dla wyliczenia QTComponent
 *
 * @param [in,out]	orig	Wyj�ciowe po�o�enie.
 *
 * @return	Kolejny element wyliczenia
 */

QTComponent& operator++( QTComponent& orig );

/**
 * @enum	PartitionMode
 *
 * @brief	Reprezentacja podzia�u CU na jednostki predykcji
 */

enum PartitionMode
{
	PART_NxN, PART_2Nx2N, PART_2NxN, PART_Nx2N, PART_2NxnU, PART_2NxnD, PART_nLx2N, PART_nRx2N, PART_NOTSET
};

/**
 * @enum	QTMode
 *
 * @brief	Tryb elementu drzewa
 */

enum QTMode
{
	QTMODE_SPLIT, QTMODE_LEAF
};

#pragma endregion

/**
 * @fn	template <typename T> T** getEmptyMatrix( UInt size1, UInt size2 )
 *
 * @brief	Generator macierzy
 *
 * @tparam	T	Typ elementu macierzy
 * @param	size1	szeroko�� maceirzy
 * @param	size2	wysoko�� macierzy
 *
 * @return	wska�nik na now� macierz
 */

template <typename T>
T** getEmptyMatrix( UInt size1, UInt size2 )
{
	assert( size1 > 0 && size2 > 0 );
	T** tmp = new T*[ size1 ];
	for( UInt i = 0; i < size1; ++i )
	{
		tmp[ i ] = new T[ size2 ];
		memset( tmp[ i ], 0, size2*sizeof( T ) );
	}
	return tmp;
}

/**
 * @fn	template <typename T> void deleteMatrix( T**& matrix, UInt size )
 *
 * @brief	Kasowanie macierzy.
 *
 * @tparam	T	Typ macierzy
 * @param [in,out]	matrix	[in,out] Wska�nik do skasowania.
 * @param	size							Rozmiar macierzy
 */

template <typename T>
void deleteMatrix( T**& matrix, UInt size )
{
	if( matrix == nullptr )
		return;

	for( UInt i = 0; i < size; ++i )
	{
		if( matrix[ i ] != nullptr )
			delete[] matrix[ i ];

		matrix[ i ] = nullptr;
	}
	delete[] matrix;
	matrix = nullptr;
}

/**
 * @fn	template <typename T> void deleteMatrix( T**& matrix )
 *
 * @brief	Kasowanie macierzy.
 *
 * @tparam	T	Typ macierzy
 * @param [in,out]	matrix	[in,out] Wska�nik do skasowania.
 */

template <typename T>
void deleteMatrix( T**& matrix )
{
	if( matrix == nullptr )
		return;

	for( UInt i = 0; i < (sizeof(matrix)/sizeof(T*)); ++i )
	{
		if( matrix[ i ] != nullptr )
			delete[] matrix[ i ];

		matrix[ i ] = nullptr;
	}
	delete[] matrix;
	matrix = nullptr;
}

/**
 * @fn	template <typename T> inline T clipToRange( T minVal, T maxVal, T a )
 *
 * @brief	Obci�cie do warto�ci z zadanego przedzia�u <minVal, maxVal>
 *
 * @tparam	T	typ warto�ci
 * @param	minVal	Warto�� minimalna.
 * @param	maxVal	Warto�� maksymalna.
 * @param	a				Warto�� do obciecia.
 *
 * @return	Obci�ta warto��.
 */

template <typename T> inline T clipToRange( T minVal, T maxVal, T a )
{
	return std::min<T>( std::max<T>( minVal, a ), maxVal );
}

/**
 * @fn	Int log2Int( const Int );
 *
 * @brief	Logarytm dw�jkowy podanej liczby zaokr�glony w d�.
 *
 * @param	val	warto�� wej�ciowa.
 *
 * @return	floor(log2(int))
 */

Int log2Int( const Int val );

/**
 * @fn	Int numBits( const Int value );
 *
 * @brief	Ilo�� bit�w bitowej reprezentacji podanej liczby
 *
 * @param	value	liczba wej�ciowa.
 *
 * @return	floor(log2(int))+1
 */

Int numBits( const Int value );

/**
* @fn	UInt numBits( const UInt value );
*
* @brief	Ilo�� bit�w bitowej reprezentacji podanej liczby
*
* @param	value	liczba wej�ciowa.
*
* @return	floor(log2(int))+1
*/

UInt numBits( const UInt value );

/**
 * @fn	UInt getZScanIdxIn4x4( const UInt puX, const UInt puY );
 *
 * @brief	Pobranie indeksu Z-skanowania
 *
 * @param	puX	Wsp�rz�dna X jendostki predykcji
 * @param	puY	Wsp�rz�dna Y jendostki predykcji
 *
 * @return	Indeks Z-skanowania.
 */

UInt getZScanIdxIn4x4( const UInt puX, const UInt puY );

/**
 * @class	Column
 *
 * @brief	Kolumna w klasie Submatrix.
 *
 * @tparam	T	Typ warto�ci przechowywanej w kolumnie.
 */

template<typename T>
class Column
{
private:

	/**
	 * @brief	Wska�nik wyj�ciowy.
	 */

	T* itsStartPointer;

	/**
	 * @brief	Przesuni�cie w pionie.
	 */

	UInt itsOffsetY;
public:

	/**
	 * @fn	Column::Column( T* start, UInt offset )
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in,out]	start	Wska�nik bazowy.
	 * @param	offset				 	Przesuni�cie pocz�tku kolumny wzgl�dem wska�nika bazowego.
	 */

	Column( T* start, UInt offset ) :
		itsStartPointer( start ),
		itsOffsetY( offset )
	{
	}

	/**
	 * @fn	Column::Column( const Column& rhs )
	 *
	 * @brief	Konstruktor kopiuj�cy. (p�ytka kopia)
	 *
	 * @param	rhs	Kopiowana kolumna.
	 */

	Column( const Column& rhs ) :
		itsStartPointer( rhs.itsStartPointer ),
		itsOffsetY( rhs.itsOffsetY )
	{
	}

	/**
	 * @fn	Column::~Column( )
	 *
	 * @brief	Destruktor.
	 */

	~Column( )
	{
	}

	/**
	 * @fn	T& Column::operator[] ( const int index );
	 *
	 * @brief	Operator indeksowania.
	 *
	 * @param	index	Indeks warto�ci w kolumnie.
	 *
	 * @return	Warto�� z tabeli bazowej.
	 */

	T& operator[] ( const int index );

	/**
	 * @fn	Column& Column::operator=( Column const& rhs )
	 *
	 * @brief	Operator przypisania.
	 *
	 * @param	rhs	Kolumna, kt�ra jest przypisywana do obecnej
	 *
	 * @return	P�ytka kopia kolumny.
	 */

	Column& operator =( Column const& rhs )
	{
		itsStartPointer = rhs.itsStartPointer;
		itsOffsetY = rhs.itsOffsetY;
		return *this;
	}

};

/**
 * @class	Submatrix
 *
 * @brief	Klasa macierzy b�d�cej cz�ci� wi�kszej macierzy.
 *
 * @tparam	T	Typ zmiennej przechowywanej w macierzy.
 */

template <typename T>
class Submatrix
{
private:

	/**
	 * @property	std::vector<Column<T> > itsColumns
	 *
	 * @brief	Lista kolumn podmacierzy.
	 */

	std::vector<Column<T> > itsColumns;

	/**
	 * @brief	Rozmiar macierzy.
	 */

	UInt itsSize;

public:

	/**
	 * @fn	Submatrix::Submatrix( )
	 *
	 * @brief	Konstruktor domy�lny.
	 */

	Submatrix( ) :
		itsSize( 0 ),
		itsColumns( )
	{
	}

	/**
	 * @fn	Submatrix::Submatrix( T** corePointer, UInt size, UInt offsetX = 0, UInt offsetY = 0 )
	 *
	 * @brief	Konstruktor.
	 *
	 * @param [in]	corePointer			Wska�nik wyj�ciowy.
	 * @param	size								 	Rozmiar podmacierzy.
	 * @param	offsetX							 	Przesuni�cie podmacierzy w poziomie wzgl�dem macierzy bazowej.
	 * @param	offsetY							 	Przesuni�cie podmacierzy w pionie wzgl�dem macierzy bazowej.
	 */

	Submatrix( T** corePointer, UInt size, UInt offsetX = 0, UInt offsetY = 0 ) :
		itsSize( size ),
		itsColumns( )
	{
		for( UInt i = 0; i < size; ++i )
		{
			itsColumns.push_back( Column<T>( corePointer[ offsetX + i ], offsetY ) );
		}
	}

	/**
	 * @fn	Submatrix::~Submatrix( )
	 *
	 * @brief	Destruktor.
	 */

	~Submatrix( )
	{
	}

	/**
	 * @fn	UInt Submatrix::size( ) const
	 *
	 * @brief	Zwraca rozmiar podmacierzy.
	 *
	 * @return	Rozmiar podmacierzy.
	 */

	UInt size( ) const
	{
		return itsSize;
	}

	/**
	 * @fn	Column<T>& Submatrix::operator[] ( const UInt index );
	 *
	 * @brief	Operator indeksowania.
	 *
	 * @param	index	Indeks kolumny
	 *
	 * @return	index-ta kolumna podmacierzy.
	 */

	Column<T>& operator[] ( const UInt index );

	/**
	 * @fn	Submatrix& Submatrix::operator=( Submatrix const& rhs )
	 *
	 * @brief Operator przypisania. (p�ytka kopia)
	 *
	 * @param	rhs	Przypisywana podmacierz.
	 *
	 * @return	P�ytka kopia przypisywanej macierzy.
	 */

	Submatrix& operator =( Submatrix const& rhs )
	{
		itsSize = rhs.itsSize;
		itsColumns.clear( );
		itsColumns = rhs.itsColumns;
		return *this;
	}
};

/**
 * @fn	template<typename T> Column<T>& Submatrix<T>::operator[] ( const UInt index )
 *
 * @brief	Operator indeksowania podmacierzy (zwraca kolumn�)
 *
 * @tparam	T	Typ danych macierzy.
 * @param	index	Indeks kolumny.
 *
 * @return	Kolumna podmacierzy.
 */

template<typename T>
Column<T>& Submatrix<T>::operator[] ( const UInt index )
{
	assert( index < itsSize );
	return itsColumns[ index ];
}

/**
 * @fn	template<typename T> T& Column<T>::operator[] ( const int index )
 *
 * @brief	Operator indeksowania kolumny. (zwraca warto��).
 *
 * @tparam	T	Typ przechowywany w kolumnie.
 * @param	index	Indeks warto�ci w kolumnie.
 *
 * @return	Referencja do warto�ci znajduj�cej si� pod danym indeksem w kolumnie.
 */

template<typename T>
T& Column<T>::operator[] ( const int index )
{
	return itsStartPointer[ itsOffsetY + index ];
}

/**
 * @fn	template <typename T> Void printMatrix( T** matrix, UInt size, std::ostream& out = std::cout )
 *
 * @brief Wypisanie macierzy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w macierzy.
 * @param [in]	matrix	Wska�nik na macierz do wypisania.
 * @param	size					Rozmiar macierzy.
 * @param [in]	out			Strumie� wyj�ciowy.
 */

template <typename T>
Void printMatrix( T** matrix, UInt size, std::ostream& out = std::cout )
{
	printMatrix<T>( matrix, size, size, out );
}

/**
 * @fn	template <typename T> Void printMatrix( T** matrix, UInt sizeX, UInt sizeY, std::ostream& out = std::cout, UInt offsetX = 0, UInt offsetY = 0, std::string tabs = "" )
 *
 * @brief	Wypisanie macierzy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w macierzy.
 * @param [in]	matrix	Wska�nik na macierz do wypisania.
 * @param	sizeX					Rozmiar X macierzy.
 * @param	sizeY					Rozmiar Y macierzy.
 * @param [in]	out			Strumie� wyj�ciowy.
 * @param	offsetX				Offset X pocz�tku wypisywania.
 * @param	offsetY				Offset Y pocz�tku wypisywania.
 * @param	tabs					Tabulacja na pocz�tku ka�dego wiersza.
 */

template <typename T>
Void printMatrix( T** matrix, UInt sizeX, UInt sizeY, std::ostream& out = std::cout, UInt offsetX = 0, UInt offsetY = 0, std::string tabs = "" )
{
	for( UInt i = offsetY; i < offsetY + sizeY; ++i )
	{
		out << tabs;
		for( UInt j = offsetX; j < offsetX + sizeX; ++j )
		{
			out << std::setw( 0 ) << matrix[ j ][ i ] << " ";
		}
		out << std::endl;
	}
}

/**
 * @fn	template <typename T> Void printMatrix( T** matrix, UInt size, Logger& out )
 *
 * @brief	Wypisanie macierzy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w macierzy.
 * @param [in]	matrix	Wska�nik na macierz do wypisania.
 * @param	size					Rozmiar macierzy.
 * @param [in]	out			Obiekt loguj�cy do pliku.
 */

template <typename T>
Void printMatrix( T** matrix, UInt size, Logger& out )
{
	printMatrix<T>( matrix, size, size, out );
}

/**
 * @fn	template <typename T> Void printMatrix( T** matrix, UInt sizeX, UInt sizeY, Logger& out, UInt offsetX = 0, UInt offsetY = 0, std::string tabs = "" )
 *
 * @brief	Wypisanie macierzy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w macierzy.
 * @param [in]	matrix	Wska�nik na macierz do wypisania.
 * @param	sizeX					Rozmiar X macierzy.
 * @param	sizeY					Rozmiar Y macierzy.
 * @param [in]	out			Obiekt loguj�cy do pliku.
 * @param	offsetX				Offset X pocz�tku wypisywania.
 * @param	offsetY				Offset Y pocz�tku wypisywania.
 * @param	tabs					Tabulacja na pocz�tku ka�dego wiersza.
 */

template <typename T>
Void printMatrix( T** matrix, UInt sizeX, UInt sizeY, Logger& out, UInt offsetX = 0, UInt offsetY = 0, std::string tabs = "" )
{
	for( UInt i = offsetY; i < offsetY + sizeY; ++i )
	{
		out << tabs;
		for( UInt j = offsetX; j < offsetX + sizeX; ++j )
		{
			out << std::setw( 0 ) << matrix[ j ][ i ] << " ";
		}
		out << std::endl;
	}
}

/**
 * @fn	template <typename T> Void printMatrix( Submatrix<T>& matrix, UInt size, std::ostream& out = std::cout )
 *
 * @brief	Wypisanie macierzy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w macierzy.
 * @param [in]	matrix  Macierz do wypisania.
 * @param	size					Rozmiar macierzy.
 * @param [in]	out			Strumie� wyj�ciowy.
 */

template <typename T>
Void printMatrix( Submatrix<T>& matrix, UInt size, std::ostream& out = std::cout )
{
	printMatrix<T>( matrix, size, size, out );
}

/**
 * @fn	template <typename T> Void printMatrix( Submatrix<T>& matrix, UInt sizeX, UInt sizeY, std::ostream& out = std::cout, UInt offsetX = 0, UInt offsetY = 0, std::string tabs = "" )
 *
 * @brief	Wypisanie macierzy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w macierzy.
 * @param [in]	matrix	The matrix.
 * @param	sizeX					Rozmiar X macierzy.
 * @param	sizeY					Rozmiar Y macierzy.
 * @param [in]	out			Strumie� wyj�ciowy.
 * @param	offsetX				Offset X pocz�tku wypisywania.
 * @param	offsetY				Offset Y pocz�tku wypisywania.
 * @param	tabs					Tabulacja na pocz�tku ka�dego wiersza.
 */

template <typename T>
Void printMatrix( Submatrix<T>& matrix, UInt sizeX, UInt sizeY, std::ostream& out = std::cout, UInt offsetX = 0, UInt offsetY = 0, std::string tabs = "" )
{
	for( UInt i = offsetY; i < offsetY + sizeY; ++i )
	{
		out << tabs;
		for( UInt j = offsetX; j < offsetX + sizeX; ++j )
		{
			out << std::setw( 0 ) << matrix[ j ][ i ] << " ";
		}
		out << std::endl;
	}
}

/**
 * @fn	template <typename T> Void printMatrix( Submatrix<T>& matrix, UInt size, Logger& out )
 *
 * @brief	Wypisanie macierzy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w macierzy.
 * @param [in]	matrix	Macierz do wypisania.
 * @param	size					Rozmiar macierzy.
 * @param [in]	out			Obiekt loguj�cy do pliku.
 */

template <typename T>
Void printMatrix( Submatrix<T>& matrix, UInt size, Logger& out )
{
	printMatrix<T>( matrix, size, size, out );
}

/**
 * @fn	template <typename T> Void printMatrix( Submatrix<T>& matrix, UInt sizeX, UInt sizeY, Logger& out, UInt offsetX = 0, UInt offsetY = 0, std::string tabs = "" )
 *
 * @brief	Wypisanie macierzy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w macierzy.
 * @param [in]	matrix	The matrix.
 * @param	sizeX					Rozmiar X macierzy.
 * @param	sizeY					Rozmiar Y macierzy.
 * @param [in]	out			Obiekt loguj�cy do pliku.
 * @param	offsetX				Offset X pocz�tku wypisywania.
 * @param	offsetY				Offset Y pocz�tku wypisywania.
 * @param	tabs					Tabulacja na pocz�tku ka�dego wiersza.
 */

template <typename T>
Void printMatrix( Submatrix<T>& matrix, UInt sizeX, UInt sizeY, Logger& out, UInt offsetX = 0, UInt offsetY = 0, std::string tabs = "" )
{
	for( UInt i = offsetY; i < offsetY + sizeY; ++i )
	{
		out << tabs;
		for( UInt j = offsetX; j < offsetX + sizeX; ++j )
		{
			out << std::setw( 0 ) << matrix[ j ][ i ] << " ";
		}
		out << std::endl;
	}
}

/**
 * @fn	template <typename T> Void printTable( T* matrix, UInt size, std::ostream& out = std::cout, UInt width = 4 )
 *
 * @brief	Wypisanie tablicy na strumie� wyj�ciowy
 *
 * @tparam	T	Typ danych przechowywany w tablicy.
 * @param [in]	matrix	Tabela do wypisania.
 * @param	size					Rozmiar macierzy.
 * @param [in]	out			Strumie� wyj�ciowy.
 * @param	width					Ilo�� miejsc, jak� zajmie ka�da wypisana liczba.
 */

template <typename T>
Void printTable( T* matrix, UInt size, std::ostream& out = std::cout, UInt width = 0 )
{
	if( size == 0 )
		return;

	for( UInt j = 0; j < size; ++j )
	{
		if( width > 0 )
			out << std::setw( width );
		out << matrix[ j ] << " ";
	}
	out << std::endl;
}

/**
 * @fn	template <typename T> Void printTable( T* matrix, UInt size, Logger& out, UInt width = 4 )
 *
 * @brief Wypisanie tablicy na strumie� wyj�ciowy.
 *
 * @tparam	T	Typ danych przechowywany w tablicy.
 * @param [in]	matrix	Tabela do wypisania.
 * @param	size					Rozmiar macierzy.
 * @param [in]	out			Obiekt loguj�cy do pliku.
 * @param	width					Ilo�� miejsc, jak� zajmie ka�da wypisana liczba.
 */

template <typename T>
Void printTable( T* matrix, UInt size, Logger& out, UInt width = 0 )
{
	if( size == 0 )
		return;

	for( UInt j = 0; j < size; ++j )
	{
		if( width > 0 )
			out << std::setw( width );
		out << matrix[ j ] << " ";
	}
	out << std::endl;
}

/**
 * @fn	Bool goToMarker( std::ifstream& str, std::string key );
 *
 * @brief	Przesuni�cie si� w strumieniu do zadanego markera.
 *
 * @param [in]	strumie� wej�ciowy.
 * @param	key		Nazwa markera.
 *
 * @return	true je�li marker zostanie odnaleziony, flase w p.p.
 */

Bool goToMarker( std::ifstream& str, std::string key );

/**
 * @fn	template <typename T> Bool readMatrix( std::ifstream& str, std::string key, T**& out_matrix, UInt& out_width, UInt& out_height )
 *
 * @brief	Wczytuje macierz o nazwie "key" ze strumienia wejs�iowego.
 *
 * @tparam	T	Typ danych w macierzy
 * @param [in]	str					Strumie� wej�ciowy.
 * @param	key										Nazwa macierzy w pliku.
 * @param [out]	out_matrix	[out] Wska�nik na wczytan� macierz.
 * @param [out]	out_width 	Wczytana szeroko�� macierzy.
 * @param [out]	out_height	Wczytana wysoko�� macierzy.
 *
 * @return	true je�li macierz zostanie odnalezione i wczytana; false w p.p.
 */

template <typename T>
Bool readMatrix( std::ifstream& str, std::string key, T**& out_matrix, UInt& out_width, UInt& out_height )
{
	try
	{
		if( !goToMarker( str, key ) || str.eof( ) || str.fail( ) )
			return false;
		if( !goToMarker( str, "WIDTH" ) || str.eof( ) || str.fail( ) )
			return false;
		str >> out_width;
		if( str.eof( ) || str.fail( ) )
			return false;

		if( !goToMarker( str, "HEIGHT" ) || str.eof( ) || str.fail( ) )
			return false;
		str >> out_height;
		if( str.eof( ) || str.fail( ) )
			return false;
		if( !goToMarker( str, "MATRIX" ) || str.eof( ) || str.fail( ) )
			return false;
		out_matrix = getEmptyMatrix<T>( out_width, out_height );

		T tmp;
		for( UInt i = 0; i < out_height; ++i )
		{
			for( UInt j = 0; j < out_width; ++j )
			{
				str >> tmp;
				if( str.eof( ) || str.fail( ) )
					return false;
				out_matrix[ j ][ i ] = tmp;
			}
		}
	}
	catch(...)
	{
		return false;
	}
	return true;
}

/**
 * @fn	template <typename T> Bool readValue( std::ifstream& str, std::string key, T& out_value )
 *
 * @brief	Wczytanie zmiennej o nazwie "key" ze strumienia.
 *
 * @tparam	T	Typ wczytanej warto�ci.
 * @param [in]	str			 	Strumie� wej�ciowy.
 * @param	key								 	Nazwa zmiennej.
 * @param [out]	out_value	Wczytana wartos�.
 *
 * @return	true je�li warto�� zostanie odnaleziona i wczytana; false w p.p.
 */

template <typename T>
Bool readValue( std::ifstream& str, std::string key, T& out_value )
{
	try
	{
		if( !goToMarker( str, key ) || str.eof( ) || str.fail( ) )
			return false;
		str >> out_value;
		if( str.eof( ) || str.fail( ) )
			return false;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

/**
 * @fn	boost::dynamic_bitset<> reverseBitset( boost::dynamic_bitset<>& set );
 *
 * @brief	Funkcja odwraca kolejno�� bit�w w obiekcie boost::dynamic_bitset
 *
 * @param [in,out]	set	Bitset do odwr�cenia.
 *
 * @return	Nowy bitset b�d�cy obr�conym bitsetem wej�ciowym.
 */

boost::dynamic_bitset<> reverseBitset( boost::dynamic_bitset<>& set );

/**
 * @class	Constants
 *
 * @brief	Zbi�r sta�ych.
 */

class Constants
{
public:

	/**
	 * @brief Rozmiar grupy CG.
	 */

	static const Int CG_SIZE = 4;

	/**
	 * @brief	Ilo�� wsp�czynik�w w grupie CG.
	 */

	static const Int NUM_COEFFS_IN_CG = 16;

	/**
	 * @brief Pr�g zmiany rodzaju kodowania reszty wsp�czynnika na kodowanie Golomba
	 */

	static const Int COEFF_REMAIN_THRESHOLD = 3;

	/**
	 * @brief	Pr�g odleg�o�ci mi�dzy wsp�czynnikami pozwalaj�cy na u�ycie trybu SDH.
	 */

	static const Int SDH_THRESHOLD = 4;

};

/**
 * @fn	std::string bitsetToString( boost::dynamic_bitset<>& set );
 *
 * @brief	Zamiana bitsetu na ci�g znak�w "0" i "1".
 *
 * @param [in] Bitset do zamiany na string.
 *
 * @return	Obiekt std::string b�d� cy reprezentacj� bitsetu.
 */

std::string bitsetToString( boost::dynamic_bitset<>& set );

#endif