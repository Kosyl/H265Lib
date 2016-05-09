/**
* @file	IntraPred.h
*
* @brief	Deklaracje klasy predyktora intra
*/
#ifndef INTRA_PRED_H
#define INTRA_PRED_H

#include <cassert>
#include <algorithm>

#include <Common/TypeDef.h>
#include <Common/Enums.h>
#include <Data/PU.h>

namespace HEVC
{
	class IntraPred
	{
	private:

		static IntraPred *itsInstance;

		IntraMode **itsModes;

		PB* itsCurrentPB;

		Sample itsCornerValue;

		Sample** itsReferenceValues;

		int getFilteringThreshold() const;

		bool isFilteringRequired() const;

		Sample filtRef(const Sample mainRef, const Sample leftRef, const Sample rightRef) const;

		void filterSideRefs(const IntraDirection referenceDirection);

		void doReferenceFiltering();

		bool checkSmoothConditions(const IntraDirection direction) const;

		bool isSmoothingRequired() const;

		Sample getSmoothedReferenceAtPosition(const IntraDirection dir, const int offset) const;

		void smoothSideRefs(const IntraDirection dir);

		void doReferenceSmoothing();

		IntraMode* getPredictionStrategy();

	public:

		IntraPred();
		~IntraPred();

		Sample** calcPred(PBIntra* targetPB);

		Sample** calcPredForceRefs(PBIntra* tergetPB, Sample* leftRefs, Sample* topRefs, const Sample corner);
	};
}
#endif