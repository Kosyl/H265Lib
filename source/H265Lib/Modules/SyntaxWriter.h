#pragma once
#include <fstream>
#include <vector>
#include <memory>
#include <ParameterSets/VideoParameterSet.h>
#include <ParameterSets/SequenceParameterSet.h>
#include <ParameterSets/PictureParameterSet.h>
#include <Common/Bitset.h>

namespace HEVC
{
	class SyntaxWriter
	{
	protected:
		std::vector<uint8_t>& buffer;

		Bitset profileTierLevel(ProfileTierLevel& ptl, bool profilePresent, int maxNumSubLayers, int *out_len = nullptr);

	public:
		SyntaxWriter(std::vector<uint8_t>& targetBuffer);

		int writeVpsNalUnit(std::shared_ptr<VideoParameterSet> vps);
		int writeSpsNalUnit(std::shared_ptr<SequenceParameterSet> vps);
		int writePpsNalUnit(std::shared_ptr<PictureParameterSet> vps);

		int writeNalHeader(NalUnitType type, bool addStartZeroByte = false, uint8_t layerId = 0, uint8_t temporalId = 1);
		int addRbspTrailingBits(Bitset& content);
		void writeContentToBuffer(Bitset& content);
	};
}
