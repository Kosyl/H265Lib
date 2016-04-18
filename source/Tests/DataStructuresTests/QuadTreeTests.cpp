#include "stdafx.h"
#include <Common/Bitset.h>
#include <Data/QuadTree.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace HEVC;

namespace UnitTests
{
	TEST_CLASS(QuadTreeTests)
	{
	public:

		TEST_METHOD(defaultCtorTest)
		{
			CUQuadTree root(0, 0, 64);
			root.buildSubtrees(100, 100);
			root.subtrees(0, 0)->buildLeaf();
			root.subtrees(1, 0)->buildLeaf();
			root.subtrees(0, 1)->buildSubtrees(100, 100);
			root.subtrees(0, 1)->subtrees(0, 0)->buildSubtrees(100, 100);
			root.subtrees(0, 1)->subtrees(0, 0)->subtrees(0, 0)->buildLeaf();
			root.subtrees(0, 1)->subtrees(0, 0)->subtrees(0, 1)->buildLeaf();
			root.subtrees(0, 1)->subtrees(0, 0)->subtrees(1, 0)->buildLeaf();
			root.subtrees(0, 1)->subtrees(0, 0)->subtrees(1, 1)->buildLeaf();
			root.subtrees(0, 1)->subtrees(1, 0)->buildLeaf();
			root.subtrees(0, 1)->subtrees(0, 1)->buildLeaf();
			root.subtrees(0, 1)->subtrees(1, 1)->buildLeaf();
			root.subtrees(1, 1)->buildLeaf();

			auto iter = root.begin();
			do
			{
				Log::println("(", iter->pos.x, ",",iter->pos.y,"), ", iter->size(),"x",iter->size());
				++iter;
			}
			while (!iter.atEnd());
		}
	};
}