/** \file     Main.cpp
\brief    Test funkcji transformacji i kwantyzacji
*/

#ifndef _MAIN_
#define _MAIN_

#include "TypeDef.h"
#include "Utils.h"
#include "Binarization.h"
#include "SeqPicParameters.h"

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

		if (width != 8)
			continue;

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

		//bylo wczytywane z pliku, ustawiam na sztywno najprosztszy przypadek
		/*Binarization::getInstance( )->setCurrentCUUsesTransQuantBypass( transQuantBypass );
		Binarization::getInstance( )->setCurrentTUUsesTransformSkip( transQuantBypass );
		PicParams( )->setTransformSkipEnabled( useTransformSkip );
		PicParams( )->setSDHEnabled( sdh );*/


		Binarization::getInstance()->setCurrentCUUsesTransQuantBypass(false);
		Binarization::getInstance()->setCurrentTUUsesTransformSkip(false);
		PicParams()->setTransformSkipEnabled(false);
		PicParams()->setSDHEnabled(false);


		UInt numBins = Binarization::getInstance( )->countBinsInTB( coeffs, width, scanStrategy );
		delete scanStrategy;

		out << "#########################" << s << " " << (scanIdx == 1 ? "horizontal" : scanIdx == 2 ? "vertical" : "diagonal") << std::endl;
		out << "wspolczynniki" << std::endl;
		for (UInt i = 0; i < 8; ++i)
		{
			for (UInt j = 0; j < 8; ++j)
			{
				out << coeffs[j][i] << std::endl;
			}
		}
		out << "wynik" << std::endl << numBins << std::endl;

		std::cout << count << std::endl;
		++count;
	}
	while( keepGoing );

	deleteMatrix( coeffs );
}

Int main( Int argc, Char* argv[] )
{
	binarizationTest("D:\\txt\\ec_summary.txt","D:\\txt\\bin - res.txt");
}

#endif