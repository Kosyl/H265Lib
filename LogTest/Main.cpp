/** \file     Main.cpp
\brief    Test funkcji transformacji i kwantyzacji
*/

#ifndef _MAIN_
#define _MAIN_

#include "Logger.h"
#include "TypeDef.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <iomanip> 

class Class
{
public:
	void method()
	{
		LOG_FUNCTION_INDENT(Logs::RDO);
		LOG(RDO, __FUNCTION__, " wnetrze");
	}
	static void staticMethod()
	{
		LOG_FUNCTION_INDENT(Logs::RDO);
		LOG(RDO, __FUNCTION__, " wnetrze");
	}
};

template <typename T>
T** getEmptyMatrix(UInt size1, UInt size2)
{
	T** tmp = new T*[size1];
	for (UInt i = 0; i < size1; ++i)
	{
		tmp[i] = new T[size2];
		memset(tmp[i], 0, size2*sizeof(T));
	}
	return tmp;
}

void fun2()
{
	LOG_FUNCTION_INDENT(Logs::RDO);
	LOG_FUNCTION_INDENT(Logs::RDO);
	LOG_FUNCTION_INDENT(Logs::RDO);
	LOG_FUNCTION_INDENT(Logs::RDO);
	LOG(RDO, __FUNCTION__, " wnetrze");
}

void fun1()
{
	Indent i(RDO);
	LOG(RDO, "fun1 wnetrze");
	fun2();
}

Int main(Int argc, Char* argv[])
{
	int x = 45;
	float f = 345.44f;
	std::string s = "jakis string";

	LOG(Logs::RDO, x, f, s);
	LOG(Binarization, x, f, s);
	LOG(RDO, PRINTVAR(s), PRINTVAR(Logs::Quant), PRINTVAR(Logs::LumaPrediction), PRINTVAR(Logs::RDO));
	LOG(RDO, PRINTVAR(x), PRINTVAR(f), PRINTVAR(s));
	LOG(RDO, PRINTVAR(x) );
	LOG(RDO, PRINTVAR(f) );

	float** floats = getEmptyMatrix<float>(16,16);
	int** ints = getEmptyMatrix<int>(16,16);
	for (int i = 0; i < 256; ++i)
	{
		floats[i / 16][i % 16] = f;
		ints[i / 16][i % 16] = x;
		f += 1.2f;
		x += 3;
	}

	LOG_MATRIX_SQUARE(RDO, floats, 16);

	{
		LOG_SCOPE_INDENT(RDO);
		LOG_SCOPE_INDENT(LumaReco);

		LOG_MATRIX_RECT(RDO, ints, 16, 16);
		{
			LOG_SCOPE_INDENT(RDO);
			int tab2[16];
			for (int i = 0; i < 16 ; i++)
			{
				tab2[i] = i;
			}
			LOG_MATRIX_1D(Logs::RDO, tab2, 4, 4, 4);
		}
	}

	fun1();

	Class::staticMethod();
	Class c;
	c.method();

	return EXIT_SUCCESS;
}

#endif