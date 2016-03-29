#include "stdafx.h"
#include <Data/Picture.h>
#include <Modules/PictureEncoder.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace UnitTests
{
	class ExposedIntraEncoder: public IntraPictureEncoder
	{
		
	};

	TEST_CLASS(IntraPictureEncoderTests)
	{
	public:

		TEST_METHOD(encodeSamplePicture_PrintResult)
		{
			auto parameters = ParametersBundle::getDefaultParameters(100, 100);
			auto pic = std::make_shared<Picture>();
			pic->initFromParameters(parameters.Sps);
			
			IntraPictureEncoder encoder;
			encoder.Parameters = parameters;

			encoder.encodePicture(pic);

			printPictureDescription(*pic);

			Log::flush();
		}

		void printPictureDescription(Picture& pic)
		{
			auto widthInCTUs = pic.width_in_ctus;
			auto heightInCTUs = pic.height_in_ctus;

			for (auto y = 0; y < heightInCTUs; ++y)
			{
				for (auto x = 0; x < widthInCTUs; ++x)
				{
					auto ctu = pic.getCTU(x, y);
					Log::println("CTU: x=", ctu->x, ", y=", ctu->y, ", size=", ctu->size);
					printCUTreeDescription(ctu->CUQuadTree);
				}
			}
		}
		void printCUTreeDescription(std::shared_ptr<CUQuadTree> subTree)
		{
			Log::tab();
			if (subTree->mode == QTMode::Leaf)
			{
				auto leaf = subTree->leaf;
				Log::println("CU: x=", leaf->x, ", y=", leaf->y, ", size=", leaf->size);
			}
			else
			{
				Log::println("CUTree: x=", subTree->x, ", y=", subTree->y, ", size=", subTree->size);
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