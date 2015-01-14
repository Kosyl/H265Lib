/* The copyright in this software is being made available under the BSD
* License, included below. This software may be subject to other third party
* and contributor rights, including patent rights, and no such rights are
* granted under this license.
*
* Copyright (c) 2010-2013, ITU/ISO/IEC
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*  * Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*/

/** \file     TEncSbac.cpp
\brief    SBAC encoder class
*/

#include "TEncTop.h"
#include "TEncSbac.h"
#include "LoggingControl.h"

#include <map>
#include <bitset>
#include <algorithm>
#include <iomanip>

//! \ingroup TLibEncoder
//! \{

// ====================================================================================================================
// Constructor / destructor / create / destroy
// ====================================================================================================================

TEncSbac::TEncSbac()
// new structure here
: m_pcBitIf(NULL)
, m_pcSlice(NULL)
, m_pcBinIf(NULL)
, m_uiCoeffCost(0)
, m_numContextModels(0)
, m_cCUSplitFlagSCModel(1, 1, NUM_SPLIT_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUSkipFlagSCModel(1, 1, NUM_SKIP_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUMergeFlagExtSCModel(1, 1, NUM_MERGE_FLAG_EXT_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUMergeIdxExtSCModel(1, 1, NUM_MERGE_IDX_EXT_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUPartSizeSCModel(1, 1, NUM_PART_SIZE_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUPredModeSCModel(1, 1, NUM_PRED_MODE_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUIntraPredSCModel(1, 1, NUM_ADI_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUChromaPredSCModel(1, 1, NUM_CHROMA_PRED_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUDeltaQpSCModel(1, 1, NUM_DELTA_QP_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUInterDirSCModel(1, 1, NUM_INTER_DIR_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCURefPicSCModel(1, 1, NUM_REF_NO_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUMvdSCModel(1, 1, NUM_MV_RES_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUQtCbfSCModel(1, 2, NUM_QT_CBF_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUTransSubdivFlagSCModel(1, 1, NUM_TRANS_SUBDIV_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUQtRootCbfSCModel(1, 1, NUM_QT_ROOT_CBF_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUSigCoeffGroupSCModel(1, 2, NUM_SIG_CG_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUSigSCModel(1, 1, NUM_SIG_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCuCtxLastX(1, 2, NUM_CTX_LAST_FLAG_XY, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCuCtxLastY(1, 2, NUM_CTX_LAST_FLAG_XY, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUOneSCModel(1, 1, NUM_ONE_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUAbsSCModel(1, 1, NUM_ABS_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cMVPIdxSCModel(1, 1, NUM_MVP_IDX_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cCUAMPSCModel(1, 1, NUM_CU_AMP_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cSaoMergeSCModel(1, 1, NUM_SAO_MERGE_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cSaoTypeIdxSCModel(1, 1, NUM_SAO_TYPE_IDX_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_cTransformSkipSCModel(1, 2, NUM_TRANSFORMSKIP_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
, m_CUTransquantBypassFlagSCModel(1, 1, NUM_CU_TRANSQUANT_BYPASS_FLAG_CTX, m_contextModels + m_numContextModels, m_numContextModels)
{
	assert(m_numContextModels <= MAX_NUM_CTX_MOD);
}

TEncSbac::~TEncSbac()
{
}

// ====================================================================================================================
// Public member functions
// ====================================================================================================================

Void TEncSbac::resetEntropy()
{
	Int  iQp = m_pcSlice->getSliceQp();
	SliceType eSliceType = m_pcSlice->getSliceType();

	Int  encCABACTableIdx = m_pcSlice->getPPS()->getEncCABACTableIdx();
	if (!m_pcSlice->isIntra() && (encCABACTableIdx == B_SLICE || encCABACTableIdx == P_SLICE) && m_pcSlice->getPPS()->getCabacInitPresentFlag())
	{
		eSliceType = (SliceType)encCABACTableIdx;
	}

	m_cCUSplitFlagSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SPLIT_FLAG);

	m_cCUSkipFlagSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SKIP_FLAG);
	m_cCUMergeFlagExtSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_MERGE_FLAG_EXT);
	m_cCUMergeIdxExtSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_MERGE_IDX_EXT);
	m_cCUPartSizeSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_PART_SIZE);
	m_cCUAMPSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_CU_AMP_POS);
	m_cCUPredModeSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_PRED_MODE);
	m_cCUIntraPredSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_INTRA_PRED_MODE);
	m_cCUChromaPredSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_CHROMA_PRED_MODE);
	m_cCUInterDirSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_INTER_DIR);
	m_cCUMvdSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_MVD);
	m_cCURefPicSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_REF_PIC);
	m_cCUDeltaQpSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_DQP);
	m_cCUQtCbfSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_QT_CBF);
	m_cCUQtRootCbfSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_QT_ROOT_CBF);
	m_cCUSigCoeffGroupSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SIG_CG_FLAG);
	m_cCUSigSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SIG_FLAG);
	m_cCuCtxLastX.initBuffer(eSliceType, iQp, (UChar*)INIT_LAST);
	m_cCuCtxLastY.initBuffer(eSliceType, iQp, (UChar*)INIT_LAST);
	m_cCUOneSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_ONE_FLAG);
	m_cCUAbsSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_ABS_FLAG);
	m_cMVPIdxSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_MVP_IDX);
	m_cCUTransSubdivFlagSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_TRANS_SUBDIV_FLAG);
	m_cSaoMergeSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SAO_MERGE_FLAG);
	m_cSaoTypeIdxSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SAO_TYPE_IDX);
	m_cTransformSkipSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_TRANSFORMSKIP_FLAG);
	m_CUTransquantBypassFlagSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_CU_TRANSQUANT_BYPASS_FLAG);
	// new structure
	m_uiLastQp = iQp;

	m_pcBinIf->start();

	return;
}

/** The function does the following:
* If current slice type is P/B then it determines the distance of initialisation type 1 and 2 from the current CABAC states and
* stores the index of the closest table.  This index is used for the next P/B slice when cabac_init_present_flag is true.
*/
Void TEncSbac::determineCabacInitIdx()
{
	Int  qp = m_pcSlice->getSliceQp();

	if (!m_pcSlice->isIntra())
	{
		SliceType aSliceTypeChoices[] = { B_SLICE, P_SLICE };

		UInt bestCost = MAX_UINT;
		SliceType bestSliceType = aSliceTypeChoices[0];
		for (UInt idx = 0; idx < 2; idx++)
		{
			UInt curCost = 0;
			SliceType curSliceType = aSliceTypeChoices[idx];

			curCost = m_cCUSplitFlagSCModel.calcCost(curSliceType, qp, (UChar*)INIT_SPLIT_FLAG);
			curCost += m_cCUSkipFlagSCModel.calcCost(curSliceType, qp, (UChar*)INIT_SKIP_FLAG);
			curCost += m_cCUMergeFlagExtSCModel.calcCost(curSliceType, qp, (UChar*)INIT_MERGE_FLAG_EXT);
			curCost += m_cCUMergeIdxExtSCModel.calcCost(curSliceType, qp, (UChar*)INIT_MERGE_IDX_EXT);
			curCost += m_cCUPartSizeSCModel.calcCost(curSliceType, qp, (UChar*)INIT_PART_SIZE);
			curCost += m_cCUAMPSCModel.calcCost(curSliceType, qp, (UChar*)INIT_CU_AMP_POS);
			curCost += m_cCUPredModeSCModel.calcCost(curSliceType, qp, (UChar*)INIT_PRED_MODE);
			curCost += m_cCUIntraPredSCModel.calcCost(curSliceType, qp, (UChar*)INIT_INTRA_PRED_MODE);
			curCost += m_cCUChromaPredSCModel.calcCost(curSliceType, qp, (UChar*)INIT_CHROMA_PRED_MODE);
			curCost += m_cCUInterDirSCModel.calcCost(curSliceType, qp, (UChar*)INIT_INTER_DIR);
			curCost += m_cCUMvdSCModel.calcCost(curSliceType, qp, (UChar*)INIT_MVD);
			curCost += m_cCURefPicSCModel.calcCost(curSliceType, qp, (UChar*)INIT_REF_PIC);
			curCost += m_cCUDeltaQpSCModel.calcCost(curSliceType, qp, (UChar*)INIT_DQP);
			curCost += m_cCUQtCbfSCModel.calcCost(curSliceType, qp, (UChar*)INIT_QT_CBF);
			curCost += m_cCUQtRootCbfSCModel.calcCost(curSliceType, qp, (UChar*)INIT_QT_ROOT_CBF);
			curCost += m_cCUSigCoeffGroupSCModel.calcCost(curSliceType, qp, (UChar*)INIT_SIG_CG_FLAG);
			curCost += m_cCUSigSCModel.calcCost(curSliceType, qp, (UChar*)INIT_SIG_FLAG);
			curCost += m_cCuCtxLastX.calcCost(curSliceType, qp, (UChar*)INIT_LAST);
			curCost += m_cCuCtxLastY.calcCost(curSliceType, qp, (UChar*)INIT_LAST);
			curCost += m_cCUOneSCModel.calcCost(curSliceType, qp, (UChar*)INIT_ONE_FLAG);
			curCost += m_cCUAbsSCModel.calcCost(curSliceType, qp, (UChar*)INIT_ABS_FLAG);
			curCost += m_cMVPIdxSCModel.calcCost(curSliceType, qp, (UChar*)INIT_MVP_IDX);
			curCost += m_cCUTransSubdivFlagSCModel.calcCost(curSliceType, qp, (UChar*)INIT_TRANS_SUBDIV_FLAG);
			curCost += m_cSaoMergeSCModel.calcCost(curSliceType, qp, (UChar*)INIT_SAO_MERGE_FLAG);
			curCost += m_cSaoTypeIdxSCModel.calcCost(curSliceType, qp, (UChar*)INIT_SAO_TYPE_IDX);
			curCost += m_cTransformSkipSCModel.calcCost(curSliceType, qp, (UChar*)INIT_TRANSFORMSKIP_FLAG);
			curCost += m_CUTransquantBypassFlagSCModel.calcCost(curSliceType, qp, (UChar*)INIT_CU_TRANSQUANT_BYPASS_FLAG);
			if (curCost < bestCost)
			{
				bestSliceType = curSliceType;
				bestCost = curCost;
			}
		}
		m_pcSlice->getPPS()->setEncCABACTableIdx(bestSliceType);
	}
	else
	{
		m_pcSlice->getPPS()->setEncCABACTableIdx(I_SLICE);
	}
}


/** The function does the followng: Write out terminate bit. Flush CABAC. Intialize CABAC states. Start CABAC.
*/
Void TEncSbac::updateContextTables(SliceType eSliceType, Int iQp, Bool bExecuteFinish)
{
	m_pcBinIf->encodeBinTrm(1);
	if (bExecuteFinish) m_pcBinIf->finish();
	m_cCUSplitFlagSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SPLIT_FLAG);

	m_cCUSkipFlagSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SKIP_FLAG);
	m_cCUMergeFlagExtSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_MERGE_FLAG_EXT);
	m_cCUMergeIdxExtSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_MERGE_IDX_EXT);
	m_cCUPartSizeSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_PART_SIZE);
	m_cCUAMPSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_CU_AMP_POS);
	m_cCUPredModeSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_PRED_MODE);
	m_cCUIntraPredSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_INTRA_PRED_MODE);
	m_cCUChromaPredSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_CHROMA_PRED_MODE);
	m_cCUInterDirSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_INTER_DIR);
	m_cCUMvdSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_MVD);
	m_cCURefPicSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_REF_PIC);
	m_cCUDeltaQpSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_DQP);
	m_cCUQtCbfSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_QT_CBF);
	m_cCUQtRootCbfSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_QT_ROOT_CBF);
	m_cCUSigCoeffGroupSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SIG_CG_FLAG);
	m_cCUSigSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SIG_FLAG);
	m_cCuCtxLastX.initBuffer(eSliceType, iQp, (UChar*)INIT_LAST);
	m_cCuCtxLastY.initBuffer(eSliceType, iQp, (UChar*)INIT_LAST);
	m_cCUOneSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_ONE_FLAG);
	m_cCUAbsSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_ABS_FLAG);
	m_cMVPIdxSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_MVP_IDX);
	m_cCUTransSubdivFlagSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_TRANS_SUBDIV_FLAG);
	m_cSaoMergeSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SAO_MERGE_FLAG);
	m_cSaoTypeIdxSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_SAO_TYPE_IDX);
	m_cTransformSkipSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_TRANSFORMSKIP_FLAG);
	m_CUTransquantBypassFlagSCModel.initBuffer(eSliceType, iQp, (UChar*)INIT_CU_TRANSQUANT_BYPASS_FLAG);
	m_pcBinIf->start();
}

Void TEncSbac::codeVPS(TComVPS* pcVPS)
{
	assert(0);
	return;
}

Void TEncSbac::codeSPS(TComSPS* pcSPS)
{
	assert(0);
	return;
}

Void TEncSbac::codePPS(TComPPS* pcPPS)
{
	assert(0);
	return;
}

Void TEncSbac::codeSliceHeader(TComSlice* pcSlice)
{
	assert(0);
	return;
}

Void TEncSbac::codeTilesWPPEntryPoint(TComSlice* pSlice)
{
	assert(0);
	return;
}

Void TEncSbac::codeTerminatingBit(UInt uilsLast)
{
	m_pcBinIf->encodeBinTrm(uilsLast);
}

Void TEncSbac::codeSliceFinish()
{
	m_pcBinIf->finish();
}

Void TEncSbac::xWriteUnarySymbol(UInt uiSymbol, ContextModel* pcSCModel, Int iOffset)
{
	m_pcBinIf->encodeBin(uiSymbol ? 1 : 0, pcSCModel[0]);

	if (0 == uiSymbol)
	{
		return;
	}

	while (uiSymbol--)
	{
		m_pcBinIf->encodeBin(uiSymbol ? 1 : 0, pcSCModel[iOffset]);
	}

	return;
}

Void TEncSbac::xWriteUnaryMaxSymbol(UInt uiSymbol, ContextModel* pcSCModel, Int iOffset, UInt uiMaxSymbol)
{
	if (uiMaxSymbol == 0)
	{
		return;
	}

	m_pcBinIf->encodeBin(uiSymbol ? 1 : 0, pcSCModel[0]);

	if (uiSymbol == 0)
	{
		return;
	}

	Bool bCodeLast = (uiMaxSymbol > uiSymbol);

	while (--uiSymbol)
	{
		m_pcBinIf->encodeBin(1, pcSCModel[iOffset]);
	}
	if (bCodeLast)
	{
		m_pcBinIf->encodeBin(0, pcSCModel[iOffset]);
	}

	return;
}

Void TEncSbac::xWriteEpExGolomb(UInt uiSymbol, UInt uiCount)
{
	UInt bins = 0;
	Int numBins = 0;

	while (uiSymbol >= (UInt)(1 << uiCount))
	{
		bins = 2 * bins + 1;
		numBins++;
		uiSymbol -= 1 << uiCount;
		uiCount++;
	}
	bins = 2 * bins + 0;
	numBins++;

	bins = (bins << uiCount) | uiSymbol;
	numBins += uiCount;

	assert(numBins <= 32);
	m_pcBinIf->encodeBinsEP(bins, numBins);
}

/** Coding of coeff_abs_level_minus3
* \param uiSymbol value of coeff_abs_level_minus3
* \param ruiGoRiceParam reference to Rice parameter
* \returns Void
*/
Void TEncSbac::xWriteCoefRemainExGolomb(UInt symbol, UInt &rParam)
{
	Int codeNumber = (Int)symbol;

#pragma region Log


	LOGLN(Logs::Binarization, "koduje reszte wartosci");
	LOGLN(Logs::Binarization, "codeNumber: ", codeNumber);
	LOGLN(Logs::Binarization, "rParam: ", rParam);

#pragma endregion

	UInt length;
	if (codeNumber < (COEF_REMAIN_BIN_REDUCTION << rParam))
	{

#pragma region Log

		LOGLN(Logs::Binarization, "codeNumber: ", codeNumber, " < ", (COEF_REMAIN_BIN_REDUCTION << rParam), "(COEF_REMAIN_BIN_REDUCTION = 3 , rParam)");

#pragma endregion

		length = codeNumber >> rParam;

#pragma region Log

		LOGLN(Logs::Binarization, "kodowanie bitow");
		LOGLN(Logs::Binarization, "length: ", length, " = codeNumber>>rParam");

		std::bitset<(sizeof Int) * 8> first((1 << (length + 1)) - 2);
		std::bitset<(sizeof Int) * 8> second((codeNumber % (1 << rParam)));

		LOGLN(Logs::Binarization, "prefix ((1<<(length+1))-2): ", ((1 << (length + 1)) - 2), "(");
		for (int i = length; i >= 0; --i)
			LOG(Logs::Binarization, first[i]);
		LOGLN(Logs::Binarization, ") - ", length + 1, " bitow");
		LOGLN(Logs::Binarization, "suffix (codeNumber%(1<<rParam)): ", (codeNumber % (1 << rParam)), "(");
		for (int i = rParam - 1; i >= 0; --i)
			LOG(Logs::Binarization, second[i]);
		LOGLN(Logs::Binarization, ") - ", rParam, " bitow");

		LOGLN(Logs::Binarization, "+++", "bity symbolu ( ", symbol, ",", rParam, "): ", length + 1 + rParam);
		totalBinCount += length + 1 + rParam;

#pragma endregion

		m_pcBinIf->encodeBinsEP((1 << (length + 1)) - 2, length + 1);
		m_pcBinIf->encodeBinsEP((codeNumber % (1 << rParam)), rParam);
	}
	else
	{
		length = rParam;
		codeNumber = codeNumber - (COEF_REMAIN_BIN_REDUCTION << rParam);

#pragma region Log

		LOGLN(Logs::Binarization, "codeNumber: ", codeNumber, " >= ", (COEF_REMAIN_BIN_REDUCTION << rParam), "(COEF_REMAIN_BIN_REDUCTION = 3 << rParam)");

		LOGLN(Logs::Binarization, "porawka length i codeNumber!");
		LOGLN(Logs::Binarization, "length = rParam: ", length);
		LOGLN(Logs::Binarization, "codeNumber  = codeNumber - ( COEF_REMAIN_BIN_REDUCTION << rParam): ", codeNumber);

#pragma endregion

		while (codeNumber >= (1 << length))
		{

#pragma region Log

			LOGLN(Logs::Binarization, "porawka codeNumber, codeNumber = ", codeNumber, " >= (1<<length) = ", (1 << length));
			LOGLN(Logs::Binarization, "codeNumber -=  (1<<(length++)), length = ", length);
			LOGLN(Logs::Binarization, "codeNumber stary: ", codeNumber);

#pragma endregion

			codeNumber -= (1 << (length++));

#pragma region Log

			LOGLN(Logs::Binarization, "codeNumber nowy: ", codeNumber);

#pragma endregion

		}

#pragma region Log

		LOGLN(Logs::Binarization, "finalny codeNumber: ", codeNumber);
		LOGLN(Logs::Binarization, "kodowanie bitow");
		LOGLN(Logs::Binarization, "length: ", length);

		std::bitset<(sizeof Int) * 8> first((1 << (COEF_REMAIN_BIN_REDUCTION + length + 1 - rParam)) - 2);
		std::bitset<(sizeof Int) * 8> second(codeNumber);
		LOGLN(Logs::Binarization, "prefix ((1<<(COEF_REMAIN_BIN_REDUCTION(=3)+length+1-rParam))-2): ", ((1 << (COEF_REMAIN_BIN_REDUCTION + length + 1 - rParam)) - 2), "(");
		for (int i = COEF_REMAIN_BIN_REDUCTION + length + 1 - rParam - 1; i >= 0; --i)
			LOG(Logs::Binarization, first[i]);
		LOGLN(Logs::Binarization, ") - ", COEF_REMAIN_BIN_REDUCTION + length + 1 - rParam, " bitow (COEF_REMAIN_BIN_REDUCTION+length+1-rParam)");

		LOGLN(Logs::Binarization, "suffix (codeNumber): ", (codeNumber), "(");
		for (int i = length - 1; i >= 0; --i)
			LOG(Logs::Binarization, second[i]);
		LOGLN(Logs::Binarization, ") - ", length, " bitow (length)");

		LOGLN(Logs::BinarizationSummary, "+++", "bity symbolu( ", symbol, ",", rParam, "): ", COEF_REMAIN_BIN_REDUCTION + length + 1 - rParam + length);
		totalBinCount += COEF_REMAIN_BIN_REDUCTION + length + 1 - rParam + length;

#pragma endregion

		m_pcBinIf->encodeBinsEP((1 << (COEF_REMAIN_BIN_REDUCTION + length + 1 - rParam)) - 2, COEF_REMAIN_BIN_REDUCTION + length + 1 - rParam);
		m_pcBinIf->encodeBinsEP(codeNumber, length);
	}
}

// SBAC RD
Void  TEncSbac::load(TEncSbac* pSrc)
{
	this->xCopyFrom(pSrc);
}

Void  TEncSbac::loadIntraDirModeLuma(TEncSbac* pSrc)
{
	m_pcBinIf->copyState(pSrc->m_pcBinIf);

	this->m_cCUIntraPredSCModel.copyFrom(&pSrc->m_cCUIntraPredSCModel);
}


Void  TEncSbac::store(TEncSbac* pDest)
{
	pDest->xCopyFrom(this);
}


Void TEncSbac::xCopyFrom(TEncSbac* pSrc)
{
	m_pcBinIf->copyState(pSrc->m_pcBinIf);

	this->m_uiCoeffCost = pSrc->m_uiCoeffCost;
	this->m_uiLastQp = pSrc->m_uiLastQp;

	memcpy(m_contextModels, pSrc->m_contextModels, m_numContextModels * sizeof(ContextModel));
}

Void TEncSbac::codeMVPIdx(TComDataCU* pcCU, UInt uiAbsPartIdx, RefPicList eRefList)
{
	Int iSymbol = pcCU->getMVPIdx(eRefList, uiAbsPartIdx);
	Int iNum = AMVP_MAX_NUM_CANDS;

	xWriteUnaryMaxSymbol(iSymbol, m_cMVPIdxSCModel.get(0), 1, iNum - 1);
}

Void TEncSbac::codePartSize(TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth)
{
	PartSize eSize = pcCU->getPartitionSize(uiAbsPartIdx);
	if (pcCU->isIntra(uiAbsPartIdx))
	{
		if (uiDepth == g_uiMaxCUDepth - g_uiAddCUDepth)
		{
			m_pcBinIf->encodeBin(eSize == SIZE_2Nx2N ? 1 : 0, m_cCUPartSizeSCModel.get(0, 0, 0));
		}
		return;
	}

	switch (eSize)
	{
	case SIZE_2Nx2N:
	{
		m_pcBinIf->encodeBin(1, m_cCUPartSizeSCModel.get(0, 0, 0));
		break;
	}
	case SIZE_2NxN:
	case SIZE_2NxnU:
	case SIZE_2NxnD:
	{
		m_pcBinIf->encodeBin(0, m_cCUPartSizeSCModel.get(0, 0, 0));
		m_pcBinIf->encodeBin(1, m_cCUPartSizeSCModel.get(0, 0, 1));
		if (pcCU->getSlice()->getSPS()->getAMPAcc(uiDepth))
		{
			if (eSize == SIZE_2NxN)
			{
				m_pcBinIf->encodeBin(1, m_cCUAMPSCModel.get(0, 0, 0));
			}
			else
			{
				m_pcBinIf->encodeBin(0, m_cCUAMPSCModel.get(0, 0, 0));
				m_pcBinIf->encodeBinEP((eSize == SIZE_2NxnU ? 0 : 1));
			}
		}
		break;
	}
	case SIZE_Nx2N:
	case SIZE_nLx2N:
	case SIZE_nRx2N:
	{
		m_pcBinIf->encodeBin(0, m_cCUPartSizeSCModel.get(0, 0, 0));
		m_pcBinIf->encodeBin(0, m_cCUPartSizeSCModel.get(0, 0, 1));
		if (uiDepth == g_uiMaxCUDepth - g_uiAddCUDepth && !(pcCU->getWidth(uiAbsPartIdx) == 8 && pcCU->getHeight(uiAbsPartIdx) == 8))
		{
			m_pcBinIf->encodeBin(1, m_cCUPartSizeSCModel.get(0, 0, 2));
		}
		if (pcCU->getSlice()->getSPS()->getAMPAcc(uiDepth))
		{
			if (eSize == SIZE_Nx2N)
			{
				m_pcBinIf->encodeBin(1, m_cCUAMPSCModel.get(0, 0, 0));
			}
			else
			{
				m_pcBinIf->encodeBin(0, m_cCUAMPSCModel.get(0, 0, 0));
				m_pcBinIf->encodeBinEP((eSize == SIZE_nLx2N ? 0 : 1));
			}
		}
		break;
	}
	case SIZE_NxN:
	{
		if (uiDepth == g_uiMaxCUDepth - g_uiAddCUDepth && !(pcCU->getWidth(uiAbsPartIdx) == 8 && pcCU->getHeight(uiAbsPartIdx) == 8))
		{
			m_pcBinIf->encodeBin(0, m_cCUPartSizeSCModel.get(0, 0, 0));
			m_pcBinIf->encodeBin(0, m_cCUPartSizeSCModel.get(0, 0, 1));
			m_pcBinIf->encodeBin(0, m_cCUPartSizeSCModel.get(0, 0, 2));
		}
		break;
	}
	default:
	{
		assert(0);
	}
	}
}

/** code prediction mode
* \param pcCU
* \param uiAbsPartIdx
* \returns Void
*/
Void TEncSbac::codePredMode(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	// get context function is here
	Int iPredMode = pcCU->getPredictionMode(uiAbsPartIdx);
	m_pcBinIf->encodeBin(iPredMode == MODE_INTER ? 0 : 1, m_cCUPredModeSCModel.get(0, 0, 0));
}

Void TEncSbac::codeCUTransquantBypassFlag(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	UInt uiSymbol = pcCU->getCUTransquantBypass(uiAbsPartIdx);
	m_pcBinIf->encodeBin(uiSymbol, m_CUTransquantBypassFlagSCModel.get(0, 0, 0));
}

/** code skip flag
* \param pcCU
* \param uiAbsPartIdx
* \returns Void
*/
Void TEncSbac::codeSkipFlag(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	// get context function is here
	UInt uiSymbol = pcCU->isSkipped(uiAbsPartIdx) ? 1 : 0;
	UInt uiCtxSkip = pcCU->getCtxSkipFlag(uiAbsPartIdx);
	m_pcBinIf->encodeBin(uiSymbol, m_cCUSkipFlagSCModel.get(0, 0, uiCtxSkip));
	DTRACE_CABAC_VL(g_nSymbolCounter++);
	DTRACE_CABAC_T("\tSkipFlag");
	DTRACE_CABAC_T("\tuiCtxSkip: ");
	DTRACE_CABAC_V(uiCtxSkip);
	DTRACE_CABAC_T("\tuiSymbol: ");
	DTRACE_CABAC_V(uiSymbol);
	DTRACE_CABAC_T("\n");
}

/** code merge flag
* \param pcCU
* \param uiAbsPartIdx
* \returns Void
*/
Void TEncSbac::codeMergeFlag(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	const UInt uiSymbol = pcCU->getMergeFlag(uiAbsPartIdx) ? 1 : 0;
	m_pcBinIf->encodeBin(uiSymbol, *m_cCUMergeFlagExtSCModel.get(0));

	DTRACE_CABAC_VL(g_nSymbolCounter++);
	DTRACE_CABAC_T("\tMergeFlag: ");
	DTRACE_CABAC_V(uiSymbol);
	DTRACE_CABAC_T("\tAddress: ");
	DTRACE_CABAC_V(pcCU->getAddr());
	DTRACE_CABAC_T("\tuiAbsPartIdx: ");
	DTRACE_CABAC_V(uiAbsPartIdx);
	DTRACE_CABAC_T("\n");
}

/** code merge index
* \param pcCU
* \param uiAbsPartIdx
* \returns Void
*/
Void TEncSbac::codeMergeIndex(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	UInt uiUnaryIdx = pcCU->getMergeIndex(uiAbsPartIdx);
	UInt uiNumCand = pcCU->getSlice()->getMaxNumMergeCand();
	if (uiNumCand > 1)
	{
		for (UInt ui = 0; ui < uiNumCand - 1; ++ui)
		{
			const UInt uiSymbol = ui == uiUnaryIdx ? 0 : 1;
			if (ui == 0)
			{
				m_pcBinIf->encodeBin(uiSymbol, m_cCUMergeIdxExtSCModel.get(0, 0, 0));
			}
			else
			{
				m_pcBinIf->encodeBinEP(uiSymbol);
			}
			if (uiSymbol == 0)
			{
				break;
			}
		}
	}
	DTRACE_CABAC_VL(g_nSymbolCounter++);
	DTRACE_CABAC_T("\tparseMergeIndex()");
	DTRACE_CABAC_T("\tuiMRGIdx= ");
	DTRACE_CABAC_V(pcCU->getMergeIndex(uiAbsPartIdx));
	DTRACE_CABAC_T("\n");
}

Void TEncSbac::codeSplitFlag(TComDataCU* pcCU, UInt uiAbsPartIdx, UInt uiDepth)
{
	if (uiDepth == g_uiMaxCUDepth - g_uiAddCUDepth)
		return;

	UInt uiCtx = pcCU->getCtxSplitFlag(uiAbsPartIdx, uiDepth);
	UInt uiCurrSplitFlag = (pcCU->getDepth(uiAbsPartIdx) > uiDepth) ? 1 : 0;

	assert(uiCtx < 3);
	m_pcBinIf->encodeBin(uiCurrSplitFlag, m_cCUSplitFlagSCModel.get(0, 0, uiCtx));
	DTRACE_CABAC_VL(g_nSymbolCounter++)
		DTRACE_CABAC_T("\tSplitFlag\n")
		return;
}

Void TEncSbac::codeTransformSubdivFlag(UInt uiSymbol, UInt uiCtx)
{
	m_pcBinIf->encodeBin(uiSymbol, m_cCUTransSubdivFlagSCModel.get(0, 0, uiCtx));
	DTRACE_CABAC_VL(g_nSymbolCounter++)
		DTRACE_CABAC_T("\tparseTransformSubdivFlag()")
		DTRACE_CABAC_T("\tsymbol=")
		DTRACE_CABAC_V(uiSymbol)
		DTRACE_CABAC_T("\tctx=")
		DTRACE_CABAC_V(uiCtx)
		DTRACE_CABAC_T("\n")
}

Void TEncSbac::codeIntraDirLumaAng(TComDataCU* pcCU, UInt absPartIdx, Bool isMultiple)
{
	UInt dir[4], j;
	Int preds[4][3] = { { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 }, { -1, -1, -1 } };
	Int predNum[4], predIdx[4] = { -1, -1, -1, -1 };
	PartSize mode = pcCU->getPartitionSize(absPartIdx);
	UInt partNum = isMultiple ? (mode == SIZE_NxN ? 4 : 1) : 1;
	UInt partOffset = (pcCU->getPic()->getNumPartInCU() >> (pcCU->getDepth(absPartIdx) << 1)) >> 2;
	for (j = 0; j < partNum; j++)
	{
		dir[j] = pcCU->getLumaIntraDir(absPartIdx + partOffset*j);
		predNum[j] = pcCU->getIntraDirLumaPredictor(absPartIdx + partOffset*j, preds[j]);
		for (UInt i = 0; i < predNum[j]; i++)
		{
			if (dir[j] == preds[j][i])
			{
				predIdx[j] = i;
			}
		}
		m_pcBinIf->encodeBin((predIdx[j] != -1) ? 1 : 0, m_cCUIntraPredSCModel.get(0, 0, 0));
	}
	for (j = 0; j < partNum; j++)
	{
		if (predIdx[j] != -1)
		{
			m_pcBinIf->encodeBinEP(predIdx[j] ? 1 : 0);
			if (predIdx[j])
			{
				m_pcBinIf->encodeBinEP(predIdx[j] - 1);
			}
		}
		else
		{
			if (preds[j][0] > preds[j][1])
			{
				std::swap(preds[j][0], preds[j][1]);
			}
			if (preds[j][0] > preds[j][2])
			{
				std::swap(preds[j][0], preds[j][2]);
			}
			if (preds[j][1] > preds[j][2])
			{
				std::swap(preds[j][1], preds[j][2]);
			}
			for (Int i = (predNum[j] - 1); i >= 0; i--)
			{
				dir[j] = dir[j] > preds[j][i] ? dir[j] - 1 : dir[j];
			}
			m_pcBinIf->encodeBinsEP(dir[j], 5);
		}
	}
	return;
}

Void TEncSbac::codeIntraDirChroma(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	UInt uiIntraDirChroma = pcCU->getChromaIntraDir(uiAbsPartIdx);

	if (uiIntraDirChroma == DM_CHROMA_IDX)
	{
		m_pcBinIf->encodeBin(0, m_cCUChromaPredSCModel.get(0, 0, 0));
	}
	else
	{
		UInt uiAllowedChromaDir[NUM_CHROMA_MODE];
		pcCU->getAllowedChromaDir(uiAbsPartIdx, uiAllowedChromaDir);

		for (Int i = 0; i < NUM_CHROMA_MODE - 1; i++)
		{
			if (uiIntraDirChroma == uiAllowedChromaDir[i])
			{
				uiIntraDirChroma = i;
				break;
			}
		}
		m_pcBinIf->encodeBin(1, m_cCUChromaPredSCModel.get(0, 0, 0));

		m_pcBinIf->encodeBinsEP(uiIntraDirChroma, 2);
	}
	return;
}

Void TEncSbac::codeInterDir(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	const UInt uiInterDir = pcCU->getInterDir(uiAbsPartIdx) - 1;
	const UInt uiCtx = pcCU->getCtxInterDir(uiAbsPartIdx);
	ContextModel *pCtx = m_cCUInterDirSCModel.get(0);
	if (pcCU->getPartitionSize(uiAbsPartIdx) == SIZE_2Nx2N || pcCU->getHeight(uiAbsPartIdx) != 8)
	{
		m_pcBinIf->encodeBin(uiInterDir == 2 ? 1 : 0, *(pCtx + uiCtx));
	}
	if (uiInterDir < 2)
	{
		m_pcBinIf->encodeBin(uiInterDir, *(pCtx + 4));
	}
	return;
}

Void TEncSbac::codeRefFrmIdx(TComDataCU* pcCU, UInt uiAbsPartIdx, RefPicList eRefList)
{
	{
		Int iRefFrame = pcCU->getCUMvField(eRefList)->getRefIdx(uiAbsPartIdx);
		ContextModel *pCtx = m_cCURefPicSCModel.get(0);
		m_pcBinIf->encodeBin((iRefFrame == 0 ? 0 : 1), *pCtx);

		if (iRefFrame > 0)
		{
			UInt uiRefNum = pcCU->getSlice()->getNumRefIdx(eRefList) - 2;
			pCtx++;
			iRefFrame--;
			for (UInt ui = 0; ui < uiRefNum; ++ui)
			{
				const UInt uiSymbol = ui == iRefFrame ? 0 : 1;
				if (ui == 0)
				{
					m_pcBinIf->encodeBin(uiSymbol, *pCtx);
				}
				else
				{
					m_pcBinIf->encodeBinEP(uiSymbol);
				}
				if (uiSymbol == 0)
				{
					break;
				}
			}
		}
	}
	return;
}

Void TEncSbac::codeMvd(TComDataCU* pcCU, UInt uiAbsPartIdx, RefPicList eRefList)
{
	if (pcCU->getSlice()->getMvdL1ZeroFlag() && eRefList == REF_PIC_LIST_1 && pcCU->getInterDir(uiAbsPartIdx) == 3)
	{
		return;
	}

	const TComCUMvField* pcCUMvField = pcCU->getCUMvField(eRefList);
	const Int iHor = pcCUMvField->getMvd(uiAbsPartIdx).getHor();
	const Int iVer = pcCUMvField->getMvd(uiAbsPartIdx).getVer();
	ContextModel* pCtx = m_cCUMvdSCModel.get(0);

	m_pcBinIf->encodeBin(iHor != 0 ? 1 : 0, *pCtx);
	m_pcBinIf->encodeBin(iVer != 0 ? 1 : 0, *pCtx);

	const Bool bHorAbsGr0 = iHor != 0;
	const Bool bVerAbsGr0 = iVer != 0;
	const UInt uiHorAbs = 0 > iHor ? -iHor : iHor;
	const UInt uiVerAbs = 0 > iVer ? -iVer : iVer;
	pCtx++;

	if (bHorAbsGr0)
	{
		m_pcBinIf->encodeBin(uiHorAbs > 1 ? 1 : 0, *pCtx);
	}

	if (bVerAbsGr0)
	{
		m_pcBinIf->encodeBin(uiVerAbs > 1 ? 1 : 0, *pCtx);
	}

	if (bHorAbsGr0)
	{
		if (uiHorAbs > 1)
		{
			xWriteEpExGolomb(uiHorAbs - 2, 1);
		}

		m_pcBinIf->encodeBinEP(0 > iHor ? 1 : 0);
	}

	if (bVerAbsGr0)
	{
		if (uiVerAbs > 1)
		{
			xWriteEpExGolomb(uiVerAbs - 2, 1);
		}

		m_pcBinIf->encodeBinEP(0 > iVer ? 1 : 0);
	}

	return;
}

Void TEncSbac::codeDeltaQP(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	Int iDQp = pcCU->getQP(uiAbsPartIdx) - pcCU->getRefQP(uiAbsPartIdx);

	Int qpBdOffsetY = pcCU->getSlice()->getSPS()->getQpBDOffsetY();
	iDQp = (iDQp + 78 + qpBdOffsetY + (qpBdOffsetY / 2)) % (52 + qpBdOffsetY) - 26 - (qpBdOffsetY / 2);

	UInt uiAbsDQp = (UInt)((iDQp > 0) ? iDQp : (-iDQp));
	UInt TUValue = min((Int)uiAbsDQp, CU_DQP_TU_CMAX);
	xWriteUnaryMaxSymbol(TUValue, &m_cCUDeltaQpSCModel.get(0, 0, 0), 1, CU_DQP_TU_CMAX);
	if (uiAbsDQp >= CU_DQP_TU_CMAX)
	{
		xWriteEpExGolomb(uiAbsDQp - CU_DQP_TU_CMAX, CU_DQP_EG_k);
	}

	if (uiAbsDQp > 0)
	{
		UInt uiSign = (iDQp > 0 ? 0 : 1);
		m_pcBinIf->encodeBinEP(uiSign);
	}

	return;
}

Void TEncSbac::codeQtCbf(TComDataCU* pcCU, UInt uiAbsPartIdx, TextType eType, UInt uiTrDepth)
{
	UInt uiCbf = pcCU->getCbf(uiAbsPartIdx, eType, uiTrDepth);
	UInt uiCtx = pcCU->getCtxQtCbf(eType, uiTrDepth);
	m_pcBinIf->encodeBin(uiCbf, m_cCUQtCbfSCModel.get(0, eType ? TEXT_CHROMA : eType, uiCtx));
	DTRACE_CABAC_VL(g_nSymbolCounter++)
		DTRACE_CABAC_T("\tparseQtCbf()")
		DTRACE_CABAC_T("\tsymbol=")
		DTRACE_CABAC_V(uiCbf)
		DTRACE_CABAC_T("\tctx=")
		DTRACE_CABAC_V(uiCtx)
		DTRACE_CABAC_T("\tetype=")
		DTRACE_CABAC_V(eType)
		DTRACE_CABAC_T("\tuiAbsPartIdx=")
		DTRACE_CABAC_V(uiAbsPartIdx)
		DTRACE_CABAC_T("\n")
}

void TEncSbac::codeTransformSkipFlags(TComDataCU* pcCU, UInt uiAbsPartIdx, UInt width, UInt height, TextType eTType)
{
	if (pcCU->getCUTransquantBypass(uiAbsPartIdx))
	{

		LOGLN(Logs::Binarization, "pcCU->getCUTransquantBypass(uiAbsPartIdx) == false, wychodzê");

		return;
	}
	if (width != 4 || height != 4)
	{

		LOGLN(Logs::Binarization, "size != 4x4, wychodzê");

		return;
	}

	UInt useTransformSkip = pcCU->getTransformSkip(uiAbsPartIdx, eTType);

	if (useTransformSkip == 0)
		LOGLN(Logs::Binarization, "KODUJE 0");
	else
		LOGLN(Logs::Binarization, "KODUJE 1");

	m_pcBinIf->encodeBin(useTransformSkip, m_cTransformSkipSCModel.get(0, eTType ? TEXT_CHROMA : TEXT_LUMA, 0));

	DTRACE_CABAC_VL(g_nSymbolCounter++)
		DTRACE_CABAC_T("\tparseTransformSkip()");
	DTRACE_CABAC_T("\tsymbol=")
		DTRACE_CABAC_V(useTransformSkip)
		DTRACE_CABAC_T("\tAddr=")
		DTRACE_CABAC_V(pcCU->getAddr())
		DTRACE_CABAC_T("\tetype=")
		DTRACE_CABAC_V(eTType)
		DTRACE_CABAC_T("\tuiAbsPartIdx=")
		DTRACE_CABAC_V(uiAbsPartIdx)
		DTRACE_CABAC_T("\n")
}

/** Code I_PCM information.
* \param pcCU pointer to CU
* \param uiAbsPartIdx CU index
* \returns Void
*/
Void TEncSbac::codeIPCMInfo(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	UInt uiIPCM = (pcCU->getIPCMFlag(uiAbsPartIdx) == true) ? 1 : 0;

	Bool writePCMSampleFlag = pcCU->getIPCMFlag(uiAbsPartIdx);

	m_pcBinIf->encodeBinTrm(uiIPCM);

	if (writePCMSampleFlag)
	{
		m_pcBinIf->encodePCMAlignBits();

		UInt uiMinCoeffSize = pcCU->getPic()->getMinCUWidth()*pcCU->getPic()->getMinCUHeight();
		UInt uiLumaOffset = uiMinCoeffSize*uiAbsPartIdx;
		UInt uiChromaOffset = uiLumaOffset >> 2;
		Pel* piPCMSample;
		UInt uiWidth;
		UInt uiHeight;
		UInt uiSampleBits;
		UInt uiX, uiY;

		piPCMSample = pcCU->getPCMSampleY() + uiLumaOffset;
		uiWidth = pcCU->getWidth(uiAbsPartIdx);
		uiHeight = pcCU->getHeight(uiAbsPartIdx);
		uiSampleBits = pcCU->getSlice()->getSPS()->getPCMBitDepthLuma();

		for (uiY = 0; uiY < uiHeight; uiY++)
		{
			for (uiX = 0; uiX < uiWidth; uiX++)
			{
				UInt uiSample = piPCMSample[uiX];

				m_pcBinIf->xWritePCMCode(uiSample, uiSampleBits);
			}
			piPCMSample += uiWidth;
		}

		piPCMSample = pcCU->getPCMSampleCb() + uiChromaOffset;
		uiWidth = pcCU->getWidth(uiAbsPartIdx) / 2;
		uiHeight = pcCU->getHeight(uiAbsPartIdx) / 2;
		uiSampleBits = pcCU->getSlice()->getSPS()->getPCMBitDepthChroma();

		for (uiY = 0; uiY < uiHeight; uiY++)
		{
			for (uiX = 0; uiX < uiWidth; uiX++)
			{
				UInt uiSample = piPCMSample[uiX];

				m_pcBinIf->xWritePCMCode(uiSample, uiSampleBits);
			}
			piPCMSample += uiWidth;
		}

		piPCMSample = pcCU->getPCMSampleCr() + uiChromaOffset;
		uiWidth = pcCU->getWidth(uiAbsPartIdx) / 2;
		uiHeight = pcCU->getHeight(uiAbsPartIdx) / 2;
		uiSampleBits = pcCU->getSlice()->getSPS()->getPCMBitDepthChroma();

		for (uiY = 0; uiY < uiHeight; uiY++)
		{
			for (uiX = 0; uiX < uiWidth; uiX++)
			{
				UInt uiSample = piPCMSample[uiX];

				m_pcBinIf->xWritePCMCode(uiSample, uiSampleBits);
			}
			piPCMSample += uiWidth;
		}
		m_pcBinIf->resetBac();
	}
}

Void TEncSbac::codeQtRootCbf(TComDataCU* pcCU, UInt uiAbsPartIdx)
{
	UInt uiCbf = pcCU->getQtRootCbf(uiAbsPartIdx);
	UInt uiCtx = 0;
	m_pcBinIf->encodeBin(uiCbf, m_cCUQtRootCbfSCModel.get(0, 0, uiCtx));
	DTRACE_CABAC_VL(g_nSymbolCounter++)
		DTRACE_CABAC_T("\tparseQtRootCbf()")
		DTRACE_CABAC_T("\tsymbol=")
		DTRACE_CABAC_V(uiCbf)
		DTRACE_CABAC_T("\tctx=")
		DTRACE_CABAC_V(uiCtx)
		DTRACE_CABAC_T("\tuiAbsPartIdx=")
		DTRACE_CABAC_V(uiAbsPartIdx)
		DTRACE_CABAC_T("\n")
}

Void TEncSbac::codeQtCbfZero(TComDataCU* pcCU, TextType eType, UInt uiTrDepth)
{
	// this function is only used to estimate the bits when cbf is 0
	// and will never be called when writing the bistream. do not need to write log
	UInt uiCbf = 0;
	UInt uiCtx = pcCU->getCtxQtCbf(eType, uiTrDepth);
	m_pcBinIf->encodeBin(uiCbf, m_cCUQtCbfSCModel.get(0, eType ? TEXT_CHROMA : eType, uiCtx));
}

Void TEncSbac::codeQtRootCbfZero(TComDataCU* pcCU)
{
	// this function is only used to estimate the bits when cbf is 0
	// and will never be called when writing the bistream. do not need to write log
	UInt uiCbf = 0;
	UInt uiCtx = 0;
	m_pcBinIf->encodeBin(uiCbf, m_cCUQtRootCbfSCModel.get(0, 0, uiCtx));
}

/** Encode (X,Y) position of the last significant coefficient
* \param uiPosX X component of last coefficient
* \param uiPosY Y component of last coefficient
* \param width  Block width
* \param height Block height
* \param eTType plane type / luminance or chrominance
* \param uiScanIdx scan type (zig-zag, hor, ver)
* This method encodes the X and Y component within a block of the last significant coefficient.
*/
Void TEncSbac::codeLastSignificantXY(UInt uiPosX, UInt uiPosY, Int width, Int height, TextType eTType, UInt uiScanIdx)
{
	// swap
	if (uiScanIdx == SCAN_VER)
	{
		swap(uiPosX, uiPosY);
	}

	UInt uiCtxLast;
	ContextModel *pCtxX = m_cCuCtxLastX.get(0, eTType);
	ContextModel *pCtxY = m_cCuCtxLastY.get(0, eTType);
	UInt uiGroupIdxX = g_uiGroupIdx[uiPosX];
	UInt uiGroupIdxY = g_uiGroupIdx[uiPosY];


	Int blkSizeOffsetX, blkSizeOffsetY, shiftX, shiftY;
	blkSizeOffsetX = eTType ? 0 : (g_aucConvertToBit[width] * 3 + ((g_aucConvertToBit[width] + 1) >> 2));
	blkSizeOffsetY = eTType ? 0 : (g_aucConvertToBit[height] * 3 + ((g_aucConvertToBit[height] + 1) >> 2));
	shiftX = eTType ? g_aucConvertToBit[width] : ((g_aucConvertToBit[width] + 3) >> 2);
	shiftY = eTType ? g_aucConvertToBit[height] : ((g_aucConvertToBit[height] + 3) >> 2);

#pragma region Log

	LOGLN(Logs::Binarization, "KODUJE X: ");

#pragma endregion

	// posX
	for (uiCtxLast = 0; uiCtxLast < uiGroupIdxX; uiCtxLast++)
	{

#pragma region Log

		LOGLN(Logs::Binarization, "1");

#pragma endregion

		m_pcBinIf->encodeBin(1, *(pCtxX + blkSizeOffsetX + (uiCtxLast >> shiftX)));
	}
	if (uiGroupIdxX < g_uiGroupIdx[width - 1])
	{

#pragma region Log

		LOGLN(Logs::Binarization, "0");

#pragma endregion

		m_pcBinIf->encodeBin(0, *(pCtxX + blkSizeOffsetX + (uiCtxLast >> shiftX)));
	}

#pragma region Log

	LOGLN(Logs::Binarization, "KODUJE Y: ");

#pragma endregion


	// posY
	for (uiCtxLast = 0; uiCtxLast < uiGroupIdxY; uiCtxLast++)
	{

#pragma region Log

		LOGLN(Logs::Binarization, "1");

#pragma endregion

		m_pcBinIf->encodeBin(1, *(pCtxY + blkSizeOffsetY + (uiCtxLast >> shiftY)));
	}
	if (uiGroupIdxY < g_uiGroupIdx[height - 1])
	{

#pragma region Log

		LOGLN(Logs::Binarization, "0");

#pragma endregion

		m_pcBinIf->encodeBin(0, *(pCtxY + blkSizeOffsetY + (uiCtxLast >> shiftY)));
	}

	if (uiGroupIdxX > 3)
	{
		UInt uiCount = (uiGroupIdxX - 2) >> 1;
		uiPosX = uiPosX - g_uiMinInGroup[uiGroupIdxX];
		for (Int i = uiCount - 1; i >= 0; i--)
		{
			m_pcBinIf->encodeBinEP((uiPosX >> i) & 1);
		}
	}
	if (uiGroupIdxY > 3)
	{
		UInt uiCount = (uiGroupIdxY - 2) >> 1;
		uiPosY = uiPosY - g_uiMinInGroup[uiGroupIdxY];
		for (Int i = uiCount - 1; i >= 0; i--)
		{
			m_pcBinIf->encodeBinEP((uiPosY >> i) & 1);
		}
	}
}

//tutaj
Void TEncSbac::codeCoeffNxN(TComDataCU* pcCU, TCoeff* pcCoef, UInt uiAbsPartIdx, UInt uiWidth, UInt uiHeight, UInt uiDepth, TextType eTType)
{
	DTRACE_CABAC_VL(g_nSymbolCounter++)
		DTRACE_CABAC_T("\tparseCoeffNxN()\teType=")
		DTRACE_CABAC_V(eTType)
		DTRACE_CABAC_T("\twidth=")
		DTRACE_CABAC_V(uiWidth)
		DTRACE_CABAC_T("\theight=")
		DTRACE_CABAC_V(uiHeight)
		DTRACE_CABAC_T("\tdepth=")
		DTRACE_CABAC_V(uiDepth)
		DTRACE_CABAC_T("\tabspartidx=")
		DTRACE_CABAC_V(uiAbsPartIdx)
		DTRACE_CABAC_T("\ttoCU-X=")
		DTRACE_CABAC_V(pcCU->getCUPelX())
		DTRACE_CABAC_T("\ttoCU-Y=")
		DTRACE_CABAC_V(pcCU->getCUPelY())
		DTRACE_CABAC_T("\tCU-addr=")
		DTRACE_CABAC_V(pcCU->getAddr())
		DTRACE_CABAC_T("\tinCU-X=")
		DTRACE_CABAC_V(g_auiRasterToPelX[g_auiZscanToRaster[uiAbsPartIdx]])
		DTRACE_CABAC_T("\tinCU-Y=")
		DTRACE_CABAC_V(g_auiRasterToPelY[g_auiZscanToRaster[uiAbsPartIdx]])
		DTRACE_CABAC_T("\tpredmode=")
		DTRACE_CABAC_V(pcCU->getPredictionMode(uiAbsPartIdx))
		DTRACE_CABAC_T("\n")

#pragma region Log

	LOGLN(Logs::Binarization, "*********************************");
	LOGLN(Logs::Binarization, "texture: ", ((eTType == TEXT_LUMA) ? "LUMA" : "CHROMA"));
	LOGLN(Logs::Binarization, "width: ", uiWidth);
	LOGLN(Logs::Binarization, "height: ", uiHeight);
	LOGLN(Logs::Binarization, "depth: ", uiDepth);
	LOGLN(Logs::Binarization, "abspartidx: ", uiAbsPartIdx);
	LOGLN(Logs::Binarization, "CUx: ", pcCU->getCUPelX());
	LOGLN(Logs::Binarization, "CUy: ", pcCU->getCUPelY());
	LOGLN(Logs::Binarization, "CUaddr: ", pcCU->getAddr());
	LOGLN(Logs::Binarization, "inCUx: ", g_auiRasterToPelX[g_auiZscanToRaster[uiAbsPartIdx]]);
	LOGLN(Logs::Binarization, "inCUx: ", g_auiRasterToPelY[g_auiZscanToRaster[uiAbsPartIdx]]);
	LOGLN(Logs::Binarization, "predmode: ", pcCU->getPredictionMode(uiAbsPartIdx));
	LOGLN(Logs::Binarization, "macierz: ");
	LOG_MATRIX_1D(Logs::Binarization, pcCoef, uiWidth, uiHeight, uiWidth);


#pragma endregion

	if (uiWidth > m_pcSlice->getSPS()->getMaxTrSize())
	{
		uiWidth = m_pcSlice->getSPS()->getMaxTrSize();
		uiHeight = m_pcSlice->getSPS()->getMaxTrSize();
	}

	UInt uiNumSig = 0;

	// compute number of significant coefficients
	uiNumSig = TEncEntropy::countNonZeroCoeffs(pcCoef, uiWidth * uiHeight);

#pragma region Log

	LOGLN(Logs::Binarization, "ilosc wspolczynnikow niezerowych: ", uiNumSig);

#pragma endregion

	if (uiNumSig == 0)
		return;
	if (pcCU->getSlice()->getPPS()->getUseTransformSkip())
	{

#pragma region Log

		LOGLN(Logs::Binarization, "transformSkip!");

#pragma endregion

		codeTransformSkipFlags(pcCU, uiAbsPartIdx, uiWidth, uiHeight, eTType);
	}
	eTType = eTType == TEXT_LUMA ? TEXT_LUMA : (eTType == TEXT_NONE ? TEXT_NONE : TEXT_CHROMA);

	//----- encode significance map -----
	const UInt   uiLog2BlockSize = g_aucConvertToBit[uiWidth] + 2;
	UInt uiScanIdx = pcCU->getCoefScanIdx(uiAbsPartIdx, uiWidth, eTType == TEXT_LUMA, pcCU->isIntra(uiAbsPartIdx));
	const UInt *scan = g_auiSigLastScan[uiScanIdx][uiLog2BlockSize - 1];

	Bool beValid;
	if (pcCU->getCUTransquantBypass(uiAbsPartIdx))
	{
		beValid = false;
	}
	else
	{
		beValid = pcCU->getSlice()->getPPS()->getSignHideFlag() > 0;
	}

	// Find position of last coefficient
	Int scanPosLast = -1;
	Int posLast;

	const UInt * scanCG;
	{
		scanCG = g_auiSigLastScan[uiScanIdx][uiLog2BlockSize > 3 ? uiLog2BlockSize - 2 - 1 : 0];
		if (uiLog2BlockSize == 3)
		{
			scanCG = g_sigLastScan8x8[uiScanIdx];
		}
		else if (uiLog2BlockSize == 5)
		{
			scanCG = g_sigLastScanCG32x32;
		}
	}
	UInt uiSigCoeffGroupFlag[MLS_GRP_NUM];
	static const UInt uiShift = MLS_CG_SIZE >> 1;
	const UInt uiNumBlkSide = uiWidth >> uiShift;

	::memset(uiSigCoeffGroupFlag, 0, sizeof(UInt)* MLS_GRP_NUM);

#pragma region Log

	LOGLN(Logs::Binarization, "+++", "skan: ");


#pragma endregion

	do
	{
		posLast = scan[++scanPosLast];

#pragma region Log

		LOGLN(Logs::Binarization, pcCoef[posLast], " ");

#pragma endregion

		// get L1 sig map - to ciekawe tylko dla > 4x4
		UInt uiPosY = posLast >> uiLog2BlockSize;
		UInt uiPosX = posLast - (uiPosY << uiLog2BlockSize);
		UInt uiBlkIdx = uiNumBlkSide * (uiPosY >> uiShift) + (uiPosX >> uiShift);
		if (pcCoef[posLast])
		{
			uiSigCoeffGroupFlag[uiBlkIdx] = 1;
		}

		uiNumSig -= (pcCoef[posLast] != 0);
	} while (uiNumSig > 0);

#pragma region Log

#pragma endregion

	// Code position of last coefficient
	Int posLastY = posLast >> uiLog2BlockSize;
	Int posLastX = posLast - (posLastY << uiLog2BlockSize);

#pragma region Log

	LOGLN(Logs::Binarization, "posLastY: ", posLastY);
	LOGLN(Logs::Binarization, "posLastX: ", posLastX);

	UInt XplusY = 0;
	XplusY += posLastX == 3 ? 3 : posLastX + 1;
	XplusY += posLastY == 3 ? 3 : posLastY + 1;

	LOGLN(Logs::Binarization, "+++", "bity pozycji: ", XplusY);
	totalBinCount += XplusY;

#pragma endregion

	codeLastSignificantXY(posLastX, posLastY, uiWidth, uiHeight, eTType, uiScanIdx);

	//===== code significance flag =====
	ContextModel * const baseCoeffGroupCtx = m_cCUSigCoeffGroupSCModel.get(0, eTType);
	ContextModel * const baseCtx = (eTType == TEXT_LUMA) ? m_cCUSigSCModel.get(0, 0) : m_cCUSigSCModel.get(0, 0) + NUM_SIG_FLAG_CTX_LUMA;


	const Int  iLastScanSet = scanPosLast >> LOG2_SCAN_SET_SIZE;
	UInt c1 = 1;
	UInt uiGoRiceParam = 0;
	Int  iScanPosSig = scanPosLast;

	for (Int iSubSet = iLastScanSet; iSubSet >= 0; iSubSet--)
	{
		Int numNonZero = 0;
		Int  iSubPos = iSubSet << LOG2_SCAN_SET_SIZE;
		uiGoRiceParam = 0;
		Int absCoeff[16];
		UInt coeffSigns = 0;

		Int lastNZPosInCG = -1, firstNZPosInCG = SCAN_SET_SIZE;

		if (iScanPosSig == scanPosLast)
		{
			absCoeff[0] = abs(pcCoef[posLast]);
			coeffSigns = (pcCoef[posLast] < 0);
			numNonZero = 1;
			lastNZPosInCG = iScanPosSig;
			firstNZPosInCG = iScanPosSig;
			iScanPosSig--;

			LOGLN(Logs::Binarization, "iScanPosSig == scanPosLast!");
			LOGLN(Logs::Binarization, "absCoeff[ 0 ]: ", absCoeff[0]);
			LOGLN(Logs::Binarization, "coeffSigns: ", coeffSigns);
			LOGLN(Logs::Binarization, "lastNZPosInCG: ", lastNZPosInCG);
			LOGLN(Logs::Binarization, "firstNZPosInCG: ", firstNZPosInCG);
			LOGLN(Logs::Binarization, "iScanPosSig: ", iScanPosSig);
			LOGLN(Logs::Binarization, "numNonZero: ", numNonZero);
		}

		// encode significant_coeffgroup_flag
		Int iCGBlkPos = scanCG[iSubSet];
		Int iCGPosY = iCGBlkPos / uiNumBlkSide;
		Int iCGPosX = iCGBlkPos - (iCGPosY * uiNumBlkSide);

		if (iSubSet == iLastScanSet || iSubSet == 0)
		{
			uiSigCoeffGroupFlag[iCGBlkPos] = 1;

		}
		else
		{
			UInt uiSigCoeffGroup = (uiSigCoeffGroupFlag[iCGBlkPos] != 0);
			UInt uiCtxSig = TComTrQuant::getSigCoeffGroupCtxInc(uiSigCoeffGroupFlag, iCGPosX, iCGPosY, uiWidth, uiHeight);

#pragma region Log

			LOGLN(Logs::BinarizationSummary, "+++", "bity grupy: ", uiSigCoeffGroup);
			totalBinCount += uiSigCoeffGroup;

#pragma endregion

			m_pcBinIf->encodeBin(uiSigCoeffGroup, baseCoeffGroupCtx[uiCtxSig]);
		}

		// encode significant_coeff_flag
		if (uiSigCoeffGroupFlag[iCGBlkPos])
		{
#pragma region Log

			partialBinCount = 0;

#pragma endregion

			Int patternSigCtx = TComTrQuant::calcPatternSigCtx(uiSigCoeffGroupFlag, iCGPosX, iCGPosY, uiWidth, uiHeight);

			UInt uiBlkPos, uiPosY, uiPosX, uiSig, uiCtxSig;
			for (; iScanPosSig >= iSubPos; iScanPosSig--)
			{
				uiBlkPos = scan[iScanPosSig];
				uiPosY = uiBlkPos >> uiLog2BlockSize;
				uiPosX = uiBlkPos - (uiPosY << uiLog2BlockSize);
				uiSig = (pcCoef[uiBlkPos] != 0);

#pragma region Log

				LOGLN(Logs::Binarization, "coeff[", uiPosX, "][", uiPosY, "]: ", pcCoef[uiBlkPos]);
				LOGLN(Logs::Binarization, " (uiSig: ", uiSig, ")");

#pragma endregion

				if (iScanPosSig > iSubPos || iSubSet == 0 || numNonZero)
				{
					uiCtxSig = TComTrQuant::getSigCtxInc(patternSigCtx, uiScanIdx, uiPosX, uiPosY, uiLog2BlockSize, eTType);

#pragma region Log

					LOGLN(Logs::Binarization, "KODUJE: ", uiSig);

					partialBinCount++;

#pragma endregion

					m_pcBinIf->encodeBin(uiSig, baseCtx[uiCtxSig]);
				}
				if (uiSig)
				{
					absCoeff[numNonZero] = abs(pcCoef[uiBlkPos]);
					coeffSigns = 2 * coeffSigns + (pcCoef[uiBlkPos] < 0);
					numNonZero++;
					if (lastNZPosInCG == -1)
					{
						lastNZPosInCG = iScanPosSig;
					}
					firstNZPosInCG = iScanPosSig;

#pragma region Log


					LOGLN(Logs::Binarization, "absCoeff: ");
					for (int i = 0; i < 16; ++i)
						LOG(Logs::Binarization, absCoeff[i], " ");
					LOGLN(Logs::Binarization, "\n");
					LOGLN(Logs::Binarization, "coeffSigns: ", coeffSigns);
					LOGLN(Logs::Binarization, "numNonZero: ", numNonZero);
					LOGLN(Logs::Binarization, "lastNZPosInCG: ", lastNZPosInCG);
					LOGLN(Logs::Binarization, "firstNZPosInCG: ", firstNZPosInCG);

#pragma endregion

				}
			}

#pragma region Log

			LOGLN(Logs::Binarization, "+++", "bity znaczonosci: ", partialBinCount);
			totalBinCount += partialBinCount;
			partialBinCount = 0;

#pragma endregion

		}
		else
		{
			iScanPosSig = iSubPos - 1;
		}

		if (numNonZero > 0)
		{
#pragma region Log


			LOGLN(Logs::Binarization, "numNonZero = ", numNonZero, " > 0: ");

#pragma endregion

			Bool signHidden = (lastNZPosInCG - firstNZPosInCG >= SBH_THRESHOLD);
			UInt uiCtxSet = (iSubSet > 0 && eTType == TEXT_LUMA) ? 2 : 0;

			if (c1 == 0)
			{
				uiCtxSet++;
			}
			c1 = 1;
			ContextModel *baseCtxMod = (eTType == TEXT_LUMA) ? m_cCUOneSCModel.get(0, 0) + 4 * uiCtxSet : m_cCUOneSCModel.get(0, 0) + NUM_ONE_FLAG_CTX_LUMA + 4 * uiCtxSet;

			Int numC1Flag = min(numNonZero, C1FLAG_NUMBER);
			Int firstC2FlagIdx = -1;

#pragma region Log

			LOGLN(Logs::Binarization, "signHidden: ", signHidden);
			LOGLN(Logs::Binarization, "numC1Flag (max 8): ", numC1Flag);
			LOGLN(Logs::Binarization, "\npetla po numC1flag ", firstC2FlagIdx);

#pragma endregion

			for (Int idx = 0; idx < numC1Flag; idx++)
			{
				UInt uiSymbol = absCoeff[idx] > 1;

#pragma region Log

				LOGLN(Logs::Binarization, "absCoeff[ idx = ", idx, " ]: ", absCoeff[idx]);
				LOGLN(Logs::Binarization, "KODUJE ", uiSymbol);

				partialBinCount++;

#pragma endregion

				m_pcBinIf->encodeBin(uiSymbol, baseCtxMod[c1]);

				if (uiSymbol)
				{
					c1 = 0;

					if (firstC2FlagIdx == -1)
					{
						firstC2FlagIdx = idx;
					}
				}
				else if ((c1 < 3) && (c1 > 0))
				{
					c1++;
				}

#pragma region Log

				LOGLN(Logs::Binarization, "c1: ", c1);
				LOGLN(Logs::Binarization, "firstC2FlagIdx: ", firstC2FlagIdx);

#pragma endregion

			}

#pragma region Log

			LOGLN(Logs::Binarization, "koniec petli!");
			LOGLN(Logs::Binarization, "c1: ", c1);
			LOGLN(Logs::Binarization, "firstC2FlagIdx: ", firstC2FlagIdx);

			LOGLN(Logs::Binarization, "+++", "bity C1: ", partialBinCount);
			totalBinCount += partialBinCount;
			partialBinCount = 0;

#pragma endregion

			if (c1 == 0)
			{
				baseCtxMod = (eTType == TEXT_LUMA) ? m_cCUAbsSCModel.get(0, 0) + uiCtxSet : m_cCUAbsSCModel.get(0, 0) + NUM_ABS_FLAG_CTX_LUMA + uiCtxSet;

				if (firstC2FlagIdx != -1)
				{
					UInt symbol = absCoeff[firstC2FlagIdx] > 2;

#pragma region Log

					LOGLN(Logs::Binarization, "absCoeff[ firstC2FlagIdx = ", firstC2FlagIdx, " ] = ", absCoeff[firstC2FlagIdx]);
					LOGLN(Logs::Binarization, "c1 == 0 && firstC2FlagIdx != -1");
					LOGLN(Logs::Binarization, "KODUJE ", symbol, " (1 jesli absCoeff[ firstC2FlagIdx ] > 2, 0 wpp)");

					LOGLN(Logs::Binarization, "+++", "bit C2: ", symbol);
					totalBinCount += 1;

#pragma endregion

					m_pcBinIf->encodeBin(symbol, baseCtxMod[0]);
				}
			}

			if (beValid && signHidden)
			{

#pragma region Log

				LOGLN(Logs::Binarization, "beValid && signHidden!");
				std::bitset<((sizeof UInt) * 8) - 1> first((coeffSigns >> 1));
				LOGLN(Logs::Binarization, "mapa znakow (coeffSigns >> 1): ");
				for (int i = numNonZero - 2; i >= 0; --i)
					LOG(Logs::Binarization, first[i]);
				LOGLN(Logs::Binarization, " - ", numNonZero - 1, " bitow");

				LOGLN(Logs::Binarization, "+++", "bity znakow: ", numNonZero - 1);
				totalBinCount += numNonZero - 1;

#pragma endregion

				m_pcBinIf->encodeBinsEP((coeffSigns >> 1), numNonZero - 1);
			}
			else
			{

#pragma region Log


				LOGLN(Logs::Binarization, " !(beValid && signHidden) !");
				std::bitset<((sizeof UInt) * 8) - 1> first(coeffSigns);
				LOGLN(Logs::Binarization, "KODUJE mape znakow (coeffSigns): ");
				for (int i = numNonZero - 1; i >= 0; --i)
					LOG(Logs::Binarization, first[i]);
				LOGLN(Logs::Binarization, " - ", numNonZero, " bitow");

				LOGLN(Logs::Binarization, "+++", "bity znakow: ", numNonZero);
				totalBinCount += numNonZero;

#pragma endregion

				m_pcBinIf->encodeBinsEP(coeffSigns, numNonZero);
			}

			Int iFirstCoeff2 = 1;
			if (c1 == 0 || numNonZero > C1FLAG_NUMBER)
			{

#pragma region Log

				LOGLN(Logs::Binarization, "c1 == 0 || numNonZero > 8, kodujemy mape wartosci");

#pragma endregion

				for (Int idx = 0; idx < numNonZero; idx++)
				{
					UInt baseLevel = (idx < C1FLAG_NUMBER) ? (2 + iFirstCoeff2) : 1;

#pragma region Log

					LOGLN(Logs::Binarization, "idx: ", idx);
					LOGLN(Logs::Binarization, "absCoeff[ idx ]: ", absCoeff[idx]);
					LOGLN(Logs::Binarization, "baseLevel: ", baseLevel);

#pragma endregion

					if (absCoeff[idx] >= baseLevel)
					{

#pragma region Log

						LOGLN(Logs::Binarization, "absCoeff[ idx = ", idx, " ] = ", absCoeff[idx], " >= baseLevel = ", baseLevel);

#pragma endregion

						xWriteCoefRemainExGolomb(absCoeff[idx] - baseLevel, uiGoRiceParam);

						if (absCoeff[idx] > 3 * (1 << uiGoRiceParam))
						{

#pragma region Log

							LOGLN(Logs::Binarization, "absCoeff[idx]=", absCoeff[idx], " > 3*(1<<uiGoRiceParam)=", 3 * (1 << uiGoRiceParam), " , obcinam min<UInt>(uiGoRiceParam+ 1, 4)");

#pragma endregion

							uiGoRiceParam = min<UInt>(uiGoRiceParam + 1, 4);
						}

#pragma region Log

						LOGLN(Logs::Binarization, "uiGoRiceParam: ", uiGoRiceParam);

#pragma endregion

					}
					if (absCoeff[idx] >= 2)
					{

#pragma region Log
						LOGLN(Logs::Binarization, "absCoeff[ idx ] >= 2, oraz < baseLevel, nic nie koduje, zeruje iFirstCoeff2");

#pragma endregion

						iFirstCoeff2 = 0;
					}
				}
			}
		}
	}


	LOGLN(Logs::Binarization, "+++", "WSZYSTKICH: ", totalBinCount);
	totalBinCount = 0;
	partialBinCount = 0;

	return;
}

/** code SAO offset sign
* \param code sign value
*/
Void TEncSbac::codeSAOSign(UInt code)
{
	m_pcBinIf->encodeBinEP(code);
}

Void TEncSbac::codeSaoMaxUvlc(UInt code, UInt maxSymbol)
{
	if (maxSymbol == 0)
	{
		return;
	}

	Int i;
	Bool bCodeLast = (maxSymbol > code);

	if (code == 0)
	{
		m_pcBinIf->encodeBinEP(0);
	}
	else
	{
		m_pcBinIf->encodeBinEP(1);
		for (i = 0; i < code - 1; i++)
		{
			m_pcBinIf->encodeBinEP(1);
		}
		if (bCodeLast)
		{
			m_pcBinIf->encodeBinEP(0);
		}
	}
}


/** Code SAO EO class or BO band position
* \param uiLength
* \param uiCode
*/
Void TEncSbac::codeSaoUflc(UInt uiLength, UInt uiCode)
{
	m_pcBinIf->encodeBinsEP(uiCode, uiLength);
}
/** Code SAO merge flags
* \param uiCode
* \param uiCompIdx
*/
Void TEncSbac::codeSaoMerge(UInt uiCode)
{
	if (uiCode == 0)
	{
		m_pcBinIf->encodeBin(0, m_cSaoMergeSCModel.get(0, 0, 0));
	}
	else
	{
		m_pcBinIf->encodeBin(1, m_cSaoMergeSCModel.get(0, 0, 0));
	}
}
/** Code SAO type index
* \param uiCode
*/
Void TEncSbac::codeSaoTypeIdx(UInt uiCode)
{
	if (uiCode == 0)
	{
		m_pcBinIf->encodeBin(0, m_cSaoTypeIdxSCModel.get(0, 0, 0));
	}
	else
	{
		m_pcBinIf->encodeBin(1, m_cSaoTypeIdxSCModel.get(0, 0, 0));
		m_pcBinIf->encodeBinEP(uiCode <= 4 ? 1 : 0);
	}
}
/*!
****************************************************************************
* \brief
*   estimate bit cost for CBP, significant map and significant coefficients
****************************************************************************
*/
Void TEncSbac::estBit(estBitsSbacStruct* pcEstBitsSbac, Int width, Int height, TextType eTType)
{
	estCBFBit(pcEstBitsSbac);

	estSignificantCoeffGroupMapBit(pcEstBitsSbac, eTType);

	// encode significance map
	estSignificantMapBit(pcEstBitsSbac, width, height, eTType);

	// encode significant coefficients
	estSignificantCoefficientsBit(pcEstBitsSbac, eTType);
}

/*!
****************************************************************************
* \brief
*    estimate bit cost for each CBP bit
****************************************************************************
*/
Void TEncSbac::estCBFBit(estBitsSbacStruct* pcEstBitsSbac)
{
	ContextModel *pCtx = m_cCUQtCbfSCModel.get(0);

	for (UInt uiCtxInc = 0; uiCtxInc < 3 * NUM_QT_CBF_CTX; uiCtxInc++)
	{
		pcEstBitsSbac->blockCbpBits[uiCtxInc][0] = pCtx[uiCtxInc].getEntropyBits(0);
		pcEstBitsSbac->blockCbpBits[uiCtxInc][1] = pCtx[uiCtxInc].getEntropyBits(1);
	}

	pCtx = m_cCUQtRootCbfSCModel.get(0);

	for (UInt uiCtxInc = 0; uiCtxInc < 4; uiCtxInc++)
	{
		pcEstBitsSbac->blockRootCbpBits[uiCtxInc][0] = pCtx[uiCtxInc].getEntropyBits(0);
		pcEstBitsSbac->blockRootCbpBits[uiCtxInc][1] = pCtx[uiCtxInc].getEntropyBits(1);
	}
}


/*!
****************************************************************************
* \brief
*    estimate SAMBAC bit cost for significant coefficient group map
****************************************************************************
*/
Void TEncSbac::estSignificantCoeffGroupMapBit(estBitsSbacStruct* pcEstBitsSbac, TextType eTType)
{
	Int firstCtx = 0, numCtx = NUM_SIG_CG_FLAG_CTX;

	for (Int ctxIdx = firstCtx; ctxIdx < firstCtx + numCtx; ctxIdx++)
	{
		for (UInt uiBin = 0; uiBin < 2; uiBin++)
		{
			pcEstBitsSbac->significantCoeffGroupBits[ctxIdx][uiBin] = m_cCUSigCoeffGroupSCModel.get(0, eTType, ctxIdx).getEntropyBits(uiBin);
		}
	}
}


/*!
****************************************************************************
* \brief
*    estimate SAMBAC bit cost for significant coefficient map
****************************************************************************
*/
Void TEncSbac::estSignificantMapBit(estBitsSbacStruct* pcEstBitsSbac, Int width, Int height, TextType eTType)
{
	Int firstCtx = 1, numCtx = 8;
	if (max(width, height) >= 16)
	{
		firstCtx = (eTType == TEXT_LUMA) ? 21 : 12;
		numCtx = (eTType == TEXT_LUMA) ? 6 : 3;
	}
	else if (width == 8)
	{
		firstCtx = 9;
		numCtx = (eTType == TEXT_LUMA) ? 12 : 3;
	}

	if (eTType == TEXT_LUMA)
	{
		for (UInt bin = 0; bin < 2; bin++)
		{
			pcEstBitsSbac->significantBits[0][bin] = m_cCUSigSCModel.get(0, 0, 0).getEntropyBits(bin);
		}

		for (Int ctxIdx = firstCtx; ctxIdx < firstCtx + numCtx; ctxIdx++)
		{
			for (UInt uiBin = 0; uiBin < 2; uiBin++)
			{
				pcEstBitsSbac->significantBits[ctxIdx][uiBin] = m_cCUSigSCModel.get(0, 0, ctxIdx).getEntropyBits(uiBin);
			}
		}
	}
	else
	{
		for (UInt bin = 0; bin < 2; bin++)
		{
			pcEstBitsSbac->significantBits[0][bin] = m_cCUSigSCModel.get(0, 0, NUM_SIG_FLAG_CTX_LUMA + 0).getEntropyBits(bin);
		}
		for (Int ctxIdx = firstCtx; ctxIdx < firstCtx + numCtx; ctxIdx++)
		{
			for (UInt uiBin = 0; uiBin < 2; uiBin++)
			{
				pcEstBitsSbac->significantBits[ctxIdx][uiBin] = m_cCUSigSCModel.get(0, 0, NUM_SIG_FLAG_CTX_LUMA + ctxIdx).getEntropyBits(uiBin);
			}
		}
	}
	Int iBitsX = 0, iBitsY = 0;
	Int blkSizeOffsetX, blkSizeOffsetY, shiftX, shiftY;

	blkSizeOffsetX = eTType ? 0 : (g_aucConvertToBit[width] * 3 + ((g_aucConvertToBit[width] + 1) >> 2));
	blkSizeOffsetY = eTType ? 0 : (g_aucConvertToBit[height] * 3 + ((g_aucConvertToBit[height] + 1) >> 2));
	shiftX = eTType ? g_aucConvertToBit[width] : ((g_aucConvertToBit[width] + 3) >> 2);
	shiftY = eTType ? g_aucConvertToBit[height] : ((g_aucConvertToBit[height] + 3) >> 2);

	Int ctx;
	ContextModel *pCtxX = m_cCuCtxLastX.get(0, eTType);
	for (ctx = 0; ctx < g_uiGroupIdx[width - 1]; ctx++)
	{
		Int ctxOffset = blkSizeOffsetX + (ctx >> shiftX);
		pcEstBitsSbac->lastXBits[ctx] = iBitsX + pCtxX[ctxOffset].getEntropyBits(0);
		iBitsX += pCtxX[ctxOffset].getEntropyBits(1);
	}
	pcEstBitsSbac->lastXBits[ctx] = iBitsX;
	ContextModel *pCtxY = m_cCuCtxLastY.get(0, eTType);
	for (ctx = 0; ctx < g_uiGroupIdx[height - 1]; ctx++)
	{
		Int ctxOffset = blkSizeOffsetY + (ctx >> shiftY);
		pcEstBitsSbac->lastYBits[ctx] = iBitsY + pCtxY[ctxOffset].getEntropyBits(0);
		iBitsY += pCtxY[ctxOffset].getEntropyBits(1);
	}
	pcEstBitsSbac->lastYBits[ctx] = iBitsY;
}

/*!
****************************************************************************
* \brief
*    estimate bit cost of significant coefficient
****************************************************************************
*/
Void TEncSbac::estSignificantCoefficientsBit(estBitsSbacStruct* pcEstBitsSbac, TextType eTType)
{
	if (eTType == TEXT_LUMA)
	{
		ContextModel *ctxOne = m_cCUOneSCModel.get(0, 0);
		ContextModel *ctxAbs = m_cCUAbsSCModel.get(0, 0);

		for (Int ctxIdx = 0; ctxIdx < NUM_ONE_FLAG_CTX_LUMA; ctxIdx++)
		{
			pcEstBitsSbac->m_greaterOneBits[ctxIdx][0] = ctxOne[ctxIdx].getEntropyBits(0);
			pcEstBitsSbac->m_greaterOneBits[ctxIdx][1] = ctxOne[ctxIdx].getEntropyBits(1);
		}

		for (Int ctxIdx = 0; ctxIdx < NUM_ABS_FLAG_CTX_LUMA; ctxIdx++)
		{
			pcEstBitsSbac->m_levelAbsBits[ctxIdx][0] = ctxAbs[ctxIdx].getEntropyBits(0);
			pcEstBitsSbac->m_levelAbsBits[ctxIdx][1] = ctxAbs[ctxIdx].getEntropyBits(1);
		}
	}
	else
	{
		ContextModel *ctxOne = m_cCUOneSCModel.get(0, 0) + NUM_ONE_FLAG_CTX_LUMA;
		ContextModel *ctxAbs = m_cCUAbsSCModel.get(0, 0) + NUM_ABS_FLAG_CTX_LUMA;

		for (Int ctxIdx = 0; ctxIdx < NUM_ONE_FLAG_CTX_CHROMA; ctxIdx++)
		{
			pcEstBitsSbac->m_greaterOneBits[ctxIdx][0] = ctxOne[ctxIdx].getEntropyBits(0);
			pcEstBitsSbac->m_greaterOneBits[ctxIdx][1] = ctxOne[ctxIdx].getEntropyBits(1);
		}

		for (Int ctxIdx = 0; ctxIdx < NUM_ABS_FLAG_CTX_CHROMA; ctxIdx++)
		{
			pcEstBitsSbac->m_levelAbsBits[ctxIdx][0] = ctxAbs[ctxIdx].getEntropyBits(0);
			pcEstBitsSbac->m_levelAbsBits[ctxIdx][1] = ctxAbs[ctxIdx].getEntropyBits(1);
		}
	}
}

/**
- Initialize our context information from the nominated source.
.
\param pSrc From where to copy context information.
*/
Void TEncSbac::xCopyContextsFrom(TEncSbac* pSrc)
{
	memcpy(m_contextModels, pSrc->m_contextModels, m_numContextModels*sizeof(m_contextModels[0]));
}

Void  TEncSbac::loadContexts(TEncSbac* pScr)
{
	this->xCopyContextsFrom(pScr);
}
//! \}
