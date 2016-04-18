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

	Picture::Picture(SequenceParameterSet& parameters)
	{
		initFromParameters(parameters);
	}

	void Picture::initFromParameters(SequenceParameterSet &sps)
	{
		width[Luma] = sps.getPicWidth(Luma);
		width[Cb] = sps.getPicWidth(Cb);
		width[Cr] = sps.getPicWidth(Cr);
		height[Luma] = sps.getPicHeight(Luma);
		height[Cb] = sps.getPicHeight(Cb);
		height[Cr] = sps.getPicHeight(Cr);
		width_in_ctus = sps.pic_width_in_ctus;
		height_in_ctus = sps.pic_height_in_ctus;
		ctu_size = sps.ctu_size;

		resetBuffers();
		initCTUs();
		slices.clear();

		if (tiles_enabled)
		{
			setTileIds(sps);
		}
	}

	void Picture::resetBuffers()
	{
		if (width[Luma] == 0 || height[Luma] == 0) return;

		input_samples[Luma] = Matrix<Sample>(width[Luma], height[Luma]);
		reconstructed_samples[Luma] = Matrix<Sample>(width[Luma], height[Luma]);

		input_samples[Cb] = Matrix<Sample>(width[Cb], height[Cb]);
		input_samples[Cr] = Matrix<Sample>(width[Cr], height[Cr]);
		reconstructed_samples[Cb] = Matrix<Sample>(width[Cb], height[Cb]);
		reconstructed_samples[Cr] = Matrix<Sample>(width[Cr], height[Cr]);
	}

	void Picture::initCTUs()
	{
		CTUs = Matrix<std::shared_ptr<CTU>>(width_in_ctus, height_in_ctus);

		for (auto i = 0u; i < height_in_ctus; ++i)
		{
			for (auto j = 0u; j < width_in_ctus; ++j)
			{
				CTUs(j, i) = std::make_shared<CTU>(j*ctu_size, i*ctu_size, ctu_size);
			}
		}
	}

	void Picture::setTileIds(SequenceParameterSet &sps)
	{
		//todo: przypisac tileId do CTU'ow na podstawie SPS
	}

	void Picture::loadFrameFromYuv(std::ifstream& yuvFile)
	{
		unsigned char tmp;

		auto luma = input_samples[Luma];
		auto cb = input_samples[Cb];
		auto cr = input_samples[Cr];

		for (size_t i = 0; i < height[Luma]; ++i)
		{
			for (size_t j = 0; j < width[Luma]; ++j)
			{
				yuvFile.read(reinterpret_cast<char*>(&tmp), 1);
				luma(j, i) = tmp;
			}
		}

		for (size_t i = 0; i < height[Cb]; ++i)
		{
			for (size_t j = 0; j < width[Cb]; ++j)
			{
				yuvFile.read(reinterpret_cast<char*>(&tmp), 1);
				cb(j, i) = tmp;
			}
		}
		for (size_t i = 0; i < height[Cr]; ++i)
		{
			for (size_t j = 0; j < width[Cr]; ++j)
			{
				yuvFile.read(reinterpret_cast<char*>(&tmp), 1);
				cr(j, i) = tmp;
			}
		}
	}

	std::shared_ptr<Slice> Picture::createSlice(size_t beginCtuX, size_t beginCtuY, size_t lengthInCtus)
	{
		auto slice = std::make_shared<Slice>(beginCtuX, beginCtuY, lengthInCtus);
		slice->slice_idx = slices.size();

		size_t x = beginCtuX, y = beginCtuY;

		while(lengthInCtus > 0)
		{
			auto ctu = this->CTUs(x, y);
			ctu->slice = slice;
			slice->assignCtu(ctu);

			x = (++x)%width_in_ctus;
			if (x == 0)
				++y;
			--lengthInCtus;
		}

		slices.push_back(slice);
		return slice;
	}

	std::shared_ptr<CTU> Picture::getCTU(size_t ctuX, size_t ctuY)
	{
		if (ctuX < 0 || ctuX >= CTUs.width() || ctuY < 0 || ctuY >= CTUs.height())
			return nullptr;
		return CTUs(ctuX, ctuY);
	}

	std::shared_ptr<CTU> Picture::getCTUBySamplePosition(int sampleX, int sampleY)
	{
		auto ctuX = sampleX / ctu_size;
		auto ctuY = sampleY / ctu_size;

		return getCTU(ctuX, ctuY);
	}

	void Picture::printDescription(LogId logId, bool recursive, bool printSamples)
	{
		LOGLN(logId, "Obraz ", width[Luma], " x ", height[Luma]);
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
}