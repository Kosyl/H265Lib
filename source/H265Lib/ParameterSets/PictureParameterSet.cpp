#include "PictureParameterSet.h"
#include <Common/Calc.h>

namespace H265Lib
{
	PictureParameterSet::PictureParameterSet(UShort idx) :
		ParameterSetBase(idx)
	{
		initWithDefaults();
	}

	PictureParameterSet::~PictureParameterSet()
	{
	}

	Void PictureParameterSet::initWithDefaults()
	{
		setDependentSliceSegmentsEnabled(true);
		setOutputFlagPresent(true);

		setSignDataHidingEnabled(false);

		setCabacInitPresent(false);

		setConstrainedIntraPrediction(false);

		setTransformSkipEnabled(false);

		setInitQP(30);
		setQPOffsetForCb(0);
		setQpOffsetForCr(0);
		setSliceChromaQpOffsetPresent(false);

		setWeightedPredictionEnabled(true);
		setWeightedBiPredictionEnabled(true);

		setTransQuantBypassEnabled(false);

		setEntropyCodingSyncEnabled(false);

		setTilesEnabled(false);

		setTcOffset(0);
		setBetaOffset(0);
		setLoopFilterOnSlicesEnabled(false);
		setLoopFilterOnTilesEnabled(false);
		setDeblockingFilterControlPresent(false);
		setDeblockingFilterOverrideEnabled(false);
		setDeblockingFilterDisabled(false);

		setScalingListDataPresent(false);

		setParallelMergeLevel(1);

		setSliceSegmentHeaderExtensionPresent(false);
		setExtensionFlag(false);
	}

#pragma region ParameterSets

	std::shared_ptr<VideoParameterSet> PictureParameterSet::getVideoParameterSet()
	{
		return this->_videoParameterSet;
	}

	std::shared_ptr<SequenceParameterSet> PictureParameterSet::getSequenceParameterSet()
	{
		return this->_sequenceParameterSet;
	}

	Void PictureParameterSet::setSequenceParameterSet(std::shared_ptr<SequenceParameterSet> sps)
	{
		this->_sequenceParameterSet = sps;
		this->_videoParameterSet = sps->getVideoParameterSet();
	}

	Void PictureParameterSet::setVideoParameterSet(std::shared_ptr<VideoParameterSet> vps)
	{
		this->_videoParameterSet = vps;
	}

	Void PictureParameterSet::setSequenceParameterSetFromBank(UShort idx)
	{
		this->_sequenceParameterSet = SequenceParameterSetBank::instance().getSetByIdx(idx);
		this->_videoParameterSet = this->_sequenceParameterSet->getVideoParameterSet();
	}

#pragma endregion

#pragma region Slice info

	Void PictureParameterSet::setNumExtraSliceHeaderBits(UShort val)
	{
		_numExtraSliceHeaderBits = val;
	}

	UShort PictureParameterSet::getNumExtraSliceHeaderBits() const
	{
		return _numExtraSliceHeaderBits;
	}

	Void PictureParameterSet::setOutputFlagPresent(Bool val)
	{
		_outputFlagPresent = val;
	}

	Bool PictureParameterSet::getOutputFlagPresent() const
	{
		return _outputFlagPresent;
	}

	Void PictureParameterSet::setDependentSliceSegmentsEnabled(Bool val)
	{
		_dependentSliceSegmentsEnabled = val;
	}

	Bool PictureParameterSet::getDependentSliceSegmentsEnabled() const
	{
		return _dependentSliceSegmentsEnabled;
	}

#pragma endregion

#pragma region SDH

	Void PictureParameterSet::setSignDataHidingEnabled(Bool val)
	{
		_signDataHidingEnabled = val;
	}

	Bool PictureParameterSet::getSignDataHidingEnabled() const
	{
		return _signDataHidingEnabled;
	}

#pragma endregion

#pragma region CabacInitPresent

	Void PictureParameterSet::setCabacInitPresent(Bool val)
	{
		_cabacInitPresent = val;
	}

	Bool PictureParameterSet::getCabacInitPresent() const
	{
		return _cabacInitPresent;
	}

#pragma endregion

#pragma region Reference Lists

	Void PictureParameterSet::setNumRefIdxL1DefaultActive(UShort val)
	{
		_numRefIdxL1DefaultActive = val;
	}

	UShort PictureParameterSet::getNumRefIdxL1DefaultActive() const
	{
		return _numRefIdxL1DefaultActive;
	}

	Void PictureParameterSet::setNumRefIdxL0DefaultActive(UShort val)
	{
		_numRefIdxL0DefaultActive = val;
	}

	UShort PictureParameterSet::getNumRefIdxL0DefaultActive() const
	{
		return _numRefIdxL0DefaultActive;
	}

#pragma endregion

#pragma region Constrained Intra Prediction

	Void PictureParameterSet::setConstrainedIntraPrediction(Bool val)
	{
		_constrainedIntraPrediction = val;
	}

	Bool PictureParameterSet::getConstrainedIntraPrediction() const
	{
		return _constrainedIntraPrediction;
	}

#pragma endregion

#pragma region Transform Skip

	Void PictureParameterSet::setTransformSkipEnabled(Bool val)
	{
		_transformSkipEnabled = val;
	}

	Bool PictureParameterSet::getTransformSkipEnabled() const
	{
		return _transformSkipEnabled;
	}

#pragma endregion

#pragma region QP

	Void PictureParameterSet::setSliceChromaQpOffsetPresent(Bool val)
	{
		_sliceChromaQpOffsetPresent = val;
	}

	Bool PictureParameterSet::getSliceChromaQpOffsetPresent() const
	{
		return _sliceChromaQpOffsetPresent;
	}

	Void PictureParameterSet::setQPOffsetForCb(Short val)
	{
		_CbQPOffset = val;
		refreshChromaQP();
	}

	Short PictureParameterSet::getQPOffsetForCb() const
	{
		return _CbQPOffset;
	}

	Void PictureParameterSet::setQpOffsetForCr(Short val)
	{
		_CrQPOffset = val;
		refreshChromaQP();
	}

	Short PictureParameterSet::getQpOffsetForCr() const
	{
		return _CrQPOffset;
	}

	Void PictureParameterSet::setDiffCUQpDeltaDepth(UShort val)
	{
		_diffCUQpDeltaDepth = val;
	}

	UShort PictureParameterSet::getDiffCUQpDeltaDepth() const
	{
		return _diffCUQpDeltaDepth;
	}

	Void PictureParameterSet::setCuQPDeltaEnabled(Bool val)
	{
		_cuQPDeltaEnabled = val;
	}

	Bool PictureParameterSet::getCuQPDeltaEnabled() const
	{
		return _cuQPDeltaEnabled;
	}

	Void PictureParameterSet::setInitQP(UShort val)
	{
		_initQP = val;
		refreshChromaQP();
	}

	UShort PictureParameterSet::getQP(ImgComp comp /*= ImgComp::Luma*/) const
	{
		if (comp == ImgComp::Luma)
			return _initQP;
		else if (comp == ImgComp::Cb)
			return _initQPForCb;
		else
			return _initQPForCr;
	}

	Void PictureParameterSet::refreshChromaQP()
	{
		_initQPForCb = _initQP + _CbQPOffset;
		_initQPForCr = _initQP + _CrQPOffset;
	}

#pragma endregion

#pragma region Weighted Prediction

	Void PictureParameterSet::setWeightedBiPredictionEnabled(Bool val)
	{
		_weightedBiPredictionEnabled = val;
	}

	Bool PictureParameterSet::getWeightedBiPredictionEnabled() const
	{
		return _weightedBiPredictionEnabled;
	}

	Void PictureParameterSet::setWeightedPredictionEnabled(Bool val)
	{
		_weightedPredictionEnabled = val;
	}

	Bool PictureParameterSet::getWeightedPredictionEnabled() const
	{
		return _weightedPredictionEnabled;
	}

#pragma endregion

#pragma region Trans Quant Bypass

	Void PictureParameterSet::setTransQuantBypassEnabled(Bool val)
	{
		_transQuantBypassEnabled = val;
	}

	Bool PictureParameterSet::getTransQuantBypassEnabled() const
	{
		return _transQuantBypassEnabled;
	}

#pragma endregion

#pragma region Entropy Coding Sync

	Void PictureParameterSet::setEntropyCodingSyncEnabled(Bool val)
	{
		_entropyCodingSyncEnabled = val;
	}

	Bool PictureParameterSet::getEntropyCodingSyncEnabled() const
	{
		return _entropyCodingSyncEnabled;
	}

#pragma endregion

#pragma region Tiles

	Void PictureParameterSet::setTileRowHeights(std::vector<UShort> val)
	{
		_tileRowHeights = val;
	}

	std::vector<UShort> PictureParameterSet::getTileRowHeights() const
	{
		return _tileRowHeights;
	}

	Void PictureParameterSet::setTileColumnWidths(std::vector<UShort> val)
	{
		_tileColumnWidths = val;
	}

	std::vector<UShort> PictureParameterSet::getTileColumnWidths() const
	{
		return _tileColumnWidths;
	}

	Void PictureParameterSet::setUniformTileSpacingFlag(Bool val)
	{
		_uniformTileSpacingFlag = val;
	}

	Bool PictureParameterSet::getUniformTileSpacingFlag() const
	{
		return _uniformTileSpacingFlag;
	}

	Void PictureParameterSet::setNumTileRows(UShort val)
	{
		_numTileRows = val;
	}

	UShort PictureParameterSet::getNumTileRows() const
	{
		return _numTileRows;
	}

	Void PictureParameterSet::setNumTileColumns(UShort val)
	{
		_numTileColumns = val;
	}

	UShort PictureParameterSet::getNumTileColumns() const
	{
		return _numTileColumns;
	}

	Void PictureParameterSet::setTilesEnabled(Bool val)
	{
		_tilesEnabled = val;
	}

	Bool PictureParameterSet::getTilesEnabled() const
	{
		return _tilesEnabled;
	}

#pragma endregion

#pragma region Loop Filter

	Void PictureParameterSet::setTcOffset(Bool val)
	{
		_tcOffset = val;
	}

	Bool PictureParameterSet::getTcOffset() const
	{
		return _tcOffset;
	}

	Void PictureParameterSet::setBetaOffset(Bool val)
	{
		_betaOffset = val;
	}

	Bool PictureParameterSet::getBetaOffset() const
	{
		return _betaOffset;
	}

	Void PictureParameterSet::setDeblockingFilterDisabled(Bool val)
	{
		_deblockingFilterDisabled = val;
	}

	Bool PictureParameterSet::getDeblockingFilterDisabled() const
	{
		return _deblockingFilterDisabled;
	}

	Void PictureParameterSet::setDeblockingFilterOverrideEnabled(Bool val)
	{
		_deblockingFilterOverrideEnabled = val;
	}

	Bool PictureParameterSet::getDeblockingFilterOverrideEnabled() const
	{
		return _deblockingFilterOverrideEnabled;
	}

	Void PictureParameterSet::setDeblockingFilterControlPresent(Bool val)
	{
		_deblockingFilterControlPresent = val;
	}

	Bool PictureParameterSet::getDeblockingFilterControlPresent() const
	{
		return _deblockingFilterControlPresent;
	}

	Void PictureParameterSet::setLoopFilterOnSlicesEnabled(Bool val)
	{
		_loopFilterAcrossSlicesEnabled = val;
	}

	Bool PictureParameterSet::getLoopFilterOnSlicesEnabled() const
	{
		return _loopFilterAcrossSlicesEnabled;
	}

	Void PictureParameterSet::setLoopFilterOnTilesEnabled(Bool val)
	{
		_loopFilterAcrossTilesEnabled = val;
	}

	Bool PictureParameterSet::getLoopFilterOnTilesEnabled() const
	{
		return _loopFilterAcrossTilesEnabled;
	}

#pragma endregion

#pragma region Scaling Lists

	Void PictureParameterSet::setListsModificationPresent(Bool val)
	{
		_listsModificationPresent = val;
	}

	Bool PictureParameterSet::getListsModificationPresent() const
	{
		return _listsModificationPresent;
	}

	Void PictureParameterSet::setScalingListData(ScalingListData val)
	{
		_scalingListData = val;
	}

	H265Lib::ScalingListData PictureParameterSet::getScalingListData() const
	{
		return _scalingListData;
	}

	Void PictureParameterSet::setScalingListDataPresent(Bool val)
	{
		_scalingListDataPresent = val;
	}

	Bool PictureParameterSet::getScalingListDataPresent() const
	{
		return _scalingListDataPresent;
	}

#pragma endregion

#pragma region Parallel Merge

	Void PictureParameterSet::setParallelMergeLevel(UShort val)
	{
		_parallelMergeLevel = val;
		_log2ParallelMergeLevel = Calc::log2Int(val);
	}

	UShort PictureParameterSet::getParallelMergeLevel() const
	{
		return _parallelMergeLevel;
	}

	UShort PictureParameterSet::getLog2ParallelMergeLevel() const
	{
		return _log2ParallelMergeLevel;
	}

#pragma endregion

#pragma region Extensions

	Void PictureParameterSet::setExtensionFlag(Bool val)
	{
		_extensionFlag = val;
	}

	Bool PictureParameterSet::getExtensionFlag() const
	{
		return _extensionFlag;
	}

	Void PictureParameterSet::setSliceSegmentHeaderExtensionPresent(Bool val)
	{
		_sliceSegmentHeaderExtensionPresent = val;
	}

	Bool PictureParameterSet::getSliceSegmentHeaderExtensionPresent() const
	{
		return _sliceSegmentHeaderExtensionPresent;
	}

#pragma endregion

}