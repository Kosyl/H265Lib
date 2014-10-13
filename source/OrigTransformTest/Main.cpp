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

#ifndef _TMAIN_
#define _TMAIN_

#include "TComTrQuant.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "TComRom.h"

using namespace std;

/**
*	Funkcja testujaca transformaty
*	Funkcja liczy rozne transformaty dla podanego bloku: mnozenie macierzy z modelu, tr. motylkowe z modelu, mnozenie macierzy dla tablic 2D, DST...
*	\param tab jednowymiarowa tablica probek do transformacji
*	\param bok wymiar bloku transformaty (bok x bok); mowi, kiedy w liscie tab zaczyna sie nowy "wiersz"
*/
void test(short* tab, Int bok, ofstream& out)
{
	out << "========================================";
	
	Int size = bok*bok;

	//tablica na wynik 1D
	Int* res = new Int[size];

	//tablica na wynik tr odwrotnej 1D
	short* invres= new short[size];


	/*transformata oryginalna nietykana*/
	TComTrQuant* tc = new TComTrQuant();
	tc->my_xTWrapper(8,REG_DCT,tab,bok,res,bok,bok,true);
	tc->my_xITWrapper(8,REG_DCT,res,invres,bok,bok,bok,false);
	out << "\nBlok:\n";
	for(int i=0;i<bok;i++) 
	{
		for(int j=0;j<bok;j++)
		{
			//printf("%6d ",tab[i*bok+j]);
			out << tab[i*bok+j] << " ";
		}
		out << "\n";
	}
	out << "\nTransformata oryginalna prosta przez mnozenie:\n";
	for(int i=0;i<bok;i++) 
	{
		for(int j=0;j<bok;j++)
		{
			//printf("%6d ",res[i*bok+j]);
			out << res[i*bok+j] << " ";
		}
		out << "\n";
	}
	out << "\nTransformata oryginalna odwrotna przez mnozenie:\n";
	for(int i=0;i<bok;i++) 
	{
		for(int j=0;j<bok;j++)
		{
			//printf("%6d ",invres[i*bok+j]);
			out << invres[i*bok+j] << " ";
		}
		out << "\n";
	}
	out << "\nRoznica:\n";
	for(int i=0;i<bok;i++) 
	{
		for(int j=0;j<bok;j++)
		{
			//printf("%6d ",invres[i*bok+j]);
			out << tab[i*bok+j]-invres[i*bok+j] << " ";
		}
		out << "\n";
	}
}
int main(int argc, char* argv[])
{
	initROM();
	
	Int bok = 4;
	Int size = bok*bok;
	int num;

	string s;
	ifstream input("D:\\macierze.txt");
	ofstream out;
	out.open("D:\\wynikorig.txt",'w');
	
	out.width(6);

	input >> bok;
	size = bok*bok;
	input >> num;
	getline(input,s);
	short* tab = new short[size];
	
	for(int j=0;j<num;j++)
	{
		for(int i=0;i<size;i++)input >> tab[i];
		getline(input,s);
		test(tab,bok,out);
	}
	input.close();
	out.close();

	system("PAUSE");
	return EXIT_SUCCESS;
}

#endif