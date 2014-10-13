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
    \brief    Test funkcji transformacji i kwantyzacji
*/

#ifndef _MAIN_
#define _MAIN_

#include "Quant.h"
#include "Transform.h"
#include "TypeDef.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void transfer(ifstream& in, ofstream& out, const Int& num)
{
	Int tmp;
	string s;
    for(Int i=0;i<num;++i)
	{
		in >> tmp;
		out << tmp << endl;
	}
}

Int main(Int argc, Char* argv[])
{	
	Int bok = 4; // rozmiar bloku
	Int size = bok*bok;

	string s;

	string macierze_base = "D:\\txt\\macierze4x4";
	string transform_base = "D:\\txt\\tlog_qp25";
	string quant_base = "D:\\txt\\qlog_qp25";

	string macierze = macierze_base+".txt";
	string transformLog = transform_base+".txt";
	string quantlog = quant_base+".txt";

	ifstream matrix(macierze);
	ifstream transform(transformLog);
	ifstream quant(quantlog);

	string serial_matrix = macierze_base + "_serial.txt";
	ofstream serial_matrix_os;
	serial_matrix_os.open(serial_matrix,'w');

	string t_input = transform_base + "_transform_input.txt";
	string t_hor = transform_base + "_transform_hor.txt";
	string t_ver = transform_base + "_transform_ver.txt";
	string it_input = transform_base + "_itransform_input.txt";
	string it_hor = transform_base + "_itransform_hor.txt";
	string it_ver = transform_base + "_itransform_ver.txt";
	string quant_input = quant_base + "_quant_input.txt";
	string quant_result = quant_base + "_quant_result.txt";
	string iquant_result = quant_base + "_iquant_result.txt";

	ofstream tr_input_os;
	tr_input_os.open(t_input,'w');
	ofstream t_hor_os;
	t_hor_os.open(t_hor,'w');
	ofstream t_ver_os;
	t_ver_os.open(t_ver,'w');
	ofstream it_input_os;
	it_input_os.open(it_input,'w');
	ofstream it_hor_os;
	it_hor_os.open(it_hor,'w');
	ofstream it_ver_os;
	it_ver_os.open(it_ver,'w');
	ofstream quant_input_os;
	quant_input_os.open(quant_input,'w');
	ofstream quant_result_os;
	quant_result_os.open(quant_result,'w');
	ofstream iquant_result_os;
	iquant_result_os.open(iquant_result,'w');
	
	matrix >> s;

	while(s[0]=='*')
	{
		matrix >> bok;
		size=bok*bok;

		transfer(matrix,serial_matrix_os,size);
		
		transform >> s; transform >> s; while(s[0]!='*') transform >> s;
		quant >> s; quant >> s; while(s[0]!='*') quant >> s;
		
		transfer(transform,tr_input_os,size);
		transform >> s; transform >> s; while(s[0]!='*') transform >> s;
		transfer(transform,t_hor_os,size);
		transform >> s; transform >> s; while(s[0]!='*') transform >> s;
		transfer(transform,t_ver_os,size);
		transform >> s; transform >> s; while(s[0]!='*') transform >> s;
		transfer(transform,it_input_os,size);
		transform >> s; transform >> s; while(s[0]!='*') transform >> s;
		transfer(transform,it_ver_os,size);
		transform >> s; transform >> s; while(s[0]!='*') transform >> s;
		transfer(transform,it_hor_os,size);

		transfer(quant,quant_input_os,size);
		quant >> s; quant >> s; while(s[0]!='*') quant >> s;
		transfer(quant,quant_result_os,size);
		quant >> s; quant >> s; while(s[0]!='*') quant >> s;
		transfer(quant,iquant_result_os,size);

		matrix >> s;
	}

	matrix.close();
	transform.close();
	quant.close();
	serial_matrix_os.close();
	tr_input_os.close();
	t_hor_os.close();
	t_ver_os.close();
	it_input_os.close();
	it_ver_os.close();
	it_hor_os.close();
	quant_input_os.close();
	quant_result_os.close();
	iquant_result_os.close();

	system("PAUSE");

	return EXIT_SUCCESS;
}

#endif