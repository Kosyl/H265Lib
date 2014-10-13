#ifndef _YUVREADERTEST_
#define _YUVREADERTEST_

#include "Frame.h"
#include "
Int main(Int argc, Char* argv[])
{	
	Frame f;
	f.setSize(176,144);
	std::ifstream yuv("D:\\h265\\yuv\\akiyo_qcif.yuv");
	f.loadFrameFromYuv(yuv);

	system("PAUSE");
	return EXIT_SUCCESS;
}

#endif