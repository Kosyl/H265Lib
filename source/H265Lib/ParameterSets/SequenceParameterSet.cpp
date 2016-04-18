#include "SequenceParameterSet.h"
#include <Common/Matrix.h>
#include <Common/Calc.h>

namespace HEVC
{
	SequenceParameterSet::SequenceParameterSet(int idx) :
		ParameterSetBase(idx)
	{
		initWithDefaults();
	}

	SequenceParameterSet::~SequenceParameterSet()
	{
		initWithDefaults();
	}

	void SequenceParameterSet::configure(EncoderParameters configuration)
	{
		chroma_format_idc = configuration.subsampling;
		setPicSize(configuration.pic_width, configuration.pic_height);
	}

	void SequenceParameterSet::initWithDefaults()
	{
		max_sub_layers = 1;

		chroma_format_idc =SubsamplingFormat::Mode_420;
		separate_colour_plane_flag = false;

		setConformanceWindow(0, 0, 0, 0);
		conformance_window_flag = false;

		bit_depth_luma = 8;
		bit_depth_chroma = 8;

		max_pic_order_cnt =1;

		sub_layer_ordering_info_present_flag =false;

		max_luma_coding_block_size =64;
		min_luma_coding_block_size =8;
		max_luma_transform_block_size = 32;
		min_luma_transform_block_size = 4;
		setPicSize(0, 0);

		max_transform_hierarchy_depth_intra = 3;
		max_transform_hierarchy_depth_inter = 3;

		scaling_list_enabled_flag = false;
		scaling_list_data_present_flag = false;

		amp_enabled_flag = true;

		sample_adaptive_offset_enabled_flag = false;

		pcm_enabled_flag = false;

		temporal_mvp_enabled_flag = false;

		strong_intra_smoothing_enabled_flag = true;

		vui_parameters_present_flag = false;

		sps_extension_flag = false;

		refresh();
	}

#pragma region Indexes

	void SequenceParameterSet::resetZScanArray()
	{
		int y, x, m, p, i, tbX, tbY, ctbAddrRs;
		int minTUSize = min_luma_transform_block_size, CTUSize = log2_ctu_size;
		int PicWidthInCTUs = pic_width_in_ctus, PicHeightInCTUs = pic_height_in_ctus;

		if (minTUSize < 2 || CTUSize < 4 || PicHeightInCTUs <= 0 || PicHeightInCTUs <= 0)
			return;

		int yLimit = PicHeightInCTUs << (CTUSize - minTUSize);
		int xLimit = PicWidthInCTUs << (CTUSize - minTUSize);

		_zScanArray = std::make_shared<Matrix<size_t>>(xLimit, yLimit);

		for (y = 0; y < yLimit; ++y)
		{
			for (x = 0; x < xLimit; ++x)
			{
				tbX = (x << minTUSize) >> CTUSize;
				tbY = (y << minTUSize) >> CTUSize;
				ctbAddrRs = PicWidthInCTUs * tbY + tbX;
				_zScanArray->at(x, y) = ctbAddrRs << ((CTUSize - minTUSize) * 2);
				for (i = 0, p = 0; i < (CTUSize - minTUSize); i++)
				{
					m = 1 << i;
					p += (m & x ? m * m : 0) + (m & y ? 2 * m * m : 0);
				}
				_zScanArray->at(x, y) = _zScanArray->at(x, y) + p;
			}
		}
	}

	int SequenceParameterSet::getSmallestBlockRasterIdx(int x, int y) const
	{
		return  ((y / min_luma_transform_block_size) * getPicWidth() / min_luma_transform_block_size) + (x / min_luma_transform_block_size);
	}

	std::shared_ptr<Matrix<size_t>> SequenceParameterSet::getZScanArrayPtr()
	{
		return _zScanArray;
	}

	size_t SequenceParameterSet::getSmallestBlockZScanIdxByBlockPosition(size_t puX, size_t puY) const
	{
		return _zScanArray->at(puX, puY);
	}

	size_t SequenceParameterSet::getSmallestBlockZScanIdxByPixel(const size_t pixelX, const size_t pixelY) const
	{
		return getSmallestBlockZScanIdxByBlockPosition(pixelX >> log2_min_transform_block_size, pixelY >> log2_min_transform_block_size);
	}

	size_t SequenceParameterSet::calcZScanIdxOf4x4BlockIn64x64BlockByPixel(const int puX, const int puY)
	{
		int puXDivBy4 = puX >> log2_min_transform_block_size;
		int puYDivBy4 = puY >> log2_min_transform_block_size;
		int calcPuIdx = ((puXDivBy4 & 8) << 3) | ((puXDivBy4 & 4) << 2) | ((puXDivBy4 & 2) << 1) | (puXDivBy4 & 1);
		calcPuIdx |= ((puYDivBy4 & 8) << 4) | ((puYDivBy4 & 4) << 3) | ((puYDivBy4 & 2) << 2) | ((puYDivBy4 & 1) << 1);
		return calcPuIdx;
	}

	size_t SequenceParameterSet::calcIdx(const size_t x, const size_t y, const Indexing idxType) const
	{
		switch (idxType)
		{
		case Indexing::RasterByBlock:
			return getSmallestBlockRasterIdx(x, y);
		case Indexing::ZScanByBlock:
			return getSmallestBlockZScanIdxByBlockPosition(x, y);
		case Indexing::ZScanByPixel:
		default:
			return getSmallestBlockZScanIdxByPixel(x, y);
		}
	}

#pragma endregion

	void SequenceParameterSet::refresh()
	{
		log2_max_transform_block_size = Calc::log2(max_luma_transform_block_size);
		log2_min_transform_block_size = Calc::log2(min_luma_transform_block_size);
		log2_max_coding_block_size = Calc::log2(max_luma_coding_block_size);
		log2_min_coding_block_size = Calc::log2(min_luma_coding_block_size);
		log2_ctu_size = Calc::log2(max_luma_coding_block_size);
		ctu_size = max_luma_coding_block_size;

		log2_max_pic_order_cnt = Calc::log2(max_pic_order_cnt);
	
		refreshPicSizeInCTUs();
		resetZScanArray();
	}

#pragma region PicSize

	size_t SequenceParameterSet::getPicWidth(ImgComp plane) const
	{
		if (chroma_format_idc == SubsamplingFormat::Mode_444)
		{
			return pic_width_in_luma_samples;
		}
		else //422 albo 420
		{
			if (plane == ImgComp::Luma)
				return pic_width_in_luma_samples;
			else //chroma
				return pic_width_in_luma_samples / 2;
		}
	}

	size_t SequenceParameterSet::getPicHeight(ImgComp plane) const
	{
		if (chroma_format_idc == SubsamplingFormat::Mode_444 || chroma_format_idc == SubsamplingFormat::Mode_422)
		{
			return pic_height_in_luma_samples;
		}
		else //420
		{
			if (plane == ImgComp::Luma)
				return pic_height_in_luma_samples;
			else //chroma
				return pic_height_in_luma_samples / 2;
		}
	}

	void SequenceParameterSet::setPicSize(int width, int height)
	{
		pic_width_in_luma_samples = width;
		pic_height_in_luma_samples = height;

		refreshPicSizeInCTUs();
	}

	void SequenceParameterSet::refreshPicSizeInCTUs()
	{
		pic_width_in_ctus = (pic_width_in_luma_samples - 1) / ctu_size + 1;
		pic_height_in_ctus = (pic_height_in_luma_samples - 1) / ctu_size + 1;
	}

#pragma endregion

	void SequenceParameterSet::setConformanceWindow(int top, int bottom, int left, int right)
	{
		conf_win_bottom_offset = bottom;
		conf_win_left_offset = left;
		conf_win_right_offset = right;
		conf_win_top_offset = top;
	}

	int SequenceParameterSet::getBitDepth(ImgComp comp) const
	{
		return comp == ImgComp::Luma ? bit_depth_luma : bit_depth_chroma;
	}

	Sample SequenceParameterSet::getDefaultSampleValue(ImgComp comp)
	{
		if (comp == ImgComp::Luma)
			return 1 << (bit_depth_luma - 1);
		else
			return 1 << (bit_depth_chroma - 1);
	}

	Sample SequenceParameterSet::clip(ImgComp comp, Sample value)
	{
		int maxVal = comp == ImgComp::Luma ? 1 << bit_depth_luma : 1 << bit_depth_chroma;
		return Calc::clipToRange<Sample>(0, maxVal, value);
	}
}