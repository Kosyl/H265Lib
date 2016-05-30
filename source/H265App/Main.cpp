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

void xmlTest()
{
	nlohmann::json j;

	std::ifstream t( "D:/HEVC/decoder.txt" );
	std::stringstream buffer;
	buffer << t.rdbuf( );
	std::string s = buffer.str( );

  j =	j.parse( s );

	int vps_temporal_id_nesting_flag = j["vps"][ "vps_temporal_id_nesting_flag" ].get<int>();
	int profile_compatibility_flags2 = j[ "vps" ][ "profileTier" ]["profile_compatibility_flags"][2].get<int>( );
	int sps = j[ "sps" ][ "max_transform_hierarchy_depth_intra" ].get<int>( );
}

int main( int argc, char* argv[] )
{
	xmlTest( );
	//HEVC::EncoderParameters params;
	//params.initFromConfigurationFile(std::string(""));

	//HEVC::SequenceEncoder encoder;
	//encoder.encodeSequence(params);

	std::cout << "Nacisnij klawisz, aby zakonczyc...";
	getchar();
}

#endif