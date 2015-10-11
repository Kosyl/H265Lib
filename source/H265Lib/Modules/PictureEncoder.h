#pragma once

#include <Data/BlockBase.h>
#include <Data/Picture.h>
#include <Data/QuadTree.h>
#include <Data/CTU.h>
#include <Data/CU.h>

namespace H265Lib
{
	interface IPictureEncoder
	{
		IPictureEncoder() = default;
		virtual ~IPictureEncoder() = default;

		virtual Void encodePicture(std::shared_ptr<Picture> picture) = 0;
	};

	class IntraPictureEncoder: public IPictureEncoder
	{

	protected:
		std::shared_ptr<Picture> _picture;
		ParametersBundle Parameters;

	public:
		IntraPictureEncoder();
		~IntraPictureEncoder();

		virtual Void encodePicture(std::shared_ptr<Picture> picture) override
		{
			_picture = picture;

			processPicture();
		}

	protected:

		Void processPicture()
		{
			auto widthInCTUs = Parameters.Sps->getPicWidthInCTUs(), heightInCTUs = Parameters.Sps->getPicHeightInCTUs();

			for (auto y = 0; y < heightInCTUs; ++y)
			{
				for (auto x = 0; x < widthInCTUs; ++x)
				{
					processCTU(_picture->getCTU(x, y));
				}
			}
		}

		Void processCTU(std::shared_ptr<CTU> ctu)
		{
			divideCUTreeIntoSmallestCUs(ctu->getCUQuadTree());
		}

		Void divideCUTreeIntoSmallestCUs(std::shared_ptr<CUQuadTree> subTree)
		{
			auto treeSize = subTree->getSize();
			auto isMinAllowedSize = treeSize <= Parameters.Sps->getMinCUSize();
			auto isSmallerThanWholePicture = treeSize <= Parameters.Sps->getPicWidth() && treeSize <= Parameters.Sps->getPicHeight();
			if (isMinAllowedSize&&isSmallerThanWholePicture)
			{
				subTree->rebuild(QTMode::Leaf);
			}
			else
			{
				subTree->rebuild(QTMode::Split);
				for (auto newSubTree : subTree->getSubTrees())
				{
					if (newSubTree != nullptr)
						divideCUTreeIntoSmallestCUs(newSubTree);
				}
			}
		}
	};
}