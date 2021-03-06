/** \file     Main.cpp
\brief    Test funkcji transformacji i kwantyzacji
*/

#include <ParameterSets/EncoderParameters.h>
#include <Modules/SequenceEncoder.h>
#include "../TinyXML/tinyxml2.h"
#include <Common/JSON.h>
#ifndef _MAIN_
#define _MAIN_

#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <iomanip> 

int main( int argc, char* argv[] )
{
	HEVC::EncoderParameters params;
	params.initFromConfigurationFile(std::string(""));

	HEVC::SequenceEncoder encoder;
	encoder.encodeSequence(params);

	std::cout << "Nacisnij klawisz, aby zakonczyc...";
	getchar();
}

#endif