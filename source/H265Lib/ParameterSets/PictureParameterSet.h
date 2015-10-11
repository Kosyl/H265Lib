#pragma once
#include <Common\TypeDef.h>
#include <Common/Enums.h>
#include "ParameterSetBase.h"
#include "ScalingListData.h"
#include "ParameterBank.h"
#include "SequenceParameterSet.h"

namespace H265Lib
{
	class PictureParameterSet : public ParameterSetBase
	{
	private:

		std::shared_ptr<SequenceParameterSet> _sequenceParameterSet;
		std::shared_ptr<VideoParameterSet> _videoParameterSet;

		Bool _dependentSliceSegmentsEnabled;
		Bool _outputFlagPresent;
		UShort _numExtraSliceHeaderBits;

		Bool _signDataHidingEnabled;
		Bool _cabacInitPresent;

		UShort _numRefIdxL0DefaultActive;
		UShort _numRefIdxL1DefaultActive;

		Bool _constrainedIntraPrediction;
		Bool _transformSkipEnabled;

		UShort _initQP;

		Bool _cuQPDeltaEnabled;
		UShort _diffCUQpDeltaDepth;

		Short _CbQPOffset, _CrQPOffset;
		UShort _initQPForCb, _initQPForCr;
		Bool _sliceChromaQpOffsetPresent;

		Bool _weightedPredictionEnabled;
		Bool _weightedBiPredictionEnabled;

		Bool _transQuantBypassEnabled;

		Bool _entropyCodingSyncEnabled;

		Bool _tilesEnabled;
		UShort _numTileColumns;
		UShort _numTileRows;
		Bool _uniformTileSpacingFlag;
		std::vector<UShort> _tileColumnWidths;
		std::vector<UShort> _tileRowHeights;
		Bool _loopFilterAcrossTilesEnabled;

		Bool _loopFilterAcrossSlicesEnabled;

		Bool _deblockingFilterControlPresent;
		Bool _deblockingFilterOverrideEnabled;
		Bool _deblockingFilterDisabled;
		Bool _betaOffset;
		Bool _tcOffset;

		Bool _scalingListDataPresent;
		ScalingListData _scalingListData;

		Bool _listsModificationPresent;
		UShort _parallelMergeLevel, _log2ParallelMergeLevel;

		Bool _sliceSegmentHeaderExtensionPresent;

		Bool _extensionFlag;

		Void refreshChromaQP();

	public:

		PictureParameterSet() = delete;
		PictureParameterSet(UShort idx);

		virtual ~PictureParameterSet() override;
		virtual Void initWithDefaults() override;

		Void setSequenceParameterSetFromBank(UShort idx);
		Void setSequenceParameterSet(std::shared_ptr<SequenceParameterSet> sps);
		Void setVideoParameterSet(std::shared_ptr<VideoParameterSet> vps);
		std::shared_ptr<SequenceParameterSet> getSequenceParameterSet();
		std::shared_ptr<VideoParameterSet> getVideoParameterSet();

		Bool getDependentSliceSegmentsEnabled() const;
		Void setDependentSliceSegmentsEnabled(Bool val);
		Bool getOutputFlagPresent() const;
		Void setOutputFlagPresent(Bool val);
		UShort getNumExtraSliceHeaderBits() const;
		Void setNumExtraSliceHeaderBits(UShort val);

		Bool getSignDataHidingEnabled() const;
		Void setSignDataHidingEnabled(Bool val);
		Bool getCabacInitPresent() const;
		Void setCabacInitPresent(Bool val);

		UShort getNumRefIdxL0DefaultActive() const;
		Void setNumRefIdxL0DefaultActive(UShort val);
		UShort getNumRefIdxL1DefaultActive() const;
		Void setNumRefIdxL1DefaultActive(UShort val);

		Bool getConstrainedIntraPrediction() const;
		Void setConstrainedIntraPrediction(Bool val);
		Bool getTransformSkipEnabled() const;
		Void setTransformSkipEnabled(Bool val);

		UShort getQP(ImgComp comp = ImgComp::Luma) const;
		Void setInitQP(UShort val);

		Bool getCuQPDeltaEnabled() const;
		Void setCuQPDeltaEnabled(Bool val);
		UShort getDiffCUQpDeltaDepth() const;
		Void setDiffCUQpDeltaDepth(UShort val);

		Short getQpOffsetForCr() const;
		Void setQpOffsetForCr(Short val);
		Short getQPOffsetForCb() const;
		Void setQPOffsetForCb(Short val);

		Bool getSliceChromaQpOffsetPresent() const;
		Void setSliceChromaQpOffsetPresent(Bool val);

		Bool getWeightedPredictionEnabled() const;
		Void setWeightedPredictionEnabled(Bool val);
		Bool getWeightedBiPredictionEnabled() const;
		Void setWeightedBiPredictionEnabled(Bool val);

		Bool getTransQuantBypassEnabled() const;
		Void setTransQuantBypassEnabled(Bool val);

		Bool getEntropyCodingSyncEnabled() const;
		Void setEntropyCodingSyncEnabled(Bool val);

		Bool getTilesEnabled() const;
		Void setTilesEnabled(Bool val);
		UShort getNumTileColumns() const;
		Void setNumTileColumns(UShort val);
		UShort getNumTileRows() const;
		Void setNumTileRows(UShort val);
		Bool getUniformTileSpacingFlag() const;
		Void setUniformTileSpacingFlag(Bool val);

		std::vector<UShort> getTileColumnWidths() const;
		Void setTileColumnWidths(std::vector<UShort> val);
		std::vector<UShort> getTileRowHeights() const;
		Void setTileRowHeights(std::vector<UShort> val);

		Bool getLoopFilterOnTilesEnabled() const;

		Void setLoopFilterOnTilesEnabled(Bool val);

		Bool getLoopFilterOnSlicesEnabled() const;

		Void setLoopFilterOnSlicesEnabled(Bool val);

		Bool getDeblockingFilterControlPresent() const;
		Void setDeblockingFilterControlPresent(Bool val);
		Bool getDeblockingFilterOverrideEnabled() const;
		Void setDeblockingFilterOverrideEnabled(Bool val);
		Bool getDeblockingFilterDisabled() const;
		Void setDeblockingFilterDisabled(Bool val);
		Bool getBetaOffset() const;
		Void setBetaOffset(Bool val);
		Bool getTcOffset() const;
		Void setTcOffset(Bool val);

		Bool getScalingListDataPresent() const;
		Void setScalingListDataPresent(Bool val);
		ScalingListData getScalingListData() const;
		Void setScalingListData(ScalingListData val);

		Bool getListsModificationPresent() const;
		Void setListsModificationPresent(Bool val);
		UShort getLog2ParallelMergeLevel() const;
		UShort getParallelMergeLevel() const;
		Void setParallelMergeLevel(UShort val);

		Bool getSliceSegmentHeaderExtensionPresent() const;
		Void setSliceSegmentHeaderExtensionPresent(Bool val);
		Bool getExtensionFlag() const;

		Void setExtensionFlag(Bool val);
	};

	class PictureParameterSetBank : public ParameterBank < PictureParameterSet >
	{

	};
}
