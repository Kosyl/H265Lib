#pragma once

#include <Data/BlockBase.h>
#include <Data/Picture.h>
#include <Data/QuadTree.h>
#include <Data/CTU.h>
#include <Data/CU.h>

namespace HEVC
{
	struct CuContext
	{
		std::vector<std::shared_ptr<CU>> neighbours;
		size_t pic_width, pic_height;
		size_t max_tb_size;

		void initFromparameters(ParametersBundle params)
		{
			pic_width = params.Sps->pic_width_in_luma_samples;
			pic_height = params.Sps->pic_height_in_luma_samples;
			max_tb_size = params.Sps->max_luma_transform_block_size;
		}
	};

	struct CtuContext
	{
		std::shared_ptr<CTU> upper, upper_right, left, lower_left, upper_left, lower;
		size_t pic_width, pic_height;
		size_t max_tb_size;

		void initFromparameters(ParametersBundle params)
		{
			pic_width = params.Sps->pic_width_in_luma_samples;
			pic_height = params.Sps->pic_height_in_luma_samples; 
			max_tb_size = params.Sps->max_luma_transform_block_size;
		}
	};

	interface IPictureEncoder
	{
		virtual ~IPictureEncoder() = default;

		virtual void encodePicture(Picture &pic_to_encode) = 0;
	};

	class IntraPictureEncoder: public IPictureEncoder
	{
	public:
		IntraPictureEncoder(ParametersBundle parameter_sets);
		virtual ~IntraPictureEncoder();

		virtual void encodePicture(Picture &pic) override;

	protected:
		ParametersBundle parameters;

		void processCTU(std::shared_ptr<CTU> ctu);
		void divideCUTreeIntoSmallestCUs(std::shared_ptr<CUQuadTree> subTree);
	};

	class HardcodedPictureEncoder: public IPictureEncoder
	{
	public:

		HardcodedPictureEncoder(ParametersBundle parameter_sets);
		virtual ~HardcodedPictureEncoder() = default;

		virtual void encodePicture(Picture &pic) override;

	protected:
		ParametersBundle parameters;

		void processQuadtree(CUQuadTree &tree, CtuContext context);
	};
}