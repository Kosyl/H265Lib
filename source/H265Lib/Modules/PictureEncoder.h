#pragma once

#include <Data/BlockBase.h>
#include <Data/Picture.h>
#include <Data/QuadTree.h>
#include <Data/ReferenceSamples.h>
#include <Data/CTU.h>
#include <Data/CU.h>

namespace HEVC
{
	struct CuContext
	{
		size_t pic_width, pic_height;
		size_t max_tb_size;
		Picture* picture;

		void initFromParameters(ParametersBundle params)
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

	class HardcodedPictureEncoder: public IPictureEncoder
	{
	public:

		HardcodedPictureEncoder(ParametersBundle parameter_sets, PictureCodingConfiguration *pic_coding_info);
		virtual ~HardcodedPictureEncoder() = default;

		virtual void encodePicture(Picture &pic) override;


	protected:
		ParametersBundle parameters;
    PictureCodingConfiguration *coding_info;

		void processQuadtree(CUQuadTree &tree, CuContext context, CuCodingConfiguration& mode);
		void processCu(CU &cu, CuContext context, CuCodingConfiguration& mode);
		void prepareTransformTree(TUQuadTree &tu, CuContext& context, TuCodingConfiguration& mode) const;

    void processTu(TU &tu);
	};
}