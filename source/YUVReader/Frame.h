/** \file     Quant.h
    \brief    Klasa kwantyzatora (header)
*/
#ifndef _FRAME_
#define _FRAME_

#include "TypeDef.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string.h>

class Frame
{
public:
	Frame();
	Frame(unsigned int width, unsigned int height, unsigned short bitDepth);
	~Frame();
	Logger log;

	void loadFrameFromYuv(std::ifstream& yuvStream);
	void setSize(const unsigned int width, const unsigned int height);
	void deleteMatrices();
private:

	unsigned int bitDepth_;
	unsigned int width_;
	unsigned int height_;
	Short** YSamples_;
	Short** CbSamples_;
	Short** CrSamples_;
};

#endif