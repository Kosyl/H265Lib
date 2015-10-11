#include "VideoParameterSet.h"

namespace H265Lib
{
	void VideoParameterSet::initWithDefaults()
	{
		setMaxLayers(1);
		setMaxSublayers(0);
		setTemporalIdNestingFlag(false);
		setSubLayerOrderingInfoPresent(false);
		setMaxLayerId(0);
		setNumLayerSets(0);
		setVpsTimingInfoPresent(0);
		setNumUnitsInTick(0);
		setTimeScale(0);
		setPocProportionalToTiming(false);
		setNumTicksPocDiffOne(0);
		setNumHrdParameters(0);
		setExtensionFlag(false);
	}

	void VideoParameterSet::setExtensionFlag(Bool val)
	{
		_extensionFlag = val;
	}

	Bool VideoParameterSet::getExtensionFlag() const
	{
		return _extensionFlag;
	}

	void VideoParameterSet::setHrdParametersList(std::vector<HRDParameters> val)
	{
		_hrdParametersList = val;
	}

	const std::vector<HRDParameters>& VideoParameterSet::getHrdParametersList() const
	{
		return _hrdParametersList;
	}

	void VideoParameterSet::setCprmsPresentFlags(std::vector<Bool> val)
	{
		_cprmsPresentFlags = val;
	}

	const std::vector<Bool>& VideoParameterSet::getCprmsPresentFlags() const
	{
		return _cprmsPresentFlags;
	}

	void VideoParameterSet::setHrdLayerSetIdxList(std::vector<UShort> val)
	{
		_hrdLayerSetIdxList = val;
	}

	const std::vector<UShort>& VideoParameterSet::getHrdLayerSetIdxList() const
	{
		return _hrdLayerSetIdxList;
	}

	void VideoParameterSet::setNumHrdParameters(UInt val)
	{
		_numHrdParameters = val;
	}

	UInt VideoParameterSet::getNumHrdParameters() const
	{
		return _numHrdParameters;
	}

	void VideoParameterSet::setNumTicksPocDiffOne(UInt val)
	{
		_numTicksPocDiffOne = val;
	}

	UInt VideoParameterSet::getNumTicksPocDiffOne() const
	{
		return _numTicksPocDiffOne;
	}

	void VideoParameterSet::setPocProportionalToTiming(Bool val)
	{
		_pocProportionalToTiming = val;
	}

	Bool VideoParameterSet::getPocProportionalToTiming() const
	{
		return _pocProportionalToTiming;
	}

	void VideoParameterSet::setTimeScale(UInt val)
	{
		_timeScale = val;
	}

	UInt VideoParameterSet::getTimeScale() const
	{
		return _timeScale;
	}

	void VideoParameterSet::setNumUnitsInTick(UInt val)
	{
		_numUnitsInTick = val;
	}

	UInt VideoParameterSet::getNumUnitsInTick() const
	{
		return _numUnitsInTick;
	}

	void VideoParameterSet::setVpsTimingInfoPresent(Bool val)
	{
		_vpsTimingInfoPresent = val;
	}

	Bool VideoParameterSet::getVpsTimingInfoPresent() const
	{
		return _vpsTimingInfoPresent;
	}

	Matrix<Bool>& VideoParameterSet::getLayerIdIncludedFlags()
	{
		return *_layerIdIncludedFlags;
	}

	void VideoParameterSet::setNumLayerSets(UShort val)
	{
		_numLayerSets = val;
	}

	UShort VideoParameterSet::getNumLayerSets() const
	{
		return _numLayerSets;
	}

	void VideoParameterSet::setMaxLayerId(UShort val)
	{
		_maxLayerId = val;
	}

	UShort VideoParameterSet::getMaxLayerId() const
	{
		return _maxLayerId;
	}

	void VideoParameterSet::setSubLayerOrderingInfos(std::vector<SubLayerOrderingInfo> val)
	{
		_subLayerOrderingInfos = val;
	}

	const std::vector<SubLayerOrderingInfo>& VideoParameterSet::getSubLayerOrderingInfos() const
	{
		return _subLayerOrderingInfos;
	}

	void VideoParameterSet::setSubLayerOrderingInfoPresent(Bool val)
	{
		_subLayerOrderingInfoPresent = val;
	}

	Bool VideoParameterSet::getSubLayerOrderingInfoPresent() const
	{
		return _subLayerOrderingInfoPresent;
	}

	void VideoParameterSet::setProfileTierLevel(ProfileTierLevel val)
	{
		_profileTierLevel = val;
	}

	const ProfileTierLevel& VideoParameterSet::getProfileTierLevel() const
	{
		return _profileTierLevel;
	}

	void VideoParameterSet::setTemporalIdNestingFlag(Bool val)
	{
		_temporalIdNestingFlag = val;
	}

	Bool VideoParameterSet::getTemporalIdNestingFlag() const
	{
		return _temporalIdNestingFlag;
	}

	void VideoParameterSet::setMaxSublayers(UShort val)
	{
		_maxSublayers = val;
	}

	UShort VideoParameterSet::getMaxSublayers() const
	{
		return _maxSublayers;
	}

	void VideoParameterSet::setMaxLayers(UShort val)
	{
		_maxLayers = val;
	}

	UShort VideoParameterSet::getMaxLayers() const
	{
		return _maxLayers;
	}

	VideoParameterSet::VideoParameterSet(UShort idx) : ParameterSetBase(idx)
	{
		initWithDefaults();
	}

	VideoParameterSet::~VideoParameterSet()
	{

	}
}
