#include "stdafx.h"
#include <Data/Picture.h>
#include <Modules/PictureEncoder.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace UnitTests
{
	TEST_CLASS(IntraPictureEncoderTests)
	{
	public:

		TEST_METHOD(encodeSamplePicture_PrintResult)
		{
			auto parameters = ParametersBundle::getDefaultParameters(120, 120);
			auto pic = std::make_unique<Picture>();
			pic->initFromParameters(parameters);
			
			HardcodedPictureEncoder encoder(parameters);

			encoder.encodePicture(*pic);

			printPictureDescription(*pic);

			Log::flush();
		}

		void printPictureDescription(Picture& pic)
		{
			auto widthInCTUs = pic.width_in_ctus;
			auto heightInCTUs = pic.height_in_ctus;

			for (auto y = 0u; y < heightInCTUs; ++y)
			{
				for (auto x = 0u; x < widthInCTUs; ++x)
				{
					auto ctu = pic.getCTU(x, y);
					Log::println("CTU: x=", ctu->pos.x, ", y=", ctu->pos.y, ", size=", ctu->size());
					//printCUTreeDescription(ctu->CUQuadTree);
				}
			}
		}
		void printCUTreeDescription(std::shared_ptr<CUQuadTree> subTree)
		{
			Log::tab();
			if (subTree->isLeaf())
			{
				auto leaf = subTree->getCU();
				Log::println("CU: x=", leaf->pos.x, ", y=", leaf->pos.y, ", size=", leaf->size());
			}
			else
			{
				Log::println("CUTree: x=", subTree->pos.x, ", y=", subTree->pos.y, ", size=", subTree->size());
				/*	for (auto& i : subTree->subtrees)
					{
						if (i != nullptr)
							printCUTreeDescription(i);
					}*/
			}
			Log::untab();
		}
	};
}