#include "stdafx.h"
#include <Data/Picture.h>
#include <Modules/PictureEncoder.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace H265Lib;

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
			auto pic = std::make_shared<Picture>(ParametersBundle::getDefaultParameters(100,100));
			
			IntraPictureEncoder encoder;

			encoder.encodePicture(pic);

			printPictureDescription(*pic);
		}

		Void printPictureDescription(Picture& pic)
		{
			auto widthInCTUs = pic.Parameters.Sps->getPicWidthInCTUs(), heightInCTUs = pic.Parameters.Sps->getPicHeightInCTUs();

			for (auto y = 0; y < heightInCTUs; ++y)
			{
				for (auto x = 0; x < widthInCTUs; ++x)
				{
					auto ctu = pic.getCTU(x, y);
					Log::println("CTU: x=", ctu->Position.X, ", y=", ctu->Position.Y, ", size=", ctu->getSize());
					printCUTreeDescription(ctu->getCUQuadTree());
				}
			}
		}
		Void printCUTreeDescription(std::shared_ptr<CUQuadTree> subTree)
		{
			Log::tab();
			if (subTree->getQTMode() == QTMode::Leaf)
			{
				auto leaf = subTree->getLeaf();
				Log::println("CU: x=", leaf->Position.X, ", y=", leaf->Position.Y, ", size=", leaf->getSize());
			}
			else
			{
				Log::println("CUTree: x=", subTree->Position.X, ", y=", subTree->Position.Y, ", size=", subTree->getSize());
				for (auto& i : subTree->getSubTrees())
				{
					if (i != nullptr)
						printCUTreeDescription(i);
				}
			}
			Log::untab();
		}
	};
}