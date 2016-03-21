#pragma once

#include <Data/BlockBase.h>
#include <Data/Picture.h>
#include <Data/QuadTree.h>
#include <Data/CTU.h>
#include <Data/CU.h>

namespace HEVC
{
	interface IPictureEncoder
	{
		IPictureEncoder() = default;
		virtual ~IPictureEncoder() = default;

		virtual void encodePicture(std::shared_ptr<Picture> picture) = 0;
	};

	class IntraPictureEncoder: public IPictureEncoder
	{
	protected:
		std::shared_ptr<Picture> _picture;

	public:
		ParametersBundle Parameters;

		IntraPictureEncoder();
		~IntraPictureEncoder();

		virtual void encodePicture(std::shared_ptr<Picture> picture) override
		{
			_picture = picture;

			processPicture();
		}

	protected:

		void processPicture()
		{
			auto widthInCTUs = Parameters.Sps->pic_width_in_ctus, heightInCTUs = Parameters.Sps->pic_height_in_ctus;

			for (auto y = 0; y < heightInCTUs; ++y)
			{
				for (auto x = 0; x < widthInCTUs; ++x)
				{
					processCTU(_picture->getCTU(x, y));
				}
			}
		}

		void processCTU(std::shared_ptr<CTU> ctu)
		{
			divideCUTreeIntoSmallestCUs(ctu->CUQuadTree);
		}

		void divideCUTreeIntoSmallestCUs(std::shared_ptr<CUQuadTree> subTree)
		{
			auto treeSize = subTree->size;
			auto isMinAllowedSize = treeSize <= Parameters.Sps->min_luma_coding_block_size;
			auto isSmallerThanWholePicture = treeSize <= Parameters.Sps->getPicWidth() && treeSize <= Parameters.Sps->getPicHeight();
			if (isMinAllowedSize&&isSmallerThanWholePicture)
			{
				subTree->rebuild(QTMode::Leaf, Parameters.Sps->getPicWidth(), Parameters.Sps->getPicHeight());
			}
			else
			{
				subTree->rebuild(QTMode::Split, Parameters.Sps->getPicWidth(), Parameters.Sps->getPicHeight());
				for (auto newSubTree : subTree->subtrees)
				{
					//if (newSubTree != nullptr)
						//divideCUTreeIntoSmallestCUs(newSubTree);
				}
			}
		}
	};
}