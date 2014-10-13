/** \file     Main.cpp
\brief    Test funkcji transformacji i kwantyzacji
*/

#ifndef _MAIN_
#define _MAIN_

#include "Quant.h"
#include "Transform.h"
#include "TypeDef.h"
#include "Utils.h"
#include "Picture.h"
#include "Binarization.h"
#include "PartitioningStrategy.h"
#include "PU.h"
#include "IntraPred.h"
#include "CU.h"
#include "DeblockingFilter.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <iomanip> 

void binarizationTest( std::string pathIn, std::string pathOut )
{
	bool keepGoing = false;
	bool blad = false;
	bool dst = false;
	Int count = 0;
	Int scanIdx = 0, total = 0;
	std::string s = "";
	Coeff** coeffs = nullptr;
	UInt width = 0, height = 0, nonzeroCoeffs = 0;
	Bool transQuantBypass = false, sdh = true, useTransformSkip = false;
	std::ifstream ft( pathIn );
	std::ofstream out( pathOut );

	do
	{
		deleteMatrix( coeffs );
		blad = false;
		dst = false;
		transQuantBypass = false;
		sdh = true;
		useTransformSkip = false;

		keepGoing = goToMarker( ft, "TU" );
		if( !keepGoing ) break;
		keepGoing = readMatrix( ft, "COEFFS", coeffs, width, height );
		if( !keepGoing ) break;
		keepGoing = readValue( ft, "SCAN_IDX", scanIdx );
		if( !keepGoing ) break;
		keepGoing = readValue( ft, "COMP", s );
		if( !keepGoing ) break;
		ImgComp comp = s == "LUMA" ? LUMA : CR;
		keepGoing = readValue( ft, "SDH_FLAG", sdh );
		if( !keepGoing ) break;
		keepGoing = readValue( ft, "NONZERO_COEFFS", nonzeroCoeffs );
		if( !keepGoing ) break;

		if( nonzeroCoeffs > 0 )
		{
			keepGoing = readValue( ft, "USE_TRANSFORM_SKIP", useTransformSkip );
			if( !keepGoing ) break;
			if( useTransformSkip )
			{
				keepGoing = readValue( ft, "TRANSQUANT_BYPASS", transQuantBypass );
				if( !keepGoing ) break;
			}
		}

		keepGoing = readValue( ft, "TOTAL", total );
		if( !keepGoing ) break;

		ScanningMode* scanStrategy = nullptr;
		switch( scanIdx )
		{
		case 1:
			scanStrategy = new HorizontalScanningMode( );
			break;
		case 2:
			scanStrategy = new VerticalScanningMode( );
			break;
		default:
			scanStrategy = new DiagonalScanningMode( );
			break;
		}

		Binarization::getInstance( )->setCurrentCUUsesTransQuantBypass( transQuantBypass );
		Binarization::getInstance( )->setCurrentTUUsesTransformSkip( transQuantBypass );
		PicParams( )->setTransformSkipEnabled( useTransformSkip );
		PicParams( )->setSDHEnabled( sdh );
		UInt numBins = Binarization::getInstance( )->countBinsInTB( coeffs, width, scanStrategy );
		delete scanStrategy;

		out << "#########################" << std::endl;
		out << "COEFFS" << std::endl;
		printMatrix( coeffs, width, out );
		out << "MODEL BINS" << std::endl << total << std::endl;
		out << "CALCULATED BINS" << std::endl << numBins << std::endl;
		if( total != numBins )
		{
			out << "ERROR!!!" << std::endl;
			blad = false;
		}

		std::cout << count << std::endl;
		++count;
	}
	while( keepGoing );

	deleteMatrix( coeffs );
}

Void transformTest( std::string pathIn, std::string pathOut )
{
	bool keepGoing = false;
	bool blad = false;
	bool dst = false;
	Int count = 0;

	Sample** block = nullptr;
	Coeff** res = nullptr;
	UInt width = 0, height = 0;
	Coeff** hor = nullptr;
	Coeff** ver = nullptr;

	std::ifstream ft( pathIn );
	std::ofstream out( pathOut );

	do
	{
		deleteMatrix( block );
		deleteMatrix( res );
		deleteMatrix( hor );
		deleteMatrix( ver );
		blad = false;
		dst = false;

		keepGoing = readMatrix( ft, "BLOCK", block, width, height );
		if( !keepGoing ) break;
		if( width == 4 )
		{
			std::string mode = "";
			if( !readValue( ft, "MODE", mode ) )
				break;
			dst = mode == "DCT" ? false : true;
		}
		keepGoing = readMatrix( ft, "HORIZONTAL", hor, width, height );
		if( !keepGoing ) break;
		keepGoing = readMatrix( ft, "VERTICAL", ver, width, height );
		if( !keepGoing ) break;

		res = getEmptyMatrix<Coeff>( width, height );

		Transform::getInstance( )->forwardTrans( block, res, width, 8, dst, dst );

		out << "#########################" << std::endl;
		out << "BLOCK" << std::endl;
		printMatrix( block, width, out );
		out << "HORIZONTAL" << std::endl;
		printMatrix( Transform::getInstance( )->afterHor, width, out );
		out << "DIFFERENCE" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < width; ++j )
			{
				out << Transform::getInstance( )->afterHor[ j ][ i ] - hor[ j ][ i ] << " ";
				if( Transform::getInstance( )->afterHor[ j ][ i ] != hor[ j ][ i ] )
					blad = true;
			}
			out << std::endl;
		}
		if( blad )
		{
			out << "ERROR!!!" << std::endl;
			blad = false;
		}
		out << "VERTICAL" << std::endl;
		printMatrix( res, width, out );
		out << "DIFFERENCE" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < width; ++j )
			{
				out << ver[ j ][ i ] - res[ j ][ i ] << " ";
				if( ver[ j ][ i ] != res[ j ][ i ] )
					blad = true;
			}
			out << std::endl;
		}
		if( blad )
		{
			out << "ERROR!!!" << std::endl;
			blad = false;
		}

		std::cout << count << std::endl;
		++count;
	}
	while( keepGoing );

	deleteMatrix( block );
	deleteMatrix( res );
	deleteMatrix( hor );
	deleteMatrix( ver );
}

Void transformTest2( std::string pathOut )
{
	srand( ( (UInt)time( NULL ) ) );

	bool dst = false;
	Int count = 0;
	Int qp = 0;
	Sample** block = nullptr;
	Sample** fhor = nullptr;
	Coeff** ver = nullptr;
	Coeff** afterquant = nullptr;
	Coeff** dequant = nullptr;
	Coeff** ihor = nullptr;
	Sample** res = nullptr;
	Int sizes[] = { 8, 8, 8, 16, 32 };
	std::ofstream tr( pathOut + "_transform.txt" );
	std::ofstream q( pathOut + "_quant.txt" );

	do
	{
		deleteMatrix( block );
		deleteMatrix( res );
		deleteMatrix( fhor );
		deleteMatrix( ver );
		deleteMatrix( afterquant );
		deleteMatrix( dequant );
		deleteMatrix( ihor );

		Int mode = rand( ) % 5;
		Int size = sizes[ mode ];

		block = getEmptyMatrix<Sample>( size, size );
		fhor = getEmptyMatrix<Sample>( size, size );
		ver = getEmptyMatrix<Coeff>( size, size );
		afterquant = getEmptyMatrix<Coeff>( size, size );
		dequant = getEmptyMatrix<Coeff>( size, size );
		ihor = getEmptyMatrix<Coeff>( size, size );
		res = getEmptyMatrix<Sample>( size, size );

		Sample** subBlock = getEmptyMatrix<Sample>( 4, 4 );
		Coeff** subVer = getEmptyMatrix<Coeff>( 4, 4 );
		Coeff** subQuant = getEmptyMatrix<Coeff>( 4, 4 );
		Coeff** subDequant = getEmptyMatrix<Coeff>( 4, 4 );
		Sample** subRes = getEmptyMatrix<Sample>( 4, 4 );

		dst = ( mode == 0 );

		switch( mode )
		{
		case 0: //dst 4x4
		case 1: //dct 4x4
			qp = rand( ) % 52;
			for( int i = 0; i < 4; ++i )
			{
				for( UInt j = 0; j < 4; ++j )
				{
					for( UInt k = 0; k < 4; ++k )
					{
						subBlock[ j ][ k ] = rand( ) % 512 - 256;
						block[ 4 * ( i % 2 ) + j ][ 4 * ( i / 2 ) + k ] = subBlock[ j ][ k ];
					}
				}
				Transform::getInstance( )->forwardTrans( subBlock, subVer, 4, 8, dst, dst );
				for( UInt j = 0; j < 4; ++j )
				{
					for( UInt k = 0; k < 4; ++k )
					{
						fhor[ 4 * ( i % 2 ) + j ][ 4 * ( i / 2 ) + k ] = Transform::getInstance( )->afterHor[ j ][ k ];
						ver[ 4 * ( i % 2 ) + j ][ 4 * ( i / 2 ) + k ] = subVer[ j ][ k ];
					}
				}
				Quant::getInstance( )->quant( subVer, subQuant, qp, 8, 4 );
				Quant::getInstance( )->deQuant( subQuant, subDequant, qp, 8, 4 );
				for( UInt j = 0; j < 4; ++j )
				{
					for( UInt k = 0; k < 4; ++k )
					{
						afterquant[ 4 * ( i % 2 ) + j ][ 4 * ( i / 2 ) + k ] = subQuant[ j ][ k ];
						dequant[ 4 * ( i % 2 ) + j ][ 4 * ( i / 2 ) + k ] = subDequant[ j ][ k ];
					}
				}
				Transform::getInstance( )->inverseTrans( subDequant, subRes, 4, 8, dst, dst );
				for( UInt j = 0; j < 4; ++j )
				{
					for( UInt k = 0; k < 4; ++k )
					{
						ihor[ 4 * ( i % 2 ) + j ][ 4 * ( i / 2 ) + k ] = Transform::getInstance( )->afterHor[ j ][ k ];
						res[ 4 * ( i % 2 ) + j ][ 4 * ( i / 2 ) + k ] = subRes[ j ][ k ];
					}
				}
			}
			break;

		case 2:
		case 3:
		case 4:
			for( Int j = 0; j < size; ++j )
			{
				for( Int k = 0; k < size; ++k )
				{
					block[ j ][ k ] = rand( ) % 512 - 256;
				}
			}
			Transform::getInstance( )->forwardTrans( block, ver, size, 8, dst, dst );
			for( Int j = 0; j < size; ++j )
			{
				for( Int k = 0; k < size; ++k )
				{
					fhor[ j ][ k ] = Transform::getInstance( )->afterHor[ j ][ k ];
				}
			}
			qp = rand( ) % 52;
			Quant::getInstance( )->quant( ver, afterquant, qp, 8, size );
			Quant::getInstance( )->deQuant( afterquant, dequant, qp, 8, size );

			Transform::getInstance( )->inverseTrans( dequant, res, size, 8, dst, dst );
			for( Int j = 0; j < size; ++j )
			{
				for( Int k = 0; k < size; ++k )
				{
					ihor[ j ][ k ] = Transform::getInstance( )->afterHor[ j ][ k ];
				}
			}
			break;
		}
		tr << "*** residuals " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << ( mode < 2 ? ( dst ? " DST" : " DCT" ) : "" ) << " ***" << std::endl;
		printMatrix( block, size, tr );
		tr << "*** horizontal transform " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << ( mode < 2 ? ( dst ? " DST" : " DCT" ) : "" ) << " ***" << std::endl;
		printMatrix( fhor, size, tr );
		tr << "*** vertical transform " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << ( mode < 2 ? ( dst ? " DST" : " DCT" ) : "" ) << " ***" << std::endl;
		printMatrix( ver, size, tr );
		tr << "*** coefficients " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << ( mode < 2 ? ( dst ? " DST" : " DCT" ) : "" ) << " ***" << std::endl;
		printMatrix( dequant, size, tr );
		tr << "*** vertical inverse transform " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << ( mode < 2 ? ( dst ? " DST" : " DCT" ) : "" ) << " ***" << std::endl;
		printMatrix( ihor, size, tr );
		tr << "*** horizontal inverse transform " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << ( mode < 2 ? ( dst ? " DST" : " DCT" ) : "" ) << " ***" << std::endl;
		printMatrix( res, size, tr );

		q << "*** coefficients " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << " QP=" << qp << " ***" << std::endl;
		printMatrix( ver, size, q );
		q << "*** quantization " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << " QP=" << qp << " ***" << std::endl;
		printMatrix( afterquant, size, q );
		q << "*** coefficients " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << " QP=" << qp << " ***" << std::endl;
		printMatrix( afterquant, size, q );
		q << "*** dequantization " << ( mode < 2 ? 4 : size ) << "x" << ( mode < 2 ? 4 : size ) << " QP=" << qp << " ***" << std::endl;
		printMatrix( dequant, size, q );

		std::cout << count << std::endl;
		++count;
	}
	while( count < 10000 );

	deleteMatrix( block );
	deleteMatrix( res );
	deleteMatrix( fhor );
	deleteMatrix( ver );
	deleteMatrix( afterquant );
	deleteMatrix( dequant );
	deleteMatrix( ihor );
}

Void invTransTest( std::string pathIn, std::string pathOut )
{
	bool keepGoing = false;
	bool blad = false;
	bool dst = false;
	Int count = 0;

	Coeff** coeff = nullptr;
	Sample** res = nullptr;
	UInt width = 0, height = 0;
	Coeff** hor = nullptr;
	Coeff** ver = nullptr;

	std::ifstream ft( pathIn );
	std::ofstream out( pathOut );

	do
	{
		deleteMatrix( coeff );
		deleteMatrix( res );
		deleteMatrix( hor );
		deleteMatrix( ver );
		blad = false;
		dst = false;

		keepGoing = readMatrix( ft, "COEFF", coeff, width, height );
		if( !keepGoing ) break;
		if( width == 4 )
		{
			std::string mode = "";
			if( !readValue( ft, "MODE", mode ) )
				break;
			dst = mode == "DCT" ? false : true;
		}
		keepGoing = readMatrix( ft, "HORIZONTAL", hor, width, height );
		if( !keepGoing ) break;
		keepGoing = readMatrix( ft, "VERTICAL", ver, width, height );
		if( !keepGoing ) break;

		res = getEmptyMatrix<Sample>( width, height );

		Transform::getInstance( )->inverseTrans( coeff, res, width, 8, dst, dst );
		
		out << "#########################" << std::endl;
		out << "COEFF" << std::endl;
		printMatrix( coeff, width, out );
		out << "HORIZONTAL" << std::endl;
		printMatrix( Transform::getInstance( )->afterHor, width, out );
		out << "DIFFERENCE" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < width; ++j )
			{
				out << Transform::getInstance( )->afterHor[ j ][ i ] - hor[ j ][ i ] << " ";
				if( Transform::getInstance( )->afterHor[ j ][ i ] != hor[ j ][ i ] )
					blad = true;
			}
			out << std::endl;
		}
		if( blad )
		{
			out << "ERROR!!!" << std::endl;
			blad = false;
		}
		out << "VERTICAL" << std::endl;
		printMatrix( res, width, out );
		out << "DIFFERENCE" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < width; ++j )
			{
				out << ver[ j ][ i ] - res[ j ][ i ] << " ";
				if( ver[ j ][ i ] != res[ j ][ i ] )
					blad = true;
			}
			out << std::endl;
		}
		if( blad )
		{
			out << "ERROR!!!" << std::endl;
			blad = false;
		}

		std::cout << count << std::endl;
		++count;
	}
	while( keepGoing );

	deleteMatrix( coeff );
	deleteMatrix( res );
	deleteMatrix( hor );
	deleteMatrix( ver );
}


Void quantTest( std::string pathIn, std::string pathOut )
{
	bool keepGoing = false;
	bool blad = false;
	Int count = 0;

	Coeff** coeff = nullptr;
	Coeff** res = nullptr;
	Coeff** modelRes = nullptr;
	Short qp = 0;
	UInt width = 0, height = 0;

	std::ifstream ft( pathIn );
	std::ofstream out( pathOut );

	do
	{
		deleteMatrix( coeff );
		deleteMatrix( res );
		deleteMatrix( modelRes );
		blad = false;

		keepGoing = goToMarker( ft, "NEW_SET" );
		if( !keepGoing ) break;
		keepGoing = readValue( ft, "QP", qp );
		if( !keepGoing ) break;
		keepGoing = readMatrix( ft, "COEFF", coeff, width, height );
		if( !keepGoing ) break;
		keepGoing = readMatrix( ft, "QUANT_COEFF", modelRes, width, height );
		if( !keepGoing ) break;

		res = getEmptyMatrix<Coeff>( width, height );

		out << "#########################" << std::endl;
		out << "QP = " << qp << std::endl;
		out << "COEFF" << std::endl;
		printMatrix( coeff, width, out );

		Quant::getInstance( )->quant( coeff, res, qp, 8, width );

		out << "QUANT_COEFF" << std::endl;
		printMatrix( res, width, out );
		out << "DIFFERENCE" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < width; ++j )
			{
				out << res[ j ][ i ] - modelRes[ j ][ i ] << " ";
				if( res[ j ][ i ] - modelRes[ j ][ i ] )
					blad = true;
			}
			out << std::endl;
		}
		if( blad )
		{
			out << "ERROR!!!" << std::endl;
			blad = false;
		}

		std::cout << count << std::endl;
		++count;
	}
	while( keepGoing );

	deleteMatrix( coeff );
	deleteMatrix( res );
	deleteMatrix( modelRes );
}

Void deQuantTest( std::string pathIn, std::string pathOut )
{
	bool keepGoing = false;
	bool blad = false;
	Int count = 0;

	Coeff** coeff = nullptr;
	Coeff** res = nullptr;
	Coeff** modelRes = nullptr;
	Short qp = 0;
	UInt width = 0, height = 0;

	std::ifstream ft( pathIn );
	std::ofstream out( pathOut );

	do
	{
		deleteMatrix( coeff );
		deleteMatrix( res );
		deleteMatrix( modelRes );
		blad = false;

		keepGoing = readValue( ft, "QP", qp );
		if( !keepGoing ) break;
		keepGoing = readMatrix( ft, "COEFF", coeff, width, height );
		if( !keepGoing ) break;

		keepGoing = readMatrix( ft, "DEQUANT_COEFF", modelRes, width, height );
		if( !keepGoing ) break;

		res = getEmptyMatrix<Coeff>( width, height );

		Quant::getInstance( )->deQuant( coeff, res, qp, 8, width );

		out << "#########################" << std::endl;
		out << "QP = " << qp << std::endl;
		out << "COEFF" << std::endl;
		printMatrix( coeff, width, out );
		out << "QUANT_COEFF" << std::endl;
		printMatrix( res, width, out );
		out << "DIFFERENCE" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < width; ++j )
			{
				out << res[ j ][ i ] - modelRes[ j ][ i ] << " ";
				if( res[ j ][ i ] - modelRes[ j ][ i ] )
					blad = true;
			}
			out << std::endl;
		}
		if( blad )
		{
			out << "ERROR!!!" << std::endl;
			blad = false;
		}

		std::cout << count << std::endl;
		++count;
	}
	while( keepGoing );

	deleteMatrix( coeff );
	deleteMatrix( res );
	deleteMatrix( modelRes );
}

Void predicitonTest( std::string pathIn, std::string pathOut )
{
	bool keepGoing = false;
	bool blad = false;
	Int count = 0;

	Sample** res = nullptr;
	Sample** res2 = nullptr;
	Sample* left = nullptr;
	Sample* above = nullptr;
	Sample corner = 0;
	Short qp = 0;
	UInt width = 0, height = 0;
	UInt mode = 0;
	std::string s;

	std::ifstream ft( pathIn );
	std::ofstream out( pathOut );

	do
	{
		delete[]left;
		left = nullptr;
		delete[] above;
		above = nullptr;
		deleteMatrix( res );
		deleteMatrix( res2 );
		blad = false;

		goToMarker( ft, "NEW_PREDICTION" );
		keepGoing = readValue( ft, "WIDTH", width );
		if( !keepGoing ) break;
		keepGoing = readValue( ft, "HEIGHT", height );
		if( !keepGoing ) break;
		keepGoing = readValue( ft, "MODE", mode );
		if( !keepGoing ) break;
		keepGoing = readValue( ft, "COMP", s );
		if( !keepGoing ) break;

		left = new Sample[ 2 * width ];
		above = new Sample[ 2 * width ];
		memset( left, 0, ( 2 * width )*sizeof( Sample ) );
		memset( above, 0, ( 2 * width )*sizeof( Sample ) );

		keepGoing = goToMarker( ft, "PROBKI_BEZ_FILTRU" );
		if( !keepGoing ) break;
		Bool badRead = false;
		try
		{
			ft >> corner;
			for( UInt i = 0; i < 2 * width; ++i )
			{
				ft >> above[ i ];
			}
			for( UInt i = 0; i < 2 * width; ++i )
			{
				ft >> left[ i ];
			}
		}
		catch( ... )
		{
			keepGoing = false;
			break;
		}

		keepGoing = readMatrix( ft, "PREDYKCJA", res, width, height );
		if( !keepGoing ) break;

		out << "###########################" << std::endl;
		out << "width: " << width << std::endl;
		out << "height: " << height << std::endl;
		out << "mode: " << mode << std::endl;
		out << "references: " << std::endl << corner << " ";
		for( UInt i = 0; i < 2 * width; ++i )
		{
			out << above[ i ] << " ";
			if( above[ i ] < 0 || above[ i ] > 255 )
			{
				badRead = true;
				break;
			}
		}
		out << std::endl;
		for( UInt i = 0; i < 2 * width; ++i )
		{
			out << left[ i ] << std::endl;
			if( left[ i ] < 0 || left[ i ] > 255 )
			{
				badRead = true;
				break;
			}
		}
		if( badRead )
		{
			out << "zly odczyt, pomijam" << std::endl << " ";
			continue;
		}
		PBIntra* pb = new PBIntra( nullptr, s == "LUMA" ? LUMA : CB, 0, 0, width );
		pb->setModeIdx( mode );
		res2 = IntraPred::getInstance( )->calcPredForceRefs( pb, left, above, corner );

		out << "wynik modelowy: " << std::endl;
		printMatrix( res, width, out );

		out << "wynik: " << std::endl;
		printMatrix( res2, width, out );
		out << "DIFFERENCE" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < width; ++j )
			{
				out << res[ j ][ i ] - res2[ j ][ i ] << " ";
				if( res[ j ][ i ] - res2[ j ][ i ] )
					blad = true;
			}
			out << std::endl;
		}
		if( blad )
		{
			out << "ERROR!!!" << std::endl;
			blad = false;
		}

		std::cout << count << std::endl;
		++count;
		delete pb;
	}
	while( keepGoing );

	delete[]left;
	delete[] above;
	deleteMatrix( res );
	deleteMatrix( res2 );
}

Void fdbTest( std::string pathIn, std::string pathOut )
{
	Sample** luma1 = nullptr;
	Sample** cb1 = nullptr;
	Sample** cr1 = nullptr;
	Sample** luma2 = nullptr;
	Sample** cb2 = nullptr;
	Sample** cr2 = nullptr;
	Sample** luma3 = nullptr;
	Sample** cb3 = nullptr;
	Sample** cr3 = nullptr;

	bool keepGoing = false;
	bool blad = false;
	Int count = 0;

	std::string s;

	std::ifstream* ft = new std::ifstream( pathIn );
	std::ofstream* out = new std::ofstream( pathOut );

	Short qp = 0;
	UInt width = 0, height = 0, widthC = 0, heightC = 0;
	do
	{
		deleteMatrix( luma1 );
		deleteMatrix( cb1 );
		deleteMatrix( cr1 );
		deleteMatrix( luma2 );
		deleteMatrix( cb2 );
		deleteMatrix( cr2 );
		deleteMatrix( luma3 );
		deleteMatrix( cb3 );
		deleteMatrix( cr3 );
		blad = false;

		keepGoing = goToMarker( *ft, "NEW_FDB" );
		if( !keepGoing ) break;
		keepGoing = readValue( *ft, "QP", qp );
		if( !keepGoing ) break;
		keepGoing = readMatrix( *ft, "LUMA_BEFORE", luma1, width, height );
		if( !keepGoing ) break;
		SeqParams( )->setPicWidth( width );
		SeqParams( )->setPicHeight( height );
		PicParams( )->setQP( 35 );
		keepGoing = readMatrix( *ft, "CR_BEFORE", cr1, widthC, heightC );
		if( !keepGoing ) break;
		keepGoing = readMatrix( *ft, "CB_BEFORE", cb1, widthC, heightC );
		if( !keepGoing ) break;
		std::shared_ptr<Picture> pic = std::make_shared<Picture>( );
		pic->initFrame( );
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < height; ++j )
			{
				( pic->getReconMatrix( LUMA ) )[ i ][ j ] = luma1[ i ][ j ];
			}
		}
		for( UInt i = 0; i < widthC; ++i )
		{
			for( UInt j = 0; j < heightC; ++j )
			{
				( pic->getReconMatrix( CB ) )[ i ][ j ] = cb1[ i ][ j ];
				( pic->getReconMatrix( CR ) )[ i ][ j ] = cr1[ i ][ j ];
			}
		}

		pic->initPartitions( new FDBTestPartitions( ft ) );
		pic->printDescription( );

		keepGoing = readMatrix( *ft, "LUMA_RES", luma3, width, height );
		if( !keepGoing ) break;
		keepGoing = readMatrix( *ft, "CR_RES", cr3, widthC, heightC );
		if( !keepGoing ) break;
		keepGoing = readMatrix( *ft, "CB_RES", cb3, widthC, heightC );
		if( !keepGoing ) break;

		*out << "luma1" << std::endl;
		printMatrix( luma1, width, *out );
		*out << std::endl;
		*out << "cr1" << std::endl;
		printMatrix( cr1, widthC, *out );
		*out << std::endl;
		*out << "cb1" << std::endl;
		printMatrix( cb1, widthC, *out );
		*out << std::endl;

		DeblockingFilter::getInstance( )->doFiltering( pic );

		*out << "luma result" << std::endl;
		printMatrix( pic->getReconMatrix( LUMA ), width, *out );
		*out << "cr result" << std::endl;
		printMatrix( pic->getReconMatrix( CR ), widthC, *out );
		*out << "cb result" << std::endl;
		printMatrix( pic->getReconMatrix( CB ), widthC, *out );

		*out << std::endl;
		*out << "luma difference" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < height; ++j )
			{
				*out << pic->getReconMatrix( LUMA )[ j ][ i ] - luma1[ j ][ i ] << " ";
			}
			*out << std::endl;
		}
		*out << std::endl;
		*out << "luma error" << std::endl;
		for( UInt i = 0; i < width; ++i )
		{
			for( UInt j = 0; j < height; ++j )
			{
				*out << pic->getReconMatrix( LUMA )[ j ][ i ] - luma3[ j ][ i ] << " ";
			}
			*out << std::endl;
		}
		*out << std::endl;
		*out << "cr difference" << std::endl;
		for( UInt i = 0; i < widthC; ++i )
		{
			for( UInt j = 0; j < heightC; ++j )
			{
				*out << std::setw( 2 ) << pic->getReconMatrix( CR )[ j ][ i ] - cr1[ j ][ i ] << " ";
			}
			*out << std::endl;
		}
		*out << std::endl;
		*out << "cr error" << std::endl;
		for( UInt i = 0; i < widthC; ++i )
		{
			for( UInt j = 0; j < heightC; ++j )
			{
				*out << std::setw( 2 ) << pic->getReconMatrix( CR )[ j ][ i ] - cr3[ j ][ i ] << " ";
			}
			*out << std::endl;
		}
		*out << std::endl;
		*out << "cb difference" << std::endl;
		for( UInt i = 0; i < widthC; ++i )
		{
			for( UInt j = 0; j < heightC; ++j )
			{
				*out << std::setw( 2 ) << pic->getReconMatrix( CB )[ j ][ i ] - cb1[ j ][ i ] << " ";
			}
			*out << std::endl;
		}
		*out << std::endl;
		*out << "cb error" << std::endl;
		for( UInt i = 0; i < widthC; ++i )
		{
			for( UInt j = 0; j < heightC; ++j )
			{
				*out << std::setw( 2 ) << pic->getReconMatrix( CB )[ j ][ i ] - cb3[ j ][ i ] << " ";
			}
			*out << std::endl;
		}

		std::cout << count << std::endl;
		++count;
	}
	while( keepGoing );

	ft->close( );
	out->close( );
	delete ft;
	delete out;
	deleteMatrix( luma1 );
	deleteMatrix( cb1 );
	deleteMatrix( cr1 );
	deleteMatrix( luma2 );
	deleteMatrix( cb2 );
	deleteMatrix( cr2 );
	deleteMatrix( luma3 );
	deleteMatrix( cb3 );
	deleteMatrix( cr3 );
}
/**
*	Funkcja glowna, dokonuje testow na macierzach z podanego pliku
*/
Int main( Int argc, Char* argv[] )
{
	transformTest2( "D:\\txt\\test" );

	/*if( argc < 5 )
	{
	std::cout << "Uzycie: program tryb sciezka_konfiguracji_logowania sciezka_wejscia sciezka_wyjscia" << std::endl;
	std::cout << "tryb 1 - test transformacji 1" << std::endl;
	std::cout << "tryb 2 - test transformacji odwrotnej" << std::endl;
	std::cout << "tryb 3 - test kwantyzacji" << std::endl;
	std::cout << "tryb 4 - test dekwantyzacji" << std::endl;
	std::cout << "tryb 5 - test predykcji" << std::endl;
	std::cout << "tryb 6 - test binaryzacji" << std::endl;
	std::cout << "tryb 7 - test filtru deblokujacego" << std::endl;
	return EXIT_SUCCESS;
	}

	LoggingControl::mainSettingsPath = argv[ 2 ];
	std::string in( argv[ 3 ] );
	std::string out( argv[ 4 ] );

	switch( argv[ 1 ][ 0 ] )
	{
	case '1':
	transformTest( in, out );
	break;
	case '2':
	invTransTest( in, out );
	break;
	case '3':
	quantTest( in, out );
	break;
	case '4':
	deQuantTest( in, out );
	break;
	case '5':
	predicitonTest( in, out );
	break;
	case '6':
	binarizationTest( in, out );
	break;
	case '7':
	fdbTest( in, out );
	break;
	default:
	std::cout << "nieprawidlowy tryb" << std::endl;
	break;
	}
	system( "PAUSE" );

	return EXIT_SUCCESS;*/
}

#endif