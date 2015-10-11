/**
 * @file	Picture.cpp
 *
 * @brief	implementacja klasy obrazu
 */

#include "Picture.h"
#include "CTU.h"
#include "Slice.h"
#include "CU.h"
#include <cassert>
#include <fstream>

namespace H265Lib
{
	Picture::Picture():
		_CTUs(0,0), _tilesMap(0,0), _slicesMap(0,0)
	{
		resetSampleBuffers();
	}

	Picture::Picture(ParametersBundle parameters) :
		_CTUs(0, 0), _tilesMap(0, 0), _slicesMap(0, 0), Parameters(parameters)
	{
		resetSampleBuffers();
		initFromParameters(Parameters);
	}

	Void Picture::initFromParameters(ParametersBundle parameters)
	{
		assert(parameters.Sps != nullptr);
		assert(parameters.Pps != nullptr);

		Parameters.Sps = parameters.Sps;
		Parameters.Pps = parameters.Pps;

		setSize(
			Parameters.Sps->getPicWidth(ImgComp::Luma),
			Parameters.Sps->getPicHeight(ImgComp::Luma), 
			Parameters.Sps->getPicWidth(ImgComp::Cb), 
			Parameters.Sps->getPicHeight(ImgComp::Cb));

		_slices.clear();

		if (Parameters.Pps->getTilesEnabled() == true)
		{
			_tilesMap = Matrix<UShort>(Parameters.Sps->getPicWidthInCTUs(), Parameters.Sps->getPicHeightInCTUs());
		}

		initCTUs();
	}

	void Picture::setSize(UInt lumaWidth, UInt lumaHeight, UInt chromaWidth, UInt chromaHeight)
	{
		resetSampleBuffers();

		if (lumaWidth == 0 || lumaHeight == 0) return;

		_inputSamplesY = std::make_shared<Matrix<Sample>>(lumaWidth, lumaHeight);
		_reconstructedSamplesY = std::make_shared<Matrix<Sample>>(lumaWidth, lumaHeight);

		_inputSamplesCb = std::make_shared<Matrix<Sample>>(chromaWidth, chromaHeight);
		_inputSamplesCr = std::make_shared<Matrix<Sample>>(chromaWidth, chromaHeight);
		_reconstructedSamplesCb = std::make_shared<Matrix<Sample>>(chromaWidth, chromaHeight);
		_reconstructedSamplesCr = std::make_shared<Matrix<Sample>>(chromaWidth, chromaHeight);

	}

	void Picture::loadFrameFromYuv(std::ifstream& yuvFile)
	{
		UChar tmp;
		UInt width = Parameters.Sps->getPicWidth(), height = Parameters.Sps->getPicHeight();

		for (UInt i = 0; i < height; ++i)
		{
			for (UInt j = 0; j < width; ++j)
			{
				yuvFile.read(reinterpret_cast<Char*>(&tmp), 1);
				_inputSamplesY->at(j, i) = tmp;
			}
		}

		UInt ch = Parameters.Sps->getPicHeight(ImgComp::Cb);
		UInt cw = Parameters.Sps->getPicWidth(ImgComp::Cb);

		for (UInt i = 0; i < ch; ++i)
		{
			for (UInt j = 0; j < cw; ++j)
			{
				yuvFile.read(reinterpret_cast<Char*>(&tmp), 1);
				_inputSamplesCb->at(j, i) = tmp;
			}
		}
		for (UInt i = 0; i < ch; ++i)
		{
			for (UInt j = 0; j < cw; ++j)
			{
				yuvFile.read(reinterpret_cast<Char*>(&tmp), 1);
				_inputSamplesCr->at(j, i) = tmp;
			}
		}
	}

	std::shared_ptr<Matrix<Sample>> Picture::getInputSamples(const ImgComp comp)
	{
		if (comp == ImgComp::Luma)
			return _inputSamplesY;
		else if (comp == ImgComp::Cr)
			return _inputSamplesCr;
		else
			return _inputSamplesCb;
	}

	std::shared_ptr<Matrix<Sample>> Picture::getReconstructionMatrix(const ImgComp comp)
	{
		if (comp == ImgComp::Luma)
			return _reconstructedSamplesY;
		else if (comp == ImgComp::Cr)
			return _reconstructedSamplesCr;
		else
			return _reconstructedSamplesCb;
	}

	std::shared_ptr<CTU> Picture::getCTU(UInt ctuX, UInt ctuY)
	{
		return _CTUs(ctuX, ctuY);
	}

	std::shared_ptr<CTU> Picture::getCTUBySamplePosition(UInt sampleX, UInt sampleY)
	{
		auto ctuX = sampleX / Parameters.Sps->getCTUSize();
		auto ctuY = sampleY / Parameters.Sps->getCTUSize();

		return getCTU(ctuX, ctuY);
	}

	Void Picture::resetSampleBuffers()
	{
		_inputSamplesY.reset();
		_inputSamplesCb.reset();
		_inputSamplesCr.reset();

		_reconstructedSamplesY.reset();
		_reconstructedSamplesCb.reset();
		_reconstructedSamplesCr.reset();
	}

	Void Picture::initCTUs()
	{
		UShort CTUSize = Parameters.Sps->getCTUSize();
		UShort picWidthInCTUs = Parameters.Sps->getPicWidthInCTUs(), picHeightInCTUs = Parameters.Sps->getPicHeightInCTUs();

		_CTUs.resize(picWidthInCTUs, picHeightInCTUs);

		for (UInt i = 0; i < picHeightInCTUs; ++i)
		{
			for (UInt j = 0; j < picWidthInCTUs; ++j)
			{
				_CTUs(j, i) = std::make_shared<CTU>(j*CTUSize, i*CTUSize, CTUSize, Parameters);
			}
		}
	}

	Void Picture::printDescription()
	{
		/*//LOG( "PART" ) << "Obraz " << SeqParams( )->getPicWidth( ) << " x " << SeqParams( )->getPicHeight( ) << std::endl;
		printMatrix( itsSamplesY, SeqParams( )->getPicWidth( ), SeqParams( )->getPicHeight( ), //LOG( "PART" ) );
		//LOG_TAB );
		for( UInt i = 0; i < itsCTUs.size( ); ++i )
		{
		if( itsCTUs[ i ] != nullptr ) itsCTUs[ i ]->printDescription( );
		}
		//LOG_UNTAB );*/
	}

	std::shared_ptr<CU> Picture::getCuContainingPosition(UInt x, UInt y)
	{
		std::shared_ptr<CTU> ctu = getCTUBySamplePosition(x, y);
		std::shared_ptr<CU> cu = nullptr;
		/*std::shared_ptr<CUQuadTree> tree = ctu->getCUQuadTree();

		do
		{
		if (tree->getQTMode() == QTMODE_LEAF)
		{
		cu = tree->getLeaf();
		}
		else
		{
		UInt step = tree->getSize() / 2;
		Int left, up;
		left = tree->getX() + step > x ? 1 : 0;
		up = tree->getY() + step > x ? 1 : 0;
		QTComponent comp;
		if (left && up)
		comp = UPPERLEFT;
		else if (!left && up)
		comp = UPPERRIGHT;
		else if (left && !up)
		comp = LOWERLEFT;
		else
		comp = LOWERRIGHT;

		tree = tree->getSubTree(comp);
		}
		} while (cu == nullptr);*/
		return cu;
	}
}