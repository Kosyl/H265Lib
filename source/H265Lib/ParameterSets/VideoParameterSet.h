#pragma once
#include "ParameterSetBase.h"
#include "Common/TypeDef.h"
#include <vector>
#include "ParameterBank.h"
#include "ProfileTierLevel.h"
#include "SubLayerOrderingInfo.h"
#include <Common/Matrix.h>
#include "HRDParameters.h"

namespace H265Lib
{
	class VideoParameterSet: public ParameterSetBase
	{
	private:
		UShort _maxLayers;
		UShort _maxSublayers;
		Bool _temporalIdNestingFlag;
		ProfileTierLevel _profileTierLevel;

		Bool _subLayerOrderingInfoPresent;
		std::vector<SubLayerOrderingInfo> _subLayerOrderingInfos;

		UShort _maxLayerId;
		UShort _numLayerSets;
		std::unique_ptr<Matrix<Bool>> _layerIdIncludedFlags;

		Bool _vpsTimingInfoPresent;
		UInt _numUnitsInTick;
		UInt _timeScale;
		Bool _pocProportionalToTiming;
		UInt _numTicksPocDiffOne;

		UInt _numHrdParameters;
		std::vector<UShort> _hrdLayerSetIdxList;
		std::vector<Bool> _cprmsPresentFlags;
		std::vector<HRDParameters> _hrdParametersList;

		Bool _extensionFlag;

	public:

		VideoParameterSet() = delete;
		VideoParameterSet(UShort idx);

		virtual ~VideoParameterSet() override;;

		UShort getMaxLayers() const;
		void setMaxLayers(UShort val);
		UShort getMaxSublayers() const;
		void setMaxSublayers(UShort val);
		Bool getTemporalIdNestingFlag() const;
		void setTemporalIdNestingFlag(Bool val);
		const ProfileTierLevel& getProfileTierLevel() const;
		void setProfileTierLevel(ProfileTierLevel val);

		Bool getSubLayerOrderingInfoPresent() const;
		void setSubLayerOrderingInfoPresent(Bool val);
		const std::vector<SubLayerOrderingInfo>& getSubLayerOrderingInfos() const;
		void setSubLayerOrderingInfos(std::vector<SubLayerOrderingInfo> val);

		UShort getMaxLayerId() const;
		void setMaxLayerId(UShort val);
		UShort getNumLayerSets() const;
		void setNumLayerSets(UShort val);
		Matrix<Bool>& getLayerIdIncludedFlags();

		Bool getVpsTimingInfoPresent() const;
		void setVpsTimingInfoPresent(Bool val);
		UInt getNumUnitsInTick() const;
		void setNumUnitsInTick(UInt val);
		UInt getTimeScale() const;
		void setTimeScale(UInt val);
		Bool getPocProportionalToTiming() const;
		void setPocProportionalToTiming(Bool val);
		UInt getNumTicksPocDiffOne() const;
		void setNumTicksPocDiffOne(UInt val);

		UInt getNumHrdParameters() const;
		void setNumHrdParameters(UInt val);

		const std::vector<UShort>& getHrdLayerSetIdxList() const;
		void setHrdLayerSetIdxList(std::vector<UShort> val);
		const std::vector<Bool>& getCprmsPresentFlags() const;
		void setCprmsPresentFlags(std::vector<Bool> val);
		const std::vector<HRDParameters>& getHrdParametersList() const;
		void setHrdParametersList(std::vector<HRDParameters> val);

		Bool getExtensionFlag() const;
		void setExtensionFlag(Bool val);

		void initWithDefaults() override;
	};

	class VideoParameterSetBank: public ParameterBank<VideoParameterSet>
	{

	};
}