#include "PictureEncoder.h"
#include <Common/Logger.h>

namespace HEVC
{

	HardcodedPictureEncoder::HardcodedPictureEncoder(ParametersBundle parameter_sets) :
		parameters(parameter_sets)
	{
	}

	void HardcodedPictureEncoder::encodePicture(Picture &pic)
	{
		LOG_FUNCTION_INDENT(Logger::Dump);
		LOGLN(Logger::Dump, "width: ",pic.width[Luma], ", height: ", pic.height[Luma]);

		for (auto y = 0u; y < pic.height_in_ctus; ++y)
		{
			for (auto x = 0u; x < pic.width_in_ctus; ++x)
			{
				CuContext context;
				context.initFromParameters(parameters);
				context.picture = &pic;

				processQuadtree(*(pic.getCTU(x, y)->cu_tree), context);
			}
		}
	}

	void HardcodedPictureEncoder::processQuadtree(CUQuadTree &tree, CuContext context)
	{
		bool bigger_than_max_transform = tree.size() > context.max_tb_size;
		bool overlaps_picture_boundaries = 
			tree.pos.x + tree.size() > context.pic_width
			|| tree.pos.y + tree.size() > context.pic_height;

		bool hasToSplit = bigger_than_max_transform || overlaps_picture_boundaries;
		if (hasToSplit)
		{
			tree.buildSubtrees(context.pic_width, context.pic_height);
			for (auto &subtree : tree.subtrees) //kolejnosc z
			{
				if (subtree != nullptr)
					processQuadtree(*subtree, context);
			}
		}
		else //lisc
		{
			tree.buildLeaf();

			processCu(*(tree.getCU()), context);
		}
	}

	void HardcodedPictureEncoder::processCu(CU& cu, CuContext context)
	{
		LOG_INDENT(Logger::Dump);
		LOGLN(Logger::Dump, "CU(", cu.pos.x, ",", cu.pos.y, ") ", cu.size(), "x", cu.size());

		cu.prediction_type = PredictionType::Intra;
		cu.chroma_prediction_derivation_type = 4;
		cu.cu_qp_delta = 0;
		cu.transquant_bypass_on = false;
		cu.setPartitionMode(Partition::Mode_2Nx2N);

		prepareTransformTree(cu);

		auto tu_iterator = cu.transform_tree->begin();
		do
		{

		} 
		while (!tu_iterator.atEnd());
	}

	void HardcodedPictureEncoder::prepareTransformTree(CU& cu)
	{
		cu.transform_tree = std::make_shared<TUQuadTree>(cu.pos.x, cu.pos.y, cu.size());
		cu.transform_tree->buildLeaf();
	}
}
