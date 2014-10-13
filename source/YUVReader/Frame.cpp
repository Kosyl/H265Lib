#include "Frame.h"
#include "Logger.h"

Frame::Frame()
{
	Frame(0,0,8);
}

Frame::Frame(unsigned int width, unsigned int height, unsigned short bitDepth):
	log(),
	bitDepth_(bitDepth)
{
	setSize(width,height);
}

Frame::~Frame()
{
	deleteMatrices();
}

void Frame::deleteMatrices()
{
	if(YSamples_)
	{
		for(UInt i=0;i<width_;++i)
		{
			delete[] YSamples_[i];
		}
		delete[] YSamples_;

		UInt tmp=width_/2;
		for(UInt i=0;i<tmp;++i)
		{
			delete[] CrSamples_[i];
			delete[] CbSamples_[i];
		}
		delete[] CrSamples_;
		delete[] CrSamples_;
	}
}

void Frame::setSize(const UInt width, const UInt height)
{
	if(width==width_ && height==height_)return;

	deleteMatrices();

	if(width==0 || height==0) return;

	width_=width;
	height_=height;
	YSamples_=new Short*[width];
	for(UInt i=0;i<width;++i)
	{
		YSamples_[i]=new Short[height];
	}
	UInt ch = height/2;
	UInt cw = width/2;

	CbSamples_=new Short*[cw];
	CrSamples_=new Short*[cw];
	for(UInt i=0;i<cw;++i)
	{
		CrSamples_[i]=new Short[ch];
		CrSamples_[i]=new Short[ch];
	}
}

void Frame::loadFrameFromYuv(std::ifstream& yuvFile)
{
	for(UInt i=0;i<width_;++i)
	{
		for(UInt j=0;j<height_;++j)
		{
			yuvFile >> YSamples_[i][j];
			std::cout << YSamples_[i][j] << " ";
		}
		std::cout << std::endl;
	}

	UInt ch = height_/2;
	UInt cw = width_/2;
	for(UInt i=0;i<cw;++i)
	{
		for(UInt j=0;j<ch;++j)
		{
			yuvFile >> CbSamples_[i][j];
		}
	}
	for(UInt i=0;i<cw;++i)
	{
		for(UInt j=0;j<ch;++j)
		{
			yuvFile >> CrSamples_[i][j];
		}
	}
}
