#include "SequenceParameterSet.h"
#include <Common/Matrix.h>
#include <Common/Calc.h>

namespace H265Lib
{
	SequenceParameterSet::SequenceParameterSet(UShort idx) :
		ParameterSetBase(idx)
	{
		initWithDefaults();
	}

	SequenceParameterSet::~SequenceParameterSet()
	{
		initWithDefaults();
	}

	Void SequenceParameterSet::initWithDefaults()
	{
		setMaxSublayers(1);

		setSubsamplingFormat(SubsamplingFormat::Mode_420);
		setSeparateColourPlane(false);

		setConformanceWindow(0, 0, 0, 0);
		setUseConformanceWindow(false);

		setBitDepthLuma(8);
		setBitDepthChroma(8);

		setMaxPicOrderCount(1);

		setSubLayerOrderingInfoPresent(false);

		setMaxCUSize(64);
		setMinCUSize(8);
		setMaxTUSize(32);
		setMinTUSize(4);
		setPicSize(0, 0);

		setMaxTransformHierarchyDepthIntra(3);
		setMaxTransformHierarchyDepthInter(3);

		setScalingListEnabled(false);
		setScalingListDataPresent(false);

		setAMPEnabled(true);

		setSAOEnabled(true);

		setPCMEnabled(false);

		setTemporalMVPEnabled(true);

		setStrongIntraSmoothingEnabled(true);

		setVuiParametersPresent(false);

		setSpsExtension(false);
	}

#pragma region Indexes

	Void SequenceParameterSet::resetZScanArray()
	{
		UInt y, x, m, p, i, tbX, tbY, ctbAddrRs;
		UInt minTUSize = getLog2MinTUSize(), CTUSize = getLog2CTUSize();
		UInt PicWidthInCTUs = getPicWidthInCTUs(), PicHeightInCTUs = getPicHeightInCTUs();

		if (minTUSize < 2 || CTUSize < 4 || PicHeightInCTUs <= 0 || PicHeightInCTUs <= 0)
			return;

		UInt yLimit = PicHeightInCTUs << (CTUSize - minTUSize);
		UInt xLimit = PicWidthInCTUs << (CTUSize - minTUSize);

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

	UInt SequenceParameterSet::getSmallestBlockRasterIdx(UInt x, UInt y) const
	{
		return  ((y / getMinTUSize()) * getPicWidth() / getMinTUSize()) + (x / getMinTUSize());
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
		return getSmallestBlockZScanIdxByBlockPosition(pixelX >> getLog2MinTUSize(), pixelY >> getLog2MinTUSize());
	}

	size_t SequenceParameterSet::calcZScanIdxOf4x4BlockIn64x64BlockByPixel(const UInt puX, const UInt puY)
	{
		UInt puXDivBy4 = puX >> getLog2MinTUSize();
		UInt puYDivBy4 = puY >> getLog2MinTUSize();
		UInt calcPuIdx = ((puXDivBy4 & 8) << 3) | ((puXDivBy4 & 4) << 2) | ((puXDivBy4 & 2) << 1) | (puXDivBy4 & 1);
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

#pragma region VPS

	Void SequenceParameterSet::setVideoParameterSetFromBank(UShort idx)
	{
		this->_vps = VideoParameterSetBank::instance().getSetByIdx(idx);
	}

	Void SequenceParameterSet::setVideoParameterSet(std::shared_ptr<VideoParameterSet> vps)
	{
		this->_vps = vps;
	}

	std::shared_ptr<VideoParameterSet> SequenceParameterSet::getVideoParameterSet()
	{
		return this->_vps;
	}

#pragma endregion

#pragma region ProfileTierLevel, TamporalIdNesting, Sublayers

	void SequenceParameterSet::setProfileTierLevels(std::vector<ProfileTierLevel> val)
	{
		_profileTierLevels = val;
	}

	const std::vector<ProfileTierLevel>& SequenceParameterSet::getProfileTierLevels() const
	{
		return _profileTierLevels;
	}

	void SequenceParameterSet::setTemporalIdNestingFlag(Bool val)
	{
		_temporalIdNestingFlag = val;
	}

	Bool SequenceParameterSet::getTemporalIdNestingFlag() const
	{
		return _temporalIdNestingFlag;
	}

	void SequenceParameterSet::setMaxSublayers(UShort val)
	{
		_maxSublayers = val;
	}

	UShort SequenceParameterSet::getMaxSublayers() const
	{
		return _maxSublayers;
	}

#pragma endregion

#pragma region Subsampling

	void SequenceParameterSet::setSeparateColourPlane(Bool val)
	{
		_separateColourPlane = val;
	}

	Bool SequenceParameterSet::getSeparateColourPlane() const
	{
		return _separateColourPlane;
	}

	void SequenceParameterSet::setSubsamplingFormat(SubsamplingFormat val)
	{
		_subsamplingFormat = val;
	}

	H265Lib::SubsamplingFormat SequenceParameterSet::getSubsamplingFormat() const
	{
		return _subsamplingFormat;
	}

#pragma endregion

#pragma region PicSize

	Void SequenceParameterSet::setPicSize(UShort width, UShort height)
	{
		_picWidth = width;
		_picHeight = height;

		refreshPicSizeInCTUs();
		resetZScanArray();
	}

	UShort SequenceParameterSet::getPicWidth(ImgComp plane) const
	{
		if (this->getSubsamplingFormat() == SubsamplingFormat::Mode_444)
		{
			return _picWidth;
		}
		else //422 albo 420
		{
			if (plane == ImgComp::Luma)
				return _picWidth;
			else //chroma
				return _picWidth / 2;
		}
	}

	UShort SequenceParameterSet::getPicHeight(ImgComp plane) const
	{
		if (this->getSubsamplingFormat() == SubsamplingFormat::Mode_444 || this->getSubsamplingFormat() == SubsamplingFormat::Mode_422)
		{
			return _picHeight;
		}
		else //420
		{
			if (plane == ImgComp::Luma)
				return _picHeight;
			else //chroma
				return _picHeight / 2;
		}
	}

	void SequenceParameterSet::refreshPicSizeInCTUs()
	{
		_picWidthInCTUs = (_picWidth - 1) / _maxCUSize + 1;
		_picHeightInCTUs = (_picHeight - 1) / _maxCUSize + 1;
	}

#pragma endregion

#pragma region Conformance Window

	void SequenceParameterSet::setConformanceWindow(UShort top, UShort bottom, UShort left, UShort right)
	{
		_conformanceWindowBottom = bottom;
		_conformanceWindowLeft = left;
		_conformanceWindowRight = right;
		_conformanceWindowTop = top;
	}

	UShort SequenceParameterSet::getConformanceWindowBottomOffset() const
	{
		return _conformanceWindowBottom;
	}

	UShort SequenceParameterSet::getConformanceWindowTopOffset() const
	{
		return _conformanceWindowTop;
	}

	UShort SequenceParameterSet::getConformanceWindowLeftOffset() const
	{
		return _conformanceWindowLeft;
	}

	UShort SequenceParameterSet::getConformanceWindowRightOffset() const
	{
		return _conformanceWindowRight;
	}

	void SequenceParameterSet::setUseConformanceWindow(Bool val)
	{
		_useConformanceWindow = val;
	}

	Bool SequenceParameterSet::getUseConformanceWindow() const
	{
		return _useConformanceWindow;
	}

#pragma endregion

#pragma region Bit depth

	UShort SequenceParameterSet::getBitDepth(ImgComp comp) const
	{
		return comp == ImgComp::Luma ? _bitDepthLuma : _bitDepthChroma;
	}

	Void SequenceParameterSet::setBitDepthLuma(UShort val)
	{
		_bitDepthLuma = val;
	}

	UShort SequenceParameterSet::getBitDepthLuma() const
	{
		return _bitDepthLuma;
	}

	Void SequenceParameterSet::setBitDepthChroma(UShort val)
	{
		_bitDepthChroma = val;
	}

	UShort SequenceParameterSet::getBitDepthChroma() const
	{
		return _bitDepthChroma;
	}

	Sample SequenceParameterSet::getDefaultSampleValue(ImgComp comp)
	{
		if (comp == ImgComp::Luma)
			return 1 << (_bitDepthLuma - 1);
		else
			return 1 << (_bitDepthChroma - 1);
	}

	Sample SequenceParameterSet::clip(ImgComp comp, Int value)
	{
		UShort maxVal = comp == ImgComp::Luma ? 1 << _bitDepthLuma : 1 << _bitDepthChroma;
		return Calc::clipToRange<Sample>(0, maxVal, value);
	}

#pragma endregion

#pragma region MaxPicOrderCount

	void SequenceParameterSet::setMaxPicOrderCount(UShort val)
	{
		_maxPicOrderCount = val;
		_log2MaxPicOrderCount = Calc::log2Int(_maxPicOrderCount);
	}

	UShort SequenceParameterSet::getLog2MaxPicOrderCount() const
	{
		return _log2MaxPicOrderCount;
	}

	UShort SequenceParameterSet::getMaxPicOrderCount() const
	{
		return _maxPicOrderCount;
	}

#pragma endregion

#pragma region SublayerOrderingInfo

	void SequenceParameterSet::setSubLayerOrderingInfos(std::vector<SubLayerOrderingInfo> val)
	{
		_subLayerOrderingInfos = val;
	}

	const std::vector<SubLayerOrderingInfo>& SequenceParameterSet::getSubLayerOrderingInfos() const
	{
		return _subLayerOrderingInfos;
	}

	void SequenceParameterSet::setSubLayerOrderingInfoPresent(Bool val)
	{
		_subLayerOrderingInfoPresent = val;
	}

	Bool SequenceParameterSet::getSubLayerOrderingInfoPresent() const
	{
		return _subLayerOrderingInfoPresent;
	}

#pragma endregion

#pragma region CU Size

	UShort SequenceParameterSet::getCTUSize() const
	{
		return getMaxCUSize();
	}

	Void SequenceParameterSet::setMaxCUSize(UShort val)
	{
		_maxCUSize = val;
		_log2MaxCUSize = Calc::log2Int(val);
		_log2CTUSize = Calc::log2Int(val);
		refreshPicSizeInCTUs();
	}

	UShort SequenceParameterSet::getMaxCUSize() const
	{
		return _maxCUSize;
	}

	UShort SequenceParameterSet::getPicWidthInCTUs() const
	{
		return _picWidthInCTUs;
	}

	UShort SequenceParameterSet::getPicHeightInCTUs() const
	{
		return _picHeightInCTUs;
	}

	Void SequenceParameterSet::setMinCUSize(UShort val)
	{
		_minCUSize = val;
		_log2MinCUSize = Calc::log2Int(val);
	}

	UShort SequenceParameterSet::getMinCUSize() const
	{
		return _minCUSize;
	}

	UShort SequenceParameterSet::getLog2CTUSize() const
	{
		return _log2CTUSize;
	}

	UShort SequenceParameterSet::getLog2MaxCUSize() const
	{
		return _log2MaxCUSize;
	}

	UShort SequenceParameterSet::getLog2MinCUSize() const
	{
		return _log2MinCUSize;
	}

#pragma endregion

#pragma region TU Size

	Void SequenceParameterSet::setMaxTUSize(UShort val)
	{
		_maxTUSize = val;
		_log2MaxTUSize = Calc::log2Int(val);
	}

	UShort SequenceParameterSet::getMaxTUSize() const
	{
		return _maxTUSize;
	}

	Void SequenceParameterSet::setMinTUSize(UShort val)
	{
		_minTUSize = val;
		_log2MinTUSize = Calc::log2Int(val);
	}

	UShort SequenceParameterSet::getMinTUSize() const
	{
		return _minTUSize;
	}

	UShort SequenceParameterSet::getLog2MaxTUSize() const
	{
		return _log2MaxTUSize;
	}

	UShort SequenceParameterSet::getLog2MinTUSize() const
	{
		return _log2MinTUSize;
	}

#pragma endregion

#pragma region Max Transform Hierarchy Depth

	void SequenceParameterSet::setMaxTransformHierarchyDepthIntra(UShort val)
	{
		_maxTransformHierarchyDepthIntra = val;
	}

	UShort SequenceParameterSet::getMaxTransformHierarchyDepthIntra() const
	{
		return _maxTransformHierarchyDepthIntra;
	}

	void SequenceParameterSet::setMaxTransformHierarchyDepthInter(UShort val)
	{
		_maxTransformHierarchyDepthInter = val;
	}

	UShort SequenceParameterSet::getMaxTransformHierarchyDepthInter() const
	{
		return _maxTransformHierarchyDepthInter;
	}

#pragma endregion

#pragma region Scaling List

	void SequenceParameterSet::setScalingListData(ScalingListData val)
	{
		_scalingListData = val;
	}

	const ScalingListData& SequenceParameterSet::getScalingListData() const
	{
		return _scalingListData;
	}

	void SequenceParameterSet::setScalingListDataPresent(Bool val)
	{
		_scalingListDataPresent = val;
	}

	Bool SequenceParameterSet::getScalingListDataPresent() const
	{
		return _scalingListDataPresent;
	}

	void SequenceParameterSet::setScalingListEnabled(Bool val)
	{
		_scalingListEnabled = val;
	}

	Bool SequenceParameterSet::getScalingListEnabled() const
	{
		return _scalingListEnabled;
	}

#pragma endregion

#pragma region AMP

	void SequenceParameterSet::setAMPEnabled(Bool val)
	{
		_AMPEnabled = val;
	}

	Bool SequenceParameterSet::getAMPEnabled() const
	{
		return _AMPEnabled;
	}

#pragma endregion

#pragma region SAO

	Void SequenceParameterSet::setSAOEnabled(Bool val)
	{
		_SAOEnabled = val;
	}

	Bool SequenceParameterSet::getSAOEnabled() const
	{
		return _SAOEnabled;
	}

#pragma endregion

#pragma region PCM

	void SequenceParameterSet::setMinCUSizePCM(UShort val)
	{
		_minCUSizePCM = val;
	}

	UShort SequenceParameterSet::getMinCUSizePCM() const
	{
		return _minCUSizePCM;
	}

	void SequenceParameterSet::setMaxCUSizePCM(UShort val)
	{
		_maxCUSizePCM = val;
	}

	UShort SequenceParameterSet::getMaxCUSizePCM() const
	{
		return _maxCUSizePCM;
	}

	UShort SequenceParameterSet::getLog2MinCUSizePCM() const
	{
		return _log2MinCUSizePCM;
	}

	UShort SequenceParameterSet::getLog2MaxCUSizePCM() const
	{
		return _log2MaxCUSizePCM;
	}

	void SequenceParameterSet::setPCMbitDepthLuma(UShort val)
	{
		_PCMbitDepthLuma = val;
	}

	UShort SequenceParameterSet::getPCMbitDepthLuma() const
	{
		return _PCMbitDepthLuma;
	}

	void SequenceParameterSet::setPCMbitDepthChroma(UShort val)
	{
		_PCMbitDepthChroma = val;
	}

	UShort SequenceParameterSet::getPCMbitDepthChroma() const
	{
		return _PCMbitDepthChroma;
	}

	void SequenceParameterSet::setPCMEnabled(Bool val)
	{
		_PCMEnabled = val;
	}

	Bool SequenceParameterSet::getPCMEnabled() const
	{
		return _PCMEnabled;
	}

	void SequenceParameterSet::setLoopFilterDisabledForPCM(Bool val)
	{
		_loopFilterDisabledForPCM = val;
	}

	Bool SequenceParameterSet::getLoopFilterDisabledForPCM() const
	{
		return _loopFilterDisabledForPCM;
	}

#pragma endregion

#pragma region Reference pics

	void SequenceParameterSet::setLongTermRefPicsPOCList(std::vector<UShort> val)
	{
		_longTermRefPicsPOCList = val;
	}

	const std::vector<UShort>& SequenceParameterSet::getLongTermRefPicsPOCList() const
	{
		return _longTermRefPicsPOCList;
	}

	void SequenceParameterSet::setLTReferenceUsedByCurrentPicture(std::vector<Bool> val)
	{
		_LTReferenceUsedByCurrentPicture = val;
	}

	const std::vector<Bool>& SequenceParameterSet::getLTReferenceUsedByCurrentPicture() const
	{
		return _LTReferenceUsedByCurrentPicture;
	}

	void SequenceParameterSet::setNumLongTermRefPics(UShort val)
	{
		_numLongTermRefPics = val;
	}

	UShort SequenceParameterSet::getNumLongTermRefPics() const
	{
		return _numLongTermRefPics;
	}

	void SequenceParameterSet::setLongTermRefPicsPresent(Bool val)
	{
		_longTermRefPicsPresent = val;
	}

	Bool SequenceParameterSet::getLongTermRefPicsPresent() const
	{
		return _longTermRefPicsPresent;
	}

	void SequenceParameterSet::setShortTermRefPicSets(std::vector<ShortTermReferencePictureSet> val)
	{
		_shortTermRefPicSets = val;
	}

	const std::vector<ShortTermReferencePictureSet>& SequenceParameterSet::getShortTermRefPicSets() const
	{
		return _shortTermRefPicSets;
	}

	void SequenceParameterSet::setNumShortTermReferencePictureSets(UShort val)
	{
		_numShortTermReferencePictureSets = val;
	}

	UShort SequenceParameterSet::getNumShortTermReferencePictureSets() const
	{
		return _numShortTermReferencePictureSets;
	}

#pragma endregion

#pragma region TemporalMVP

	void SequenceParameterSet::setTemporalMVPEnabled(Bool val)
	{
		_temporalMVPEnabled = val;
	}

	Bool SequenceParameterSet::getTemporalMVPEnabled() const
	{
		return _temporalMVPEnabled;
	}

#pragma endregion

#pragma region Intra smoothing

	Void SequenceParameterSet::setStrongIntraSmoothingEnabled(Bool val)
	{
		_strongIntraSmoothingEnabled = val;
	}

	Bool SequenceParameterSet::getStrongIntraSmoothingEnabled() const
	{
		return _strongIntraSmoothingEnabled;
	}

#pragma endregion

#pragma region VUI

	void SequenceParameterSet::setVuiParameters(VUIParameters val)
	{
		_vuiParameters = val;
	}

	const VUIParameters& SequenceParameterSet::getVuiParameters() const
	{
		return _vuiParameters;
	}

	void SequenceParameterSet::setVuiParametersPresent(Bool val)
	{
		_vuiParametersPresent = val;
	}

	Bool SequenceParameterSet::getVuiParametersPresent() const
	{
		return _vuiParametersPresent;
	}

#pragma endregion

#pragma region Extensions

	void SequenceParameterSet::setSpsExtension(Bool val)
	{
		_spsExtension = val;
	}

	Bool SequenceParameterSet::getSpsExtension() const
	{
		return _spsExtension;
	}

#pragma endregion

}