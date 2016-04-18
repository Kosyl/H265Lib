#include "PictureEncoder.h"

namespace HEVC
{
	IntraPictureEncoder::IntraPictureEncoder(ParametersBundle parameter_sets) :
		parameters(parameter_sets)
	{

	}

	IntraPictureEncoder::~IntraPictureEncoder()
	{

	}

	void IntraPictureEncoder::encodePicture(Picture &pic)
	{
		auto widthInCTUs = parameters.Sps->pic_width_in_ctus, heightInCTUs = parameters.Sps->pic_height_in_ctus;

		for (auto y = 0; y < heightInCTUs; ++y)
		{
			for (auto x = 0; x < widthInCTUs; ++x)
			{
				processCTU(pic.getCTU(x, y));
			}
		}
	}

	void IntraPictureEncoder::processCTU(std::shared_ptr<CTU> ctu)
	{
		//divideCUTreeIntoSmallestCUs(ctu->CUQuadTree);
	}

	void IntraPictureEncoder::divideCUTreeIntoSmallestCUs(std::shared_ptr<CUQuadTree> subTree)
	{
		auto treeSize = subTree->size();
		auto isMinAllowedSize = treeSize <= parameters.Sps->min_luma_coding_block_size;
		auto isSmallerThanWholePicture = treeSize <= parameters.Sps->getPicWidth() && treeSize <= parameters.Sps->getPicHeight();
		if (isMinAllowedSize&&isSmallerThanWholePicture)
		{
			subTree->buildLeaf();
		}
		else
		{
			subTree->buildSubtrees(parameters.Sps->getPicWidth(), parameters.Sps->getPicHeight());
			for (auto& newSubTree : subTree->subtrees)
			{
				//if (newSubTree != nullptr)
				//divideCUTreeIntoSmallestCUs(newSubTree);
			}
		}
	}

	HardcodedPictureEncoder::HardcodedPictureEncoder(ParametersBundle parameter_sets) :
		parameters(parameter_sets)
	{
	}

	void HardcodedPictureEncoder::encodePicture(Picture &pic)
	{
		for (auto y = 0u; y < pic.height_in_ctus; ++y)
		{
			for (auto x = 0u; x < pic.width_in_ctus; ++x)
			{
				CtuContext context;
				context.initFromparameters(parameters);

				context.left = pic.getCTU(x - 1, y);
				context.upper = pic.getCTU(x, y - 1);
				context.upper_left = pic.getCTU(x - 1, y - 1);
				context.upper_right = pic.getCTU(x + 1, y - 1);
				context.lower_left = pic.getCTU(x - 1, y + 1);
				context.lower = pic.getCTU(x, y + 1);

				processQuadtree(*(pic.getCTU(x, y)->cu_tree), context);
			}
		}
	}

	void HardcodedPictureEncoder::processQuadtree(CUQuadTree &tree, CtuContext context)
	{
		bool hasToSplit = tree.size() > context.max_tb_size;
		if (hasToSplit)
		{
			tree.buildSubtrees(context.pic_width, context.pic_height);
			for(auto &subtree: tree.subtrees) //kolejnosc z
			{
				processQuadtree(*subtree, context);
			}
		}
		else //lisc
		{
			tree.buildLeaf();
			
		}
	}
}
