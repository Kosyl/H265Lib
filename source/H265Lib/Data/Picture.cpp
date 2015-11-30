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
	Picture::Picture()
	{
		resetSampleBuffers();
	}

	Picture::Picture(ParametersBundle parameters) :
		Picture::Picture()
	{
		initFromParameters(parameters);
	}

	Void Picture::initFromParameters(ParametersBundle parameters)
	{
		assert(parameters.Sps != nullptr);
		assert(parameters.Pps != nullptr);

		_widthLuma = parameters.Sps->getPicWidth(ImgComp::Luma);
		_widthChroma = parameters.Sps->getPicWidth(ImgComp::Cb);
		_heightLuma = parameters.Sps->getPicHeight(ImgComp::Luma);
		_heightChroma = parameters.Sps->getPicHeight(ImgComp::Cb);
		_tilesEnabled = parameters.Pps->getTilesEnabled();
		_widthInCTUs = parameters.Sps->getPicWidthInCTUs();
		_heightInCTUs = parameters.Sps->getPicHeightInCTUs();
		_ctuSize = parameters.Sps->getCTUSize();

		setSize(_widthLuma, _widthChroma, _heightLuma, _heightChroma);
	}

	void Picture::setSize(UShort lumaWidth, UShort lumaHeight, UShort chromaWidth, UShort chromaHeight)
	{
		resetSampleBuffers();

		if (lumaWidth == 0 || lumaHeight == 0) return;

		_inputSamplesY = std::make_shared<Matrix<Sample>>(lumaWidth, lumaHeight);
		_reconstructedSamplesY = std::make_shared<Matrix<Sample>>(lumaWidth, lumaHeight);

		_inputSamplesCb = std::make_shared<Matrix<Sample>>(chromaWidth, chromaHeight);
		_inputSamplesCr = std::make_shared<Matrix<Sample>>(chromaWidth, chromaHeight);
		_reconstructedSamplesCb = std::make_shared<Matrix<Sample>>(chromaWidth, chromaHeight);
		_reconstructedSamplesCr = std::make_shared<Matrix<Sample>>(chromaWidth, chromaHeight);

		_slices.clear();

		if (_tilesEnabled)
		{
			_tilesMap = Matrix<UShort>(_widthInCTUs, _heightInCTUs);
		}

		initCTUs();
	}

	void Picture::loadFrameFromYuv(std::ifstream& yuvFile)
	{
		UChar tmp;
		UInt width = _widthLuma, height = _heightLuma;

		for (UInt i = 0; i < height; ++i)
		{
			for (UInt j = 0; j < width; ++j)
			{
				yuvFile.read(reinterpret_cast<Char*>(&tmp), 1);
				_inputSamplesY->at(j, i) = tmp;
			}
		}

		UInt ch = _widthChroma;
		UInt cw = _heightChroma;

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

	std::shared_ptr<CTU> Picture::getCTU(UShort ctuX, UShort ctuY)
	{
		return _CTUs(ctuX, ctuY);
	}

	std::shared_ptr<CTU> Picture::getCTUBySamplePosition(UShort sampleX, UShort sampleY)
	{
		auto ctuX = sampleX / _ctuSize;
		auto ctuY = sampleY / _ctuSize;

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
		_CTUs.resize(_widthInCTUs, _heightInCTUs);

		for (UInt i = 0; i < _heightInCTUs; ++i)
		{
			for (UInt j = 0; j < _widthInCTUs; ++j)
			{
				_CTUs(j, i) = std::make_shared<CTU>(j*_ctuSize, i*_ctuSize, _ctuSize);
			}
		}
	}

	Void Picture::printDescription(LogId logId, Bool recursive = true, Bool printSamples = false)
	{
		LOGLN(logId, "Obraz ", _widthLuma, " x ", _widthChroma);
		if (printSamples)
		{
			LOG_MATRIX(logId, _inputSamplesY);
		}
		if (recursive)
		{
			LOG_SCOPE_INDENT(logId, "CTU:");
			for (auto& ctu : _CTUs)
			{
				if (ctu != nullptr) ctu->printDescription(logId, recursive);
			}
		}
	}

	std::shared_ptr<CU> Picture::getCuContainingPosition(UShort x, UShort y)
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

	UShort Picture::getWidth(const ImgComp comp) const
	{
		if (comp == Luma)
			return _widthLuma;
		return _widthChroma;
	}

	UShort Picture::getHeight(const ImgComp comp) const
	{
		if (comp == Luma)
			return _heightLuma;
		return _heightChroma;
	}
}