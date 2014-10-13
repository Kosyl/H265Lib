/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.  
 *
 * Copyright (c) 2010-2013, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     Main.cpp
    \brief    Test funkcji transformacji
*/

#ifndef _MAIN_
#define _MAIN_

#include "TComTrQuant.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

#include "TComRom.h"

using namespace std;

/**
*	Funkcja testujaca transformaty
*	Funkcja liczy rozne transformaty dla podanego bloku: mnozenie macierzy z modelu, tr. motylkowe z modelu, mnozenie macierzy dla tablic 2D, DST...
*	\param tab jednowymiarowa tablica probek do transformacji
*	\param bok wymiar bloku transformaty (bok x bok); mowi, kiedy w liscie tab zaczyna sie nowy "wiersz"
*/
void test(short* tab, Int bok)
{
	cout << "========================================";
	
	Int size = bok*bok;

	//tablica na wynik 1D
	Int* res = (Int*)malloc(size*sizeof(Int));

	//tablica na wynik tr odwrotnej 1D
	short* invres= (short*)malloc(size*sizeof(short));


	/*transformata oryginalna nietykana*/
	TComTrQuant* tc = new TComTrQuant();
	tc->my_xTWrapper(8,0,tab,bok,res,bok,bok,false);
	tc->my_xITWrapper(8,0,res,invres,bok,bok,bok,false);
	cout << "\nTransformata oryginalna prosta przez mnozenie:\n";
	for(int i=0;i<bok;i++) 
	{
		for(int j=0;j<bok;j++)
		{
			printf("%3d ",res[i*bok+j]);
		}
		cout << "\n";
	}
	cout << "\nTransformata oryginalna odwrotna przez mnozenie:\n";
	for(int i=0;i<bok;i++) 
	{
		for(int j=0;j<bok;j++)
		{
			printf("%3d ",invres[i*bok+j]);
		}
		cout << "\n";
	}
}
int main(int argc, char* argv[])
{
	initROM();
	/*short tab4x4[16] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	short tab4x4[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	short tab4x4[16] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};
	short tab4x4[16] = {-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1};*/
	
	Int bok = 4;
	Int size = bok*bok;

	short* tab = (short*)malloc(size*sizeof(short));
	srand(time(NULL));
	for(int i=0;i<size;i++)tab[i]=1;//rand()%256;
	
	test(tab,bok);

	system("PAUSE");
	return EXIT_SUCCESS;
}

#endif