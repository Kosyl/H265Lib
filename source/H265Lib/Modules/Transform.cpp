///**
// * @file	Transform.cpp
// *
// * @brief	Implementacja klasy transformacji.
// */
//
//#include "Transform.h"
//#include "Utils.h"
//#include <assert.h>
//
//using namespace std;
//
///** Konstruktor domyslny, wylaczone logowanie
//*
//*/
//Transform* Transform::instance = nullptr;
//
//Transform* Transform::getInstance( )
//{
//	if( instance == nullptr )
//		instance = new Transform( );
//	return instance;
//}
//
///** Konstruktor przyjmuje sciezke do logu i rozkaz wlaczenia/nie wlaczania logu
// *	\param path sciezka do pliku logu
// \param isLogging pisze log, jesli true
// */
//void Transform::initLog( std::string path, bool isLogging )
//{
//	//log.changeLogPath( path );
//	//log.setLogging( isLogging );
//}
//
//Transform::Transform( ) 
//{
//	afterHor = getEmptyMatrix<Int>( 32, 32 );
//}
//
///** Konstruktor przyjmuje sciezke do logu i rozkaz wlaczenia/nie wlaczania logu
//*	\param path sciezka do pliku logu
//\param isLogging pisze log, jesli true
//*/
//
//Transform::~Transform( )
//{
//}
//
///** Transformata prosta DCT/DST
//*	\param block tablica 2D probek wejsciowych
//*	\param coeff pusta tablica 2D wspolczynnikow wyjsciowych
//*	\param trSize rozmiar transformaty
//*	\param bitDepth glebia bitowa probek w tablicy block (8 bitow oznacza zakres <-256;255> )
//*	\param isIntra4x4LumaDST_hor czy uzywa DST w poziomie (1sza faza)
//*	\param isIntra4x4LumaDST_ver czy uzywa DST w pionie (2ga faza)
//*	\return void
//*/
//void Transform::forwardTrans( Sample** const block, Coeff** coeff, const Int& trSize, const Int& bitDepth, const Bool& isIntra4x4LumaDST_hor, const Bool& isIntra4x4LumaDST_ver )
//{
//	Int i, j, k, iSum;
//
//	const Short *iT; // Wskaznik na odpowiednia macierz wspolczynnikow transformaty, wybierany na podstawie rozmiaru i trybu
//	UShort log2TrSize;
//
//	if( trSize == 4 )
//	{
//		log2TrSize = 2;
//		if( isIntra4x4LumaDST_hor ) // DST tylko dla 4x4
//		{
//			iT = TCOEFF_DST_4[ 0 ];
//		}
//		else
//		{
//			iT = TCOEFF_4[ 0 ];
//		}
//	}
//	else if( trSize == 8 )
//	{
//		log2TrSize = 3;
//		iT = TCOEFF_8[ 0 ];
//	}
//	else if( trSize == 16 )
//	{
//		log2TrSize = 4;
//		iT = TCOEFF_16[ 0 ];
//	}
//	else if( trSize == 32 )
//	{
//		log2TrSize = 5;
//		iT = TCOEFF_32[ 0 ];
//	}
//	else
//	{
//		assert( 0 );
//	}
//
//	int shift_1st = log2TrSize - 1 + bitDepth - 8; // przesuniecie po pierwszej fazie 
//	Int add_1st = 1 << ( shift_1st - 1 ); // offset po pierwszej fazie
//	Int shift_2nd = log2TrSize + 6; // przesuniecie po drugiej fazie
//	Int add_2nd = 1 << ( shift_2nd - 1 ); // offset po drugiej fazie
//
//	//transformata pozioma
//	for( i = 0; i < trSize; ++i )
//	{
//		for( j = 0; j < trSize; ++j )
//		{
//			iSum = 0;
//			for( k = 0; k < trSize; ++k )
//			{
//				iSum += iT[ i*trSize + k ] * block[ k ][ j ];
//			}
//			afterHor[ i ][ j ] = ( iSum + add_1st ) >> shift_1st;
//		}
//	}
//
//	//transformata pionowa
//	if( trSize == 4 )
//	{
//		if( isIntra4x4LumaDST_ver )//ponowne sprawdzenie DST
//		{
//			iT = TCOEFF_DST_4[ 0 ];
//		}
//		else
//		{
//			iT = TCOEFF_4[ 0 ];
//		}
//	}
//	for( i = 0; i < trSize; ++i )
//	{
//		for( j = 0; j < trSize; ++j )
//		{
//			iSum = 0;
//			for( k = 0; k < trSize; ++k )
//			{
//				iSum += iT[ i*trSize + k ] * afterHor[ j ][ k ];
//			}
//			coeff[ j ][ i ] = ( iSum + add_2nd ) >> shift_2nd;
//		}
//	}
//}
//
///** Transformata odwrotna
//*	\param coeff Wskaznik na tablice wartosci wejsciowych (trSize*trSize)
//*	\param block Wskaznik na tablice probek trSize*trSize (wartosci wyjsciowe)
//*	\param trSize Rozmiar bloku transformaty (4,8,16 lub 32)
//*	\param bitDepth Glebia bitowa probek (max 16)
//*	\param isIntra4x4LumaDST_hor czy uzywa DST w poziomie (1sza faza)
//*	\param isIntra4x4LumaDST_ver czy uzywa DST w pionie (2ga faza)
//*	\return void
//*/
//void Transform::inverseTrans( Coeff** const coeff, Sample** block, const Int& trSize, const Int& bitDepth, const Bool& isIntra4x4LumaDST_hor, const Bool& isIntra4x4LumaDST_ver )
//{
//	Int i, j, k;
//	Int iSum;
//
//	const Short* iT; // wskaznik na odpowiednia tablice z macierza transformaty
//
//	if( trSize == 4 )
//	{
//		if( isIntra4x4LumaDST_hor ) // DCT czy DST tylko dla 4x4
//		{
//			iT = TCOEFF_DST_4[ 0 ];
//		}
//		else
//		{
//			iT = TCOEFF_4[ 0 ];
//		}
//	}
//	else if( trSize == 8 )
//	{
//		iT = TCOEFF_8[ 0 ];
//	}
//	else if( trSize == 16 )
//	{
//		iT = TCOEFF_16[ 0 ];
//	}
//	else if( trSize == 32 )
//	{
//		iT = TCOEFF_32[ 0 ];
//	}
//	else
//	{
//		assert( 0 );
//	}
//
//	Int shift_1st = SHIFT_INV_1; // przesuniecie po pierwszej fazie
//	Int add_1st = 1 << ( shift_1st - 1 ); // offset po pierwszej fazie
//	Int shift_2nd = SHIFT_INV_2 - ( bitDepth - 8 );  // przesuniecie po drugiej fazie
//	Int add_2nd = 1 << ( shift_2nd - 1 ); // offset po drugiej fazie
//
//	/* Transformacja pozioma */
//	for( i = 0; i < trSize; ++i )
//	{
//		for( j = 0; j < trSize; ++j )
//		{
//			iSum = 0;
//			for( k = 0; k < trSize; ++k )
//			{
//				iSum += iT[ k*trSize + i ] * coeff[ j ][ k ];
//			}
//			afterHor[ j ][ i ] = std::min<int>( std::max<int>( -32768, ( iSum + add_1st ) >> shift_1st ), 32767 );
//		}
//	}
//
//	/* Transformacja pionowa */
//	if( trSize == 4 )
//	{
//		if( isIntra4x4LumaDST_ver ) // DCT czy DST
//		{
//			iT = TCOEFF_DST_4[ 0 ];
//		}
//		else
//		{
//			iT = TCOEFF_4[ 0 ];
//		}
//	}
//	for( i = 0; i < trSize; ++i )
//	{
//		for( j = 0; j < trSize; ++j )
//		{
//			iSum = 0;
//			for( k = 0; k < trSize; ++k )
//			{
//				iSum += iT[ k*trSize + j ] * afterHor[ k ][ i ];
//			}
//			block[ j ][ i ] = std::min<int>( std::max<int>( -32768, ( iSum + add_2nd ) >> shift_2nd ), 32767 );
//		}
//	}
//}
//
///**
//*	\var Short[][] TCOEFF_4
//*	\brief Macierz transformaty DCT 4x4
//*/
//const Short Transform::TCOEFF_4[ 4 ][ 4 ] =
//{
//	{ 64, 64, 64, 64 },
//	{ 83, 36, -36, -83 },
//	{ 64, -64, -64, 64 },
//	{ 36, -83, 83, -36 }
//};
//
///**
//*	\var Short[][] TCOEFF_8
//*	\brief Macierz transformaty DCT 8x8
//*/
//const Short Transform::TCOEFF_8[ 8 ][ 8 ] =
//{
//	{ 64, 64, 64, 64, 64, 64, 64, 64 },
//	{ 89, 75, 50, 18, -18, -50, -75, -89 },
//	{ 83, 36, -36, -83, -83, -36, 36, 83 },
//	{ 75, -18, -89, -50, 50, 89, 18, -75 },
//	{ 64, -64, -64, 64, 64, -64, -64, 64 },
//	{ 50, -89, 18, 75, -75, -18, 89, -50 },
//	{ 36, -83, 83, -36, -36, 83, -83, 36 },
//	{ 18, -50, 75, -89, 89, -75, 50, -18 }
//};
//
///**
//*	\var Short[][] TCOEFF_16
//*	\brief Macierz transformaty DCT 16x16
//*/
//const Short Transform::TCOEFF_16[ 16 ][ 16 ] =
//{
//	{ 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 },
//	{ 90, 87, 80, 70, 57, 43, 25, 9, -9, -25, -43, -57, -70, -80, -87, -90 },
//	{ 89, 75, 50, 18, -18, -50, -75, -89, -89, -75, -50, -18, 18, 50, 75, 89 },
//	{ 87, 57, 9, -43, -80, -90, -70, -25, 25, 70, 90, 80, 43, -9, -57, -87 },
//	{ 83, 36, -36, -83, -83, -36, 36, 83, 83, 36, -36, -83, -83, -36, 36, 83 },
//	{ 80, 9, -70, -87, -25, 57, 90, 43, -43, -90, -57, 25, 87, 70, -9, -80 },
//	{ 75, -18, -89, -50, 50, 89, 18, -75, -75, 18, 89, 50, -50, -89, -18, 75 },
//	{ 70, -43, -87, 9, 90, 25, -80, -57, 57, 80, -25, -90, -9, 87, 43, -70 },
//	{ 64, -64, -64, 64, 64, -64, -64, 64, 64, -64, -64, 64, 64, -64, -64, 64 },
//	{ 57, -80, -25, 90, -9, -87, 43, 70, -70, -43, 87, 9, -90, 25, 80, -57 },
//	{ 50, -89, 18, 75, -75, -18, 89, -50, -50, 89, -18, -75, 75, 18, -89, 50 },
//	{ 43, -90, 57, 25, -87, 70, 9, -80, 80, -9, -70, 87, -25, -57, 90, -43 },
//	{ 36, -83, 83, -36, -36, 83, -83, 36, 36, -83, 83, -36, -36, 83, -83, 36 },
//	{ 25, -70, 90, -80, 43, 9, -57, 87, -87, 57, -9, -43, 80, -90, 70, -25 },
//	{ 18, -50, 75, -89, 89, -75, 50, -18, -18, 50, -75, 89, -89, 75, -50, 18 },
//	{ 9, -25, 43, -57, 70, -80, 87, -90, 90, -87, 80, -70, 57, -43, 25, -9 }
//};
//
///**
//*	\var Short[][] TCOEFF_32
//*	\brief Macierz transformaty DCT 32x32
//*/
//const Short Transform::TCOEFF_32[ 32 ][ 32 ] =
//{
//	{ 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 },
//	{ 90, 90, 88, 85, 82, 78, 73, 67, 61, 54, 46, 38, 31, 22, 13, 4, -4, -13, -22, -31, -38, -46, -54, -61, -67, -73, -78, -82, -85, -88, -90, -90 },
//	{ 90, 87, 80, 70, 57, 43, 25, 9, -9, -25, -43, -57, -70, -80, -87, -90, -90, -87, -80, -70, -57, -43, -25, -9, 9, 25, 43, 57, 70, 80, 87, 90 },
//	{ 90, 82, 67, 46, 22, -4, -31, -54, -73, -85, -90, -88, -78, -61, -38, -13, 13, 38, 61, 78, 88, 90, 85, 73, 54, 31, 4, -22, -46, -67, -82, -90 },
//	{ 89, 75, 50, 18, -18, -50, -75, -89, -89, -75, -50, -18, 18, 50, 75, 89, 89, 75, 50, 18, -18, -50, -75, -89, -89, -75, -50, -18, 18, 50, 75, 89 },
//	{ 88, 67, 31, -13, -54, -82, -90, -78, -46, -4, 38, 73, 90, 85, 61, 22, -22, -61, -85, -90, -73, -38, 4, 46, 78, 90, 82, 54, 13, -31, -67, -88 },
//	{ 87, 57, 9, -43, -80, -90, -70, -25, 25, 70, 90, 80, 43, -9, -57, -87, -87, -57, -9, 43, 80, 90, 70, 25, -25, -70, -90, -80, -43, 9, 57, 87 },
//	{ 85, 46, -13, -67, -90, -73, -22, 38, 82, 88, 54, -4, -61, -90, -78, -31, 31, 78, 90, 61, 4, -54, -88, -82, -38, 22, 73, 90, 67, 13, -46, -85 },
//	{ 83, 36, -36, -83, -83, -36, 36, 83, 83, 36, -36, -83, -83, -36, 36, 83, 83, 36, -36, -83, -83, -36, 36, 83, 83, 36, -36, -83, -83, -36, 36, 83 },
//	{ 82, 22, -54, -90, -61, 13, 78, 85, 31, -46, -90, -67, 4, 73, 88, 38, -38, -88, -73, -4, 67, 90, 46, -31, -85, -78, -13, 61, 90, 54, -22, -82 },
//	{ 80, 9, -70, -87, -25, 57, 90, 43, -43, -90, -57, 25, 87, 70, -9, -80, -80, -9, 70, 87, 25, -57, -90, -43, 43, 90, 57, -25, -87, -70, 9, 80 },
//	{ 78, -4, -82, -73, 13, 85, 67, -22, -88, -61, 31, 90, 54, -38, -90, -46, 46, 90, 38, -54, -90, -31, 61, 88, 22, -67, -85, -13, 73, 82, 4, -78 },
//	{ 75, -18, -89, -50, 50, 89, 18, -75, -75, 18, 89, 50, -50, -89, -18, 75, 75, -18, -89, -50, 50, 89, 18, -75, -75, 18, 89, 50, -50, -89, -18, 75 },
//	{ 73, -31, -90, -22, 78, 67, -38, -90, -13, 82, 61, -46, -88, -4, 85, 54, -54, -85, 4, 88, 46, -61, -82, 13, 90, 38, -67, -78, 22, 90, 31, -73 },
//	{ 70, -43, -87, 9, 90, 25, -80, -57, 57, 80, -25, -90, -9, 87, 43, -70, -70, 43, 87, -9, -90, -25, 80, 57, -57, -80, 25, 90, 9, -87, -43, 70 },
//	{ 67, -54, -78, 38, 85, -22, -90, 4, 90, 13, -88, -31, 82, 46, -73, -61, 61, 73, -46, -82, 31, 88, -13, -90, -4, 90, 22, -85, -38, 78, 54, -67 },
//	{ 64, -64, -64, 64, 64, -64, -64, 64, 64, -64, -64, 64, 64, -64, -64, 64, 64, -64, -64, 64, 64, -64, -64, 64, 64, -64, -64, 64, 64, -64, -64, 64 },
//	{ 61, -73, -46, 82, 31, -88, -13, 90, -4, -90, 22, 85, -38, -78, 54, 67, -67, -54, 78, 38, -85, -22, 90, 4, -90, 13, 88, -31, -82, 46, 73, -61 },
//	{ 57, -80, -25, 90, -9, -87, 43, 70, -70, -43, 87, 9, -90, 25, 80, -57, -57, 80, 25, -90, 9, 87, -43, -70, 70, 43, -87, -9, 90, -25, -80, 57 },
//	{ 54, -85, -4, 88, -46, -61, 82, 13, -90, 38, 67, -78, -22, 90, -31, -73, 73, 31, -90, 22, 78, -67, -38, 90, -13, -82, 61, 46, -88, 4, 85, -54 },
//	{ 50, -89, 18, 75, -75, -18, 89, -50, -50, 89, -18, -75, 75, 18, -89, 50, 50, -89, 18, 75, -75, -18, 89, -50, -50, 89, -18, -75, 75, 18, -89, 50 },
//	{ 46, -90, 38, 54, -90, 31, 61, -88, 22, 67, -85, 13, 73, -82, 4, 78, -78, -4, 82, -73, -13, 85, -67, -22, 88, -61, -31, 90, -54, -38, 90, -46 },
//	{ 43, -90, 57, 25, -87, 70, 9, -80, 80, -9, -70, 87, -25, -57, 90, -43, -43, 90, -57, -25, 87, -70, -9, 80, -80, 9, 70, -87, 25, 57, -90, 43 },
//	{ 38, -88, 73, -4, -67, 90, -46, -31, 85, -78, 13, 61, -90, 54, 22, -82, 82, -22, -54, 90, -61, -13, 78, -85, 31, 46, -90, 67, 4, -73, 88, -38 },
//	{ 36, -83, 83, -36, -36, 83, -83, 36, 36, -83, 83, -36, -36, 83, -83, 36, 36, -83, 83, -36, -36, 83, -83, 36, 36, -83, 83, -36, -36, 83, -83, 36 },
//	{ 31, -78, 90, -61, 4, 54, -88, 82, -38, -22, 73, -90, 67, -13, -46, 85, -85, 46, 13, -67, 90, -73, 22, 38, -82, 88, -54, -4, 61, -90, 78, -31 },
//	{ 25, -70, 90, -80, 43, 9, -57, 87, -87, 57, -9, -43, 80, -90, 70, -25, -25, 70, -90, 80, -43, -9, 57, -87, 87, -57, 9, 43, -80, 90, -70, 25 },
//	{ 22, -61, 85, -90, 73, -38, -4, 46, -78, 90, -82, 54, -13, -31, 67, -88, 88, -67, 31, 13, -54, 82, -90, 78, -46, 4, 38, -73, 90, -85, 61, -22 },
//	{ 18, -50, 75, -89, 89, -75, 50, -18, -18, 50, -75, 89, -89, 75, -50, 18, 18, -50, 75, -89, 89, -75, 50, -18, -18, 50, -75, 89, -89, 75, -50, 18 },
//	{ 13, -38, 61, -78, 88, -90, 85, -73, 54, -31, 4, 22, -46, 67, -82, 90, -90, 82, -67, 46, -22, -4, 31, -54, 73, -85, 90, -88, 78, -61, 38, -13 },
//	{ 9, -25, 43, -57, 70, -80, 87, -90, 90, -87, 80, -70, 57, -43, 25, -9, -9, 25, -43, 57, -70, 80, -87, 90, -90, 87, -80, 70, -57, 43, -25, 9 },
//	{ 4, -13, 22, -31, 38, -46, 54, -61, 67, -73, 78, -82, 85, -88, 90, -90, 90, -90, 88, -85, 82, -78, 73, -67, 61, -54, 46, -38, 31, -22, 13, -4 }
//};
//
///**
//*	\var Short[][] TCOEFF_DST_4
//*	\brief Macierz transformaty DST 4x4
//*/
//const Short Transform::TCOEFF_DST_4[ 4 ][ 4 ] =
//{
//	{ 29, 55, 74, 84 },
//	{ 74, 74, 0, -74 },
//	{ 84, -29, -74, 55 },
//	{ 55, -84, 74, -29 },
//};
//
//const Short Transform::SHIFT_INV_1 = 7;
//const Short Transform::SHIFT_INV_2 = 12;
