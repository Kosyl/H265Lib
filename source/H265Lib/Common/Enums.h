#pragma once
#include <vector>

namespace HEVC
{
	enum class PredictionType
	{
		Intra = 0, Inter = 1
	};

	enum ImgComp
	{
		Luma = 0,
		Chroma = 1,
		Cb = 1,
		Cr = 2
	};

	enum EdgeType
	{
		Vertical = 0, Horizontal = 1
	};

	enum class IntraDirection
	{
		Left = 0, Top = 1, Corner = 2
	};

	enum class IntraModeType
	{
		Planar = 0, DC = 1, Linear = 2, Angular = 3
	};

	enum class ScanOrder // 2 dla predMode 6-14, 1 dla 22-30, 0 wpp
	{
		Diagonal = 0, Horizontal = 1, Vertical = 2
	};

	enum class SliceMode
	{
		I = 0, P = 1, B = 2
	};

	enum class SAOMode
	{
		None = 0, Band = 1, Edge = 2
	};

	enum TreePart
	{
		UpperLeft = 0, UpperRight = 1, LowerLeft = 2, LowerRight = 3
	};

	enum class Partition
	{
		Mode_NxN, Mode_2Nx2N, Mode_2NxN, Mode_Nx2N, Mode_2NxnU, Mode_2NxnD, Mode_nLx2N, Mode_nRx2N, Mode_NotSet
	};

	enum class QTMode
	{
		Split, Leaf, Unset
	};

	enum class SubsamplingFormat
	{
		Mode_Monochrome, Mode_420, Mode_422, Mode_444
	};

	enum class Indexing
	{
		ZScanByPixel, ZScanByBlock, RasterByBlock
	};
}