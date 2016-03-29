#include <Modules/SyntaxWriter.h>
#include "PrimitiveBinaryEncoder.h"
#include "ParameterSets/ProfileTierLevel.h"

#define write_u(x, len) PrimitiveBinaryEncoder::unsigned_fixed_length(x,len); writtenBits += len;
#define write_ue(x) PrimitiveBinaryEncoder::unsigned_exp_golomb(x,&written); writtenBits += written;

namespace HEVC
{
	Bitset SyntaxWriter::profileTierLevel(ProfileTierLevel& ptl, bool profilePresent, int maxNumSubLayers, int *out_len /*= nullptr*/)
	{
		int writtenBits = 0;
		Bitset content;

		if (profilePresent)
		{
			content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_profile_space,2); writtenBits += 2;;
			content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_tier_flag,1); writtenBits += 1;;
			content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_profile_idc,5); writtenBits += 5;;

			for (int i = 0; i < Constants::NUM_PROFILES; ++i)
			{
				content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_profile_compatibility_flag[i],1); writtenBits += 1;;
			}

			content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_progressive_source_flag,1); writtenBits += 1;;
			content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_interlaced_source_flag,1); writtenBits += 1;;
			content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_non_packed_constraint_flag,1); writtenBits += 1;;
			content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_frame_only_constraint_flag,1); writtenBits += 1;;

			//todo
			content += PrimitiveBinaryEncoder::unsigned_fixed_length(0,44); writtenBits += 44;;

			content += PrimitiveBinaryEncoder::unsigned_fixed_length(ptl.general_level_idc,8); writtenBits += 8;;
		}

		//na razie bez sublayers
		assert(maxNumSubLayers == 1);

		if (out_len != nullptr)
			*out_len = writtenBits;

		return content;
	}

	SyntaxWriter::SyntaxWriter(std::vector<uint8_t>& targetBuffer) :
		buffer(targetBuffer)
	{

	}

	int SyntaxWriter::writeVpsNalUnit(std::shared_ptr<VideoParameterSet> vps)
	{
		int writtenBits = 0;

		writtenBits += writeNalHeader(NalUnitType::VPS, true);

		int written;

		Bitset content;

		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->Id,4); writtenBits += 4;

		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->base_layer_internal_flag,1); writtenBits += 1;
		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->base_layer_available_flag,1); writtenBits += 1;
		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->max_layers - 1,6); writtenBits += 6;
		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->max_sub_layers - 1,3); writtenBits += 3;
		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->temporal_id_nesting_flag,1); writtenBits += 1;

		auto s = content.str();
		content += PrimitiveBinaryEncoder::unsigned_fixed_length(0xFFFF,16); writtenBits += 16;

		content += profileTierLevel(vps->profile_tier_level, true, vps->max_sub_layers, &written ); writtenBits += written;

		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->sub_layer_ordering_info_present_flag,1); writtenBits += 1;

		for (int i = 0; i < vps->max_sub_layers; ++i)
		{
			content += PrimitiveBinaryEncoder::unsigned_exp_golomb(vps->sub_layer_ordering_infos[i].max_dec_pic_buffering-1, &written); writtenBits += written;
			content += PrimitiveBinaryEncoder::unsigned_exp_golomb(vps->sub_layer_ordering_infos[i].max_num_reorder_pic, &written); writtenBits += written;
			content += PrimitiveBinaryEncoder::unsigned_exp_golomb(vps->sub_layer_ordering_infos[i].max_latency_increase+1, &written); writtenBits += written;
		}

		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->max_layer_id, 6); writtenBits += 6;
		content += PrimitiveBinaryEncoder::unsigned_exp_golomb(vps->num_layer_sets - 1, &written); writtenBits += written;

		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->timing_info_present_flag, 1); writtenBits += 1;
		assert(!vps->timing_info_present_flag);

		content += PrimitiveBinaryEncoder::unsigned_fixed_length(vps->extension_flag, 1); writtenBits += 1;
		assert(!vps->extension_flag);

		writtenBits += addRbspTrailingBits(content);

		writeContentToBuffer(content);

		return writtenBits >> 3;
	}

	int SyntaxWriter::writeSpsNalUnit(std::shared_ptr<SequenceParameterSet> vps)
	{
		int writtenBits = 0;

		writtenBits += writeNalHeader(NalUnitType::SPS);

		//todo

		return writtenBits;
	}

	int SyntaxWriter::writePpsNalUnit(std::shared_ptr<PictureParameterSet> vps)
	{
		int writtenBits = 0;

		writtenBits += writeNalHeader(NalUnitType::PPS);

		//todo

		return writtenBits;
	}

	int SyntaxWriter::writeNalHeader(NalUnitType type, bool addStartZeroByte /*= false*/, uint8_t layerId /*= 0*/, uint8_t temporalId /*= 1*/)
	{
		auto startSize = buffer.size();

		if (addStartZeroByte)
			buffer.push_back(0x00);

		buffer.push_back(0x00);
		buffer.push_back(0x00);
		buffer.push_back(0x01);

		buffer.push_back(static_cast<uint8_t>(type) << 1);

		uint8_t high = (layerId << 3) & 0b11111000;
		uint8_t low = temporalId & 0b00000111;
		buffer.push_back(high | low);

		return (buffer.size() - startSize) * 8;
	}

	int SyntaxWriter::addRbspTrailingBits(Bitset& content)
	{
		int writtenBits = 0;
		content += PrimitiveBinaryEncoder::unsigned_fixed_length(1, 1); writtenBits += 1;
		while(content.is_byte_aligned())
		{
			content.push_back(0);
			++writtenBits;
		}
		return writtenBits;
	}

	void SyntaxWriter::writeContentToBuffer(Bitset& content)
	{
		short zeroCount = 0;
		for (auto it = content.byte_cbegin(); it != content.byte_cend(); ++it)
		{
			const uint8_t v = (*it);
			if (zeroCount == 2 && v <= 3)
			{
				buffer.push_back(0b00000011);
				zeroCount = 0;
			}

			if (v == 0)
			{
				zeroCount++;
			}
			else
			{
				zeroCount = 0;
			}
			buffer.push_back( v );
		}
		
		if (zeroCount > 0)
		{
			buffer.push_back(0b00000011);
		}
	}
}
