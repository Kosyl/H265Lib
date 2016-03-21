/**
 * @file	Picture.cpp
 *
 * @brief	implementacja klasy obrazu
 */

#include "Picture.h"
#include "CTU.h"
#include "Slice.h"
#include "CU.h"
#include "Common/Logger.h"
#include <cassert>
#include <fstream>

namespace HEVC
{
	Picture::Picture()
	{
	}

	void Picture::initFromParameters(ParametersBundle parameters)
	{
		assert(parameters.Sps != nullptr);
		assert(parameters.Pps != nullptr);

		width_luma = parameters.Sps->getPicWidth(Luma);
		width_chroma = parameters.Sps->getPicWidth(Cb);
		height_luma = parameters.Sps->getPicHeight(Luma);
		height_chroma = parameters.Sps->getPicHeight(Cb);
		tiles_enabled = parameters.Pps->tiles_enabled_flag;
		width_in_ctus = parameters.Sps->pic_width_in_ctus;
		height_in_ctus = parameters.Sps->pic_height_in_ctus;
		ctu_size = parameters.Sps->ctu_size;
		setSize(width_luma, height_luma, width_chroma, height_chroma);
	}

	void Picture::setSize(int lumaWidth, int lumaHeight, int chromaWidth, int chromaHeight)
	{
		if (lumaWidth == 0 || lumaHeight == 0) return;

		input_samples[Luma] = Matrix<Sample>(lumaWidth, lumaHeight);
	  reconstructed_samples[Luma] = Matrix<Sample>(lumaWidth, lumaHeight);

		input_samples[Cb] = Matrix<Sample>(chromaWidth, chromaHeight);
		input_samples[Cr] = Matrix<Sample>(chromaWidth, chromaHeight);
		reconstructed_samples[Cb] = Matrix<Sample>(chromaWidth, chromaHeight);
		reconstructed_samples[Cr] = Matrix<Sample>(chromaWidth, chromaHeight);

		slices.clear();

		if (tiles_enabled)
		{
			tiles_map = Matrix<int>(width_in_ctus, height_in_ctus);
		}

		initCTUs();
	}

	void Picture::loadFrameFromYuv(std::ifstream& yuvFile)
	{
		unsigned char tmp;
		int width = width_luma, height = height_luma;

		auto luma = input_samples[Luma];
		auto cb = input_samples[Cb];
		auto cr = input_samples[Cr];

		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				yuvFile.read(reinterpret_cast<char*>(&tmp), 1);
				luma(j, i) = tmp;
			}
		}

		int ch = width_chroma;
		int cw = height_chroma;

		for (int i = 0; i < ch; ++i)
		{
			for (int j = 0; j < cw; ++j)
			{
				yuvFile.read(reinterpret_cast<char*>(&tmp), 1);
				cb(j, i) = tmp;
			}
		}
		for (int i = 0; i < ch; ++i)
		{
			for (int j = 0; j < cw; ++j)
			{
				yuvFile.read(reinterpret_cast<char*>(&tmp), 1);
				cr(j, i) = tmp;
			}
		}
	}
	
	std::shared_ptr<CTU> Picture::getCTU(int ctuX, int ctuY)
	{
		return CTUs(ctuX, ctuY);
	}

	std::shared_ptr<CTU> Picture::getCTUBySamplePosition(int sampleX, int sampleY)
	{
		auto ctuX = sampleX / ctu_size;
		auto ctuY = sampleY / ctu_size;

		return getCTU(ctuX, ctuY);
	}

	void Picture::initCTUs()
	{
		CTUs = Matrix<std::shared_ptr<CTU>>(width_in_ctus, height_in_ctus);

		for (int i = 0; i < height_in_ctus; ++i)
		{
			for (int j = 0; j < width_in_ctus; ++j)
			{
				CTUs(j, i) = std::make_shared<CTU>(j*ctu_size, i*ctu_size, ctu_size);
			}
		}
	}

	void Picture::printDescription(LogId logId, bool recursive, bool printSamples)
	{
		LOGLN(logId, "Obraz ", width_luma, " x ", height_luma);
		if (printSamples)
		{
			LOG_MATRIX(logId, input_samples[Luma]);
		}
		if (recursive)
		{
			LOG_SCOPE_INDENT(logId, "CTU:");
			for (auto& ctu : CTUs)
			{
				if (ctu != nullptr) ctu->print(logId, recursive);
			}
		}
	}

	std::shared_ptr<CU> Picture::getCuContainingPosition(int x, int y)
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
		int step = tree->getSize() / 2;
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

	int Picture::getWidth(const ImgComp comp) const
	{
		if (comp == Luma)
			return width_luma;
		return width_chroma;
	}

	int Picture::getHeight(const ImgComp comp) const
	{
		if (comp == Luma)
			return height_luma;
		return height_chroma;
	}
}