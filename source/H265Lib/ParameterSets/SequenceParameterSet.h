#pragma once
#include <Common/TypeDef.h>
#include <Common/Enums.h>
#include <Common/Matrix.h>
#include <ParameterSets/ParameterBank.h>
#include "ParameterSetBase.h"
#include "ProfileTierLevel.h"
#include "SubLayerOrderingInfo.h"
#include "ScalingListData.h"
#include "ShortTermReferencePictureSet.h"
#include "VUIParameters.h"
#include "VideoParameterSet.h"

namespace H265Lib
{
	class SequenceParameterSet : public ParameterSetBase
	{
	private:

#pragma region Data

		std::shared_ptr<VideoParameterSet> _vps;

		UShort _maxSublayers;
		Bool _temporalIdNestingFlag;
		std::vector<ProfileTierLevel> _profileTierLevels;

		SubsamplingFormat _subsamplingFormat;
		Bool _separateColourPlane;

		UShort _picWidth, _picHeight;

		Bool _useConformanceWindow;
		UShort _conformanceWindowTop, _conformanceWindowBottom, _conformanceWindowLeft, _conformanceWindowRight;

		UShort _bitDepthLuma, _bitDepthChroma;

		UShort _maxPicOrderCount, _log2MaxPicOrderCount;

		Bool _subLayerOrderingInfoPresent;
		std::vector<SubLayerOrderingInfo> _subLayerOrderingInfos;

		UShort _log2CTUSize, _log2MaxCUSize, _log2MinCUSize;
		UShort _maxCUSize, _minCUSize; //def. 64 i 8
		UShort _maxTUSize, _minTUSize; // def. 32 i 4
		UShort _picWidthInCTUs, _picHeightInCTUs;

		UShort _log2MaxTUSize, _log2MinTUSize;
		UShort _maxTransformHierarchyDepthIntra, _maxTransformHierarchyDepthInter;

		Bool _scalingListEnabled;
		Bool _scalingListDataPresent;
		ScalingListData _scalingListData;

		Bool _AMPEnabled;
		Bool _SAOEnabled;

		Bool _PCMEnabled;
		UShort _PCMbitDepthLuma, _PCMbitDepthChroma;
		UShort _log2MinCUSizePCM, _log2MaxCUSizePCM;
		UShort _minCUSizePCM, _maxCUSizePCM;
		Bool _loopFilterDisabledForPCM;

		UShort _numShortTermReferencePictureSets;
		std::vector<ShortTermReferencePictureSet> _shortTermRefPicSets;

		Bool _longTermRefPicsPresent;
		UShort _numLongTermRefPics;
		std::vector<UShort> _longTermRefPicsPOCList;
		std::vector<Bool> _LTReferenceUsedByCurrentPicture;

		Bool _temporalMVPEnabled;
		Bool _strongIntraSmoothingEnabled;

		Bool _vuiParametersPresent;
		VUIParameters _vuiParameters;

		Bool _spsExtension;

		std::shared_ptr<Matrix<size_t> > _zScanArray;

#pragma endregion

		Void resetZScanArray();

	public:

		SequenceParameterSet() = delete;
		SequenceParameterSet(UShort idx);

		virtual ~SequenceParameterSet() override;

		size_t calcIdx(const size_t x, const size_t y, const Indexing idxType) const;
		size_t calcZScanIdxOf4x4BlockIn64x64BlockByPixel(const UInt pixelX, const UInt pixelY);
		size_t getSmallestBlockZScanIdxByPixel(const size_t pixelX, const size_t pixelY) const;
		size_t getSmallestBlockZScanIdxByBlockPosition(size_t blockX, size_t blockY) const;
		std::shared_ptr<Matrix<size_t>> getZScanArrayPtr();
		UInt getSmallestBlockRasterIdx(UInt x, UInt y) const;

		Void initWithDefaults() override;

#pragma region Data

		Void setVideoParameterSetFromBank(UShort idx);
		Void setVideoParameterSet(std::shared_ptr<VideoParameterSet> vps);
		std::shared_ptr<VideoParameterSet> getVideoParameterSet();

		UShort getMaxSublayers() const;
		Void setMaxSublayers(UShort val);
		Bool getTemporalIdNestingFlag() const;
		Void setTemporalIdNestingFlag(Bool val);
		const std::vector<ProfileTierLevel>& getProfileTierLevels() const;
		Void setProfileTierLevels(std::vector<ProfileTierLevel> val);

		SubsamplingFormat getSubsamplingFormat() const;
		Void setSubsamplingFormat(SubsamplingFormat val);
		Bool getSeparateColourPlane() const;
		Void setSeparateColourPlane(Bool val);

		UShort getPicWidth(ImgComp plane = ImgComp::Luma) const;
		UShort getPicHeight(ImgComp plane = ImgComp::Luma) const;
		Void setPicSize(UShort width, UShort height);
		Void refreshPicSizeInCTUs();

		Bool getUseConformanceWindow() const;
		Void setUseConformanceWindow(Bool val);
		UShort getConformanceWindowRightOffset() const;
		UShort getConformanceWindowLeftOffset() const;
		UShort getConformanceWindowTopOffset() const;
		UShort getConformanceWindowBottomOffset() const;
		Void setConformanceWindow(UShort top, UShort bottom, UShort left, UShort right);

		UShort getBitDepthChroma() const;
		Void setBitDepthChroma(UShort val);
		UShort getBitDepthLuma() const;
		Void setBitDepthLuma(UShort val);
		UShort getBitDepth(ImgComp comp) const;
		Sample clip(ImgComp comp, Int value);
		Sample getDefaultSampleValue(ImgComp comp);

		UShort getMaxPicOrderCount() const;
		UShort getLog2MaxPicOrderCount() const;
		Void setMaxPicOrderCount(UShort val);

		Bool getSubLayerOrderingInfoPresent() const;
		Void setSubLayerOrderingInfoPresent(Bool val);
		const std::vector<SubLayerOrderingInfo>& getSubLayerOrderingInfos() const;
		Void setSubLayerOrderingInfos(std::vector<SubLayerOrderingInfo> val);

		UShort getMinCUSize() const;
		Void setMinCUSize(UShort val);
		UShort getMaxCUSize() const;
		UShort getCTUSize() const;
		Void setMaxCUSize(UShort val);
		UShort getPicWidthInCTUs() const;
		UShort getPicHeightInCTUs() const;
		UShort getLog2MinCUSize() const;
		UShort getLog2MaxCUSize() const;
		UShort getLog2CTUSize() const;

		UShort getMinTUSize() const;
		Void setMinTUSize(UShort val);
		UShort getMaxTUSize() const;
		Void setMaxTUSize(UShort val);
		UShort getLog2MinTUSize() const;
		UShort getLog2MaxTUSize() const;

		UShort getMaxTransformHierarchyDepthInter() const;
		Void setMaxTransformHierarchyDepthInter(UShort val);
		UShort getMaxTransformHierarchyDepthIntra() const;
		Void setMaxTransformHierarchyDepthIntra(UShort val);

		Bool getScalingListEnabled() const;
		Void setScalingListEnabled(Bool val);
		Bool getScalingListDataPresent() const;
		Void setScalingListDataPresent(Bool val);
		const ScalingListData& getScalingListData() const;
		Void setScalingListData(ScalingListData val);

		Bool getAMPEnabled() const;
		Void setAMPEnabled(Bool val);
		Bool getSAOEnabled() const;
		Void setSAOEnabled(Bool val);

		Bool getLoopFilterDisabledForPCM() const;
		Void setLoopFilterDisabledForPCM(Bool val);
		Bool getPCMEnabled() const;
		Void setPCMEnabled(Bool val);
		UShort getPCMbitDepthChroma() const;
		Void setPCMbitDepthChroma(UShort val);
		UShort getPCMbitDepthLuma() const;
		Void setPCMbitDepthLuma(UShort val);
		UShort getLog2MaxCUSizePCM() const;
		UShort getLog2MinCUSizePCM() const;
		UShort getMaxCUSizePCM() const;
		Void setMaxCUSizePCM(UShort val);
		UShort getMinCUSizePCM() const;
		Void setMinCUSizePCM(UShort val);

		UShort getNumShortTermReferencePictureSets() const;
		Void setNumShortTermReferencePictureSets(UShort val);
		const std::vector<ShortTermReferencePictureSet>& getShortTermRefPicSets() const;
		Void setShortTermRefPicSets(std::vector<ShortTermReferencePictureSet> val);

		Bool getLongTermRefPicsPresent() const;
		Void setLongTermRefPicsPresent(Bool val);
		UShort getNumLongTermRefPics() const;
		Void setNumLongTermRefPics(UShort val);
		const std::vector<Bool>& getLTReferenceUsedByCurrentPicture() const;
		Void setLTReferenceUsedByCurrentPicture(std::vector<Bool> val);
		const std::vector<UShort>& getLongTermRefPicsPOCList() const;
		Void setLongTermRefPicsPOCList(std::vector<UShort> val);

		Bool getTemporalMVPEnabled() const;
		Void setTemporalMVPEnabled(Bool val);
		Bool getStrongIntraSmoothingEnabled() const;
		Void setStrongIntraSmoothingEnabled(Bool val);

		Bool getVuiParametersPresent() const;
		Void setVuiParametersPresent(Bool val);
		const VUIParameters& getVuiParameters() const;
		Void setVuiParameters(VUIParameters val);

		Bool getSpsExtension() const;
		Void setSpsExtension(Bool val);

#pragma endregion

	};

	class SequenceParameterSetBank : public ParameterBank < SequenceParameterSet >
	{

	};
}
