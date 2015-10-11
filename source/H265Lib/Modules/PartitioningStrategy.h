///**
//* @file	PartitioningStrategy.h
//*
//* @brief	Deklaracje klas strategii partycjonowania
//*/
//#ifndef _PARTITIONINGSTRATEGY_H_
//#define _PARTITIONINGSTRATEGY_H_
//
//#include "Utils.h"
//#include "QuadTree.h"
//#include "CU.h"
//#include "TU.h"
//#include "Slice.h"
//#include "PU.h"
//#include "CTU.h"
//#include "IntraPred.h"
//#include <set>
//#include <limits>
//
///**
// * @class	PartitioningStrategy
// *
// * @brief	Klasa bazowa strategii partycjonowania.
// */
//
//class PartitioningStrategy
//{
//public:
//
//	/**
//	 * @fn	PartitioningStrategy::PartitioningStrategy( )
//	 *
//	 * @brief	Konstruktor domyœlny.
//	 */
//
//	PartitioningStrategy()
//	{
//	}
//
//	/**
//	 * @fn	virtual PartitioningStrategy::~PartitioningStrategy( )
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	virtual ~PartitioningStrategy()
//	{
//	}
//
//	/**
//	 * @fn	virtual Void PartitioningStrategy::doPartitioning( std::shared_ptr<CTU> ctu ) = 0;
//	 *
//	 * @brief	Partycjonowanie na poziomie CTU.
//	 *
//	 * @param	ctu	WskaŸnik na CTU.
//	 *
//	 * @return	Void.
//	 */
//
//	virtual Void doPartitioning(std::shared_ptr<CTU> ctu) = 0;
//};
//
///**
// * @class	DefaultBigPartitions
// *
// * @brief Strategia podzia³u na du¿e partycje.
// */
//
///**
// * @class	OptimalPartitioning
// *
// * @brief	Strategia optymalna.
// */
//
//class OptimalPartitioning : public PartitioningStrategy
//{
//private:
//
//	/**
//	 * @brief	Instancja.
//	 */
//
//	static OptimalPartitioning* instance;
//
//	/**
//	 * @fn	OptimalPartitioning::OptimalPartitioning( )
//	 *
//	 * @brief	Konstruktor domyœlny.
//	 */
//
//	OptimalPartitioning()
//	{
//	}
//
//	/**
//	 * @brief	QP dla sk³adowcyh obrazu.
//	 */
//
//	UInt QP[3];
//
//	/**
//	 * @brief	Aktualnie dzielona CU.
//	 */
//
//	std::shared_ptr<CUIntra> currentCU;
//
//	/**
//	 * @brief	Aktualnie dzielona PU.
//	 */
//
//	std::shared_ptr<PUIntra> currentPU;
//
//public:
//
//	/**
//	 * @fn	static OptimalPartitioning* OptimalPartitioning::getStrategy( )
//	 *
//	 * @brief	Dostêp do strategii.
//	 *
//	 * @return	WskaŸnik na singleton.
//	 */
//
//	static OptimalPartitioning* getStrategy()
//	{
//		if (instance == nullptr)
//			instance = new OptimalPartitioning();
//		return instance;
//	}
//
//	/**
//	 * @fn	virtual OptimalPartitioning::~OptimalPartitioning( )
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	virtual ~OptimalPartitioning()
//	{
//	}
//
//	/**
//	 * @fn	Void OptimalPartitioning::doPartitioning( std::shared_ptr<CTU> ctu )
//	 *
//	 * @brief	Partycjonowanie na poziomie CTU.
//	 *
//	 * @param	ctu CTU do podzia³u.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doPartitioning(std::shared_ptr<CTU> ctu)
//	{
//		QP[LUMA] = PicParams()->getQP(LUMA) + ctu->getParentSlice()->getQPDelta(LUMA);
//		QP[CB] = PicParams()->getQP(CB) + ctu->getParentSlice()->getQPDelta(CB);
//		QP[CR] = PicParams()->getQP(CR) + ctu->getParentSlice()->getQPDelta(CR);
//
//		optimizePartitions(ctu->getCUQuadTree());
//	}
//
//	/**
//	 * @fn	Void OptimalPartitioning::optimizePartitions( std::shared_ptr<CUQuadTree> CUtree )
//	 *
//	 * @brief	Partycjonowanie na poziomie drzewa CU. Sprawdzenie optymalnej wersji NxN i optymalnej 2Nx2N.
//	 *
//	 * @param	CUtree	Drzewo jednostek kodowych.
//	 *
//	 * @return	Void.
//	 */
//
//	Void optimizePartitions(std::shared_ptr<CUQuadTree> CUtree)
//	{
//		bool canMakeSinglePartition =
//			CUtree->getSize() <= SeqParams()->getCTUSize() &&
//			CUtree->getSize() <= SeqParams()->getPicWidth() &&
//			CUtree->getSize() <= SeqParams()->getPicHeight() &&
//			CUtree->getX() + CUtree->getSize() <= SeqParams()->getPicWidth() &&
//			CUtree->getY() + CUtree->getSize() <= SeqParams()->getPicHeight();
//
//		bool canMakeQuadPartitions = CUtree->getSize() > SeqParams()->getMinCUSize();
//
//		if (canMakeQuadPartitions && canMakeSinglePartition) // trzeba sprawdzic obie
//		{
//			Double costSinglePart = std::numeric_limits<Double>::max(), costQuadPart = std::numeric_limits<Double>::max();
//
//			std::shared_ptr<CUIntra> singlePart = nullptr;
//
//			//jedna part.
//			singlePart = std::make_shared<CUIntra>(CUtree->getParentCTU(), CUtree->getX(), CUtree->getY(), CUtree->getSize());
//
//			//LOG( "OPT" ) << "sprawdzam single" << std::endl;
//
//			//LOG_TAB "OPT" );
//			optimizePartitions(singlePart);
//			//LOG_UNTAB "OPT" );
//
//			costSinglePart = singlePart->getTotalCost();
//
//			//4 part.
//			//LOG( "OPT" ) << "sprawdzam multi" << std::endl;
//			//LOG_TAB "OPT" );
//			CUtree->createSubTrees();
//			for (UInt i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
//			{
//				if (CUtree->getSubTree((QTComponent)i) != nullptr)
//					optimizePartitions(CUtree->getSubTree((QTComponent)i));
//			}
//			//LOG_UNTAB "OPT" );
//			costQuadPart = CUtree->getTotalCost();
//
//			//LOG( "OPT" ) << "costSinglePart: " << costSinglePart << ", costQuadPart: " << costQuadPart << std::endl;
//			if (costSinglePart < costQuadPart)
//			{
//				//LOG( "OPT" ) << "ustawiam single" << std::endl;
//				CUtree->clear();
//				CUtree->setLeaf(singlePart);
//			}
//			else
//			{
//				//LOG( "OPT" ) << "ustawiam multi" << std::endl;
//				//nic, juz jest
//			}
//		}
//		else if (canMakeSinglePartition)
//		{
//			//LOG( "OPT" ) << "mozliwa tylko single" << std::endl;
//			std::shared_ptr<CUIntra> singlePart = nullptr;
//			singlePart = std::make_shared<CUIntra>(CUtree->getParentCTU(), CUtree->getX(), CUtree->getY(), CUtree->getSize());
//			//LOG_TAB "OPT" );
//			optimizePartitions(singlePart);
//			//LOG_UNTAB "OPT" );
//			CUtree->clear();
//			CUtree->setLeaf(singlePart);
//		}
//		else
//		{
//			//LOG( "OPT" ) << "mozliwa tylko multi" << std::endl;
//			CUtree->createSubTrees();
//			//LOG_TAB "OPT" );
//			for (UInt i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
//			{
//				if (CUtree->getSubTree((QTComponent)i) != nullptr)
//					optimizePartitions(CUtree->getSubTree((QTComponent)i));
//			}
//			//LOG_UNTAB "OPT" );
//		}
//	}
//
//	/**
//	 * @fn	Void OptimalPartitioning::optimizePartitions( std::shared_ptr<CUIntra> cu )
//	 *
//	 * @brief	Partycjonowanie na poziomie CU. (sprawdzenie NxN vs 2Nx2N)
//	 *
//	 * @param	cu	CU do otymalizacji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void optimizePartitions(std::shared_ptr<CUIntra> cu)
//	{
//		//LOG( "OPT" ) << "CU(" << cu->getX( ) << "," << cu->getY( ) << ") " << cu->getSize( ) << "x" << cu->getSize( ) << ", optymalizacja Luma" << std::endl;
//
//		currentCU = cu;
//
//		//proba dla 2Nx2N, zawsze mozna
//		cu->setPartitionMode(PART_2Nx2N);
//		cu->createPUs();
//		std::shared_ptr<PUIntra> pu = cu->getPU();
//		cu->getTransformTree()->setQTMode(QTMODE_LEAF);
//		pu->setTransformArea(cu->getTransformTree());
//
//		//LOG( "OPT" ) << "sprawdzam 2Nx2N" << std::endl;
//		//LOG_TAB "OPT" );
//		optimizePUForLuma(pu);
//		//LOG_UNTAB "OPT" );
//		Double singleCost = RDCost::getInstance()->calcCost(pu->getTransformArea()->getTotalBins(LUMA), pu->getTransformArea()->getTotalDistortion(LUMA), QP[LUMA]);
//
//		bool canMakeQuadPUPart = cu->getSize() == SeqParams()->getMinCUSize();
//		if (canMakeQuadPUPart)
//		{
//			UInt multiPartBits = 0, multiPartDistortion = 0;
//			//LOG( "OPT" ) << "sprawdzam NxN" << std::endl;
//			//LOG_TAB "OPT" );
//			cu->setPartitionMode(PART_NxN);
//			cu->getTransformTree()->setQTMode(QTMODE_SPLIT);
//			cu->createPUs();
//			for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
//			{
//				cu->getPU(position)->setTransformArea(cu->getTransformTree()->getSubTree(position));
//				optimizePUForLuma(cu->getPU(position));
//				multiPartBits += cu->getTransformTree()->getSubTree(position)->getTotalBins(LUMA);
//				multiPartDistortion += cu->getTransformTree()->getSubTree(position)->getTotalDistortion(LUMA);
//			}
//			cu->getTransformTree()->setTotalBins(multiPartBits, LUMA);
//			cu->getTransformTree()->setTotalDistortion(multiPartDistortion, LUMA);
//			//LOG_UNTAB "OPT" );
//		}
//
//		if (!canMakeQuadPUPart)
//		{
//			//LOG( "OPT" ) << "mozna tylko single" << std::endl;
//			cu->setTransformTree(pu->getTransformArea());
//		}
//		else
//		{
//			Double multiCost = RDCost::getInstance()->calcCost(cu->getTransformTree()->getTotalBins(LUMA), cu->getTransformTree()->getTotalDistortion(LUMA), QP[LUMA]);
//			//LOG( "OPT" ) << "costSinglePart: " << singleCost << ", costQuadPart: " << multiCost << std::endl;
//			if (singleCost <= multiCost)
//			{
//				//LOG( "OPT" ) << "wybieram single" << std::endl;
//				cu->resetPUs();
//				cu->setTransformTree(pu->getTransformArea());
//				cu->addPU(pu);
//				//LOG( "OPT" ) << "przywracam stan" << std::endl;
//				//LOG_TAB "OPT" );
//				optimizePUForLuma(pu);
//				//LOG_UNTAB "OPT" );
//			}
//			else
//			{
//				//LOG( "OPT" ) << "wybieram multi" << std::endl;
//				//nic, juz jest ustawione
//			}
//		}
//
//		optimizeCUForChroma(cu);
//	}
//
//	/**
//	 * @fn	Void OptimalPartitioning::optimizeCUForChroma( std::shared_ptr<CUIntra> cu )
//	 *
//	 * @brief Dobór optymalnego trybu predykcji chrominancji (bez zmiany wyników uzyskanych z optymalizacji luminancji).
//	 *
//	 * @param	cu	CU, w którym wybieramy tryb predykcji chrominancji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void optimizeCUForChroma(std::shared_ptr<CUIntra> cu)
//	{
//		//LOG( "OPT" ) << "CU(" << cu->getX( ) << "," << cu->getY( ) << ") " << cu->getSize( ) << "x" << cu->getSize( ) << ", optymalizacja CHROMA" << std::endl;
//
//		UInt bestChromaMode = -1;
//		Double bestChromaCost = std::numeric_limits<Double>::max();
//
//		//for( UShort i = 0; i < 5; ++i )
//		for (UShort i = 0; i < 2; ++i)
//		{
//			//LOG( "OPT" ) << "proba dla trybu chroma " << i << std::endl;
//
//			cu->setIntraChromaPredictionDerivationType(i);
//			//LOG_TAB "OPT" );
//			optimizePUsForChroma(cu);
//			//LOG_UNTAB "OPT" );
//			Double costCB = RDCost::getInstance()->calcCost(cu->getTransformTree()->getTotalBins(CB), cu->getTransformTree()->getTotalDistortion(CB), QP[CB]);
//			Double costCR = RDCost::getInstance()->calcCost(cu->getTransformTree()->getTotalBins(CR), cu->getTransformTree()->getTotalDistortion(CR), QP[CR]);
//			//LOG( "OPT" ) << "costCR: " << costCR << ", costCB: " << costCB << ", total: " << costCR + costCB << ", current: " << bestChromaCost << std::endl;
//			if (bestChromaCost > costCR + costCB)
//			{
//				//LOG( "OPT" ) << "zmieniam tryb na " << i << std::endl;
//				bestChromaCost = costCR + costCB;
//				bestChromaMode = i;
//			}
//		}
//		if (bestChromaMode != 4)
//		{
//			cu->setIntraChromaPredictionDerivationType(bestChromaMode);
//			//LOG( "OPT" ) << "przywracam stan" << std::endl;
//			//LOG_TAB "OPT" );
//			optimizePUsForChroma(cu);
//			//LOG_UNTAB "OPT" );
//		}
//	}
//
//	/**
//	 * @fn	Void OptimalPartitioning::optimizePUForLuma( std::shared_ptr<PUIntra> pu )
//	 *
//	 * @brief	Wybór najlepszego trybu predykcji (spoœród 1 i 2 - ze wzglêdu na szybkoœæ dzia³ania)
//	 *
//	 * @param	pu	Aktualna PU.
//	 *
//	 * @return	Void.
//	 */
//
//	Void optimizePUForLuma(std::shared_ptr<PUIntra> pu)
//	{
//		//LOG( "OPT" ) << "PU(" << pu->getX( ) << "," << pu->getY( ) << ") " << pu->getSize( ) << "x" << pu->getSize( ) << ", optymalizacja Luma" << std::endl;
//
//		currentPU = pu;
//		UInt bestLumaMode = -1;
//		Double bestLumaCost = std::numeric_limits<Double>::max();
//
//		//for( UInt lumaMode = 0; lumaMode < 36; ++lumaMode )
//		for (UInt lumaMode = 0; lumaMode < 2; ++lumaMode)
//		{
//			pu->setLumaModeIdx(lumaMode);
//			//LOG( "OPT" ) << "sprawdzam tryb " << lumaMode << std::endl;
//
//			//LOG_TAB "OPT" );
//			optimizeTUTreeForLuma(pu->getTransformArea());
//			//LOG_UNTAB "OPT" );
//
//			Double currentCost = RDCost::getInstance()->calcCost(pu->getTransformArea()->getTotalBins(LUMA), pu->getTransformArea()->getTotalDistortion(LUMA), QP[LUMA]);
//			if (currentCost < bestLumaCost)
//			{
//				//LOG( "OPT" ) << "currentCost: " << currentCost << ", bestLumaCost: " << bestLumaCost << ", poprawiam" << std::endl;
//				bestLumaCost = currentCost;
//				bestLumaMode = lumaMode;
//			}
//		}
//		pu->setLumaModeIdx(bestLumaMode);
//		//LOG( "OPT" ) << "najlepszy tryb: " << bestLumaMode << ", przywracam stan: " << std::endl;
//
//		//LOG_TAB "OPT" );
//		optimizeTUTreeForLuma(pu->getTransformArea());
//		//LOG_UNTAB "OPT" );
//
//		currentPU = nullptr;
//	}
//
//	/**
//	 * @fn	Void OptimalPartitioning::optimizeTUTreeForLuma( std::shared_ptr<TUQuadTree> TUTree )
//	 *
//	 * @brief	Optymalizacja drzewa TU (NxN vs 2Nx2N).
//	 *
//	 * @param	TUTree	Aktualne drzewo TU.
//	 *
//	 * @return	Void.
//	 */
//
//	Void optimizeTUTreeForLuma(std::shared_ptr<TUQuadTree> TUTree)
//	{
//		//LOG( "OPT" ) << "TUTree(" << TUTree->getX( ) << "," << TUTree->getY( ) << ") " << TUTree->getSize( ) << "x" << TUTree->getSize( ) << ", optymalizacja Luma" << std::endl;
//
//		UInt multiPartDistortion = 0;
//		UInt multiPartBins = 0;
//
//		bool atLeastOneQuad = false;
//
//		Bool canMakeQuadPart = TUTree->getSize() > SeqParams()->getMinTUSize();
//		if (canMakeQuadPart)
//		{
//			//LOG( "OPT" ) << "proba dla multi" << std::endl;
//			//LOG_TAB "OPT" );
//			TUTree->createSubTrees();
//			for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
//			{
//				if (TUTree->getSubTree(position) != nullptr)
//				{
//					atLeastOneQuad = true;
//					optimizeTUTreeForLuma(TUTree->getSubTree(position));
//					multiPartBins += TUTree->getSubTree(position)->getTotalBins(LUMA);
//					multiPartDistortion += TUTree->getSubTree(position)->getTotalDistortion(LUMA);
//				}
//			}
//			//LOG_UNTAB "OPT" );
//			//LOG( "OPT" ) << "multiPartBins: " << multiPartBins << ", multiPartDistortion: " << multiPartDistortion << std::endl;
//		}
//
//		//LOG( "OPT" ) << "proba dla single" << std::endl;
//		std::shared_ptr<TU> leaf = std::make_shared<TU>(TUTree->getParentCU(), TUTree->getX(), TUTree->getY(), TUTree->getSize());
//		std::shared_ptr<TB> tb = leaf->getTB(LUMA);
//		tb->transformAndReconstruct(currentPU);
//		UInt singlePartDistortion = tb->getDistortion();
//		UInt singlePartBins = tb->getNumBins();
//
//		Double singleCost = RDCost::getInstance()->calcCost(singlePartBins, singlePartDistortion, QP[LUMA]);
//		Double multiCost = RDCost::getInstance()->calcCost(multiPartBins, multiPartDistortion, QP[LUMA]);
//		//LOG( "OPT" ) << "singlePartDistortion: " << singlePartDistortion << ", singlePartBins: " << singlePartBins << std::endl;
//		//LOG( "OPT" ) << "singleCost: " << singleCost << ", multiCost: " << multiCost << std::endl;
//		if (singleCost > multiCost && atLeastOneQuad)
//		{
//			//LOG( "OPT" ) << "wybieram tryb multi" << std::endl;
//			TUTree->setQTMode(QTMODE_SPLIT);
//			TUTree->setTotalBins(multiPartBins, LUMA);
//			TUTree->setTotalDistortion(multiPartDistortion, LUMA);
//			TUTree->setLeaf(nullptr);
//		}
//		else
//		{
//			//LOG( "OPT" ) << "wybieram tryb single" << std::endl;
//			TUTree->clear();
//			TUTree->setQTMode(QTMODE_LEAF);
//			TUTree->setTotalBins(singlePartBins, LUMA);
//			TUTree->setTotalDistortion(singlePartDistortion, LUMA);
//			TUTree->setLeaf(leaf);
//		}
//	}
//
//	/**
//	 * @fn	Void OptimalPartitioning::optimizeTUTreeForChroma( std::shared_ptr<TUQuadTree> TUTree )
//	 *
//	 * @brief	Przeprowadzanie pêtli rekonstrukcji dla chrominancji w celu policzenia kosztu.
//	 *
//	 * @param	TUTree	Aktualne drzewo TU.
//	 *
//	 * @return	Void.
//	 */
//
//	Void optimizeTUTreeForChroma(std::shared_ptr<TUQuadTree> TUTree)
//	{
//		//LOG( "OPT" ) << "TUTree(" << TUTree->getX( ) << "," << TUTree->getY( ) << ") " << TUTree->getSize( ) << "x" << TUTree->getSize( ) << ", optymalizacja CHROMA" << std::endl;
//
//		if (TUTree->getQTMode() == QTMODE_SPLIT)
//		{
//			//LOG( "OPT" ) << "QTMODE_SPLIT" << std::endl;
//			UInt multiPartDistortionCr = 0, multiPartDistortionCb = 0;
//			UInt multiPartBinsCr = 0, multiPartBinsCb = 0;
//
//			if (TUTree->getSize() > 8)
//			{
//				//LOG( "OPT" ) << "size > 8, liczymy chroma dla czterech partycji" << std::endl;
//				//LOG_TAB "OPT" );
//				for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
//				{
//					if (TUTree->getSubTree(position) != nullptr)
//					{
//						optimizeTUTreeForChroma(TUTree->getSubTree(position));
//						multiPartBinsCr += TUTree->getSubTree(position)->getTotalBins(CR);
//						multiPartBinsCb += TUTree->getSubTree(position)->getTotalBins(CB);
//						multiPartDistortionCr += TUTree->getSubTree(position)->getTotalDistortion(CR);
//						multiPartDistortionCb += TUTree->getSubTree(position)->getTotalDistortion(CB);
//					}
//				}
//				//LOG_UNTAB "OPT" );
//				//LOG( "OPT" ) << "multiPartBinsCr: " << multiPartBinsCr << ", multiPartDistortionCr" << multiPartDistortionCr << std::endl;
//				//LOG( "OPT" ) << "multiPartBinsCb: " << multiPartBinsCb << ", multiPartDistortionCb" << multiPartDistortionCb << std::endl;
//			}
//			else if (TUTree->getSize() == 8)
//			{
//				//LOG( "OPT" ) << "size == 8, liczymy chroma dla jednej partycji" << std::endl;
//				//LOG_TAB "OPT" );
//				optimizeTUTreeForChroma(TUTree->getSubTree(UPPERLEFT));
//				//LOG_UNTAB "OPT" );
//				multiPartBinsCr += TUTree->getSubTree(UPPERLEFT)->getTotalBins(CR);
//				multiPartBinsCb += TUTree->getSubTree(UPPERLEFT)->getTotalBins(CB);
//				multiPartDistortionCr += TUTree->getSubTree(UPPERLEFT)->getTotalDistortion(CR);
//				multiPartDistortionCb += TUTree->getSubTree(UPPERLEFT)->getTotalDistortion(CB);
//			}
//			TUTree->setTotalBins(multiPartBinsCr, CR);
//			TUTree->setTotalBins(multiPartBinsCb, CB);
//			TUTree->setTotalDistortion(multiPartDistortionCb, CB);
//			TUTree->setTotalDistortion(multiPartDistortionCr, CR);
//		}
//		else if (TUTree->getX() % 8 == 0 && TUTree->getY() % 8 == 0) // LEAF; jesli tu wejdziemy, to z poprzednich petli wynika, ze na pewno jest skladnik chroma
//		{
//			//LOG( "OPT" ) << "lisc! robimy petle" << std::endl;
//			std::shared_ptr<TB> tbCR = TUTree->getLeaf()->getTB(CR);
//			std::shared_ptr<TB> tbCB = TUTree->getLeaf()->getTB(CB);
//			//LOG_TAB "OPT" );
//			tbCR->transformAndReconstruct(currentPU);
//			TUTree->setTotalDistortion(tbCR->getDistortion(), CR);
//			TUTree->setTotalBins(tbCR->getNumBins(), CR);
//			tbCB->transformAndReconstruct(currentPU);
//			TUTree->setTotalDistortion(tbCB->getDistortion(), CB);
//			TUTree->setTotalBins(tbCB->getNumBins(), CB);
//			//LOG_UNTAB "OPT" );
//		}
//		else
//		{
//			//LOG( "OPT" ) << "lisc bez TB chroma" << std::endl;
//			TUTree->setTotalDistortion(0, CR);
//			TUTree->setTotalBins(0, CR);
//			TUTree->setTotalDistortion(0, CB);
//			TUTree->setTotalBins(0, CB);
//		}
//	}
//
//	/**
//	 * @fn	Void OptimalPartitioning::optimizePUsForChroma( std::shared_ptr<CUIntra> cu )
//	 *
//	 * @brief	Przekazanie optymalizacji do drzewa TU przez wszystkie PU w danym CU.
//	 *
//	 * @param	cu	Aktualne CU.
//	 *
//	 * @return	Void.
//	 */
//
//	Void optimizePUsForChroma(std::shared_ptr<CUIntra> cu)
//	{
//		for (QTComponent i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
//		{
//			std::shared_ptr<PUIntra> pu = cu->getPU(i);
//			if (pu != nullptr)
//			{
//				currentPU = pu;
//				currentPU->refreshChromaModeIdx();
//				//LOG( "OPT" ) << "PU(" << pu->getX( ) << "," << pu->getY( ) << ") " << pu->getSize( ) << "x" << pu->getSize( ) << ", optymalizacja CHROMA" << std::endl;
//				//LOG_TAB "OPT" );
//				optimizeTUTreeForChroma(pu->getTransformArea());
//				//LOG_UNTAB "OPT" );
//				currentPU = nullptr;
//			}
//		}
//	}
//};
//
///**
// * @class	FDBTestPartitions
// *
// * @brief	Strategia partycjonowania wed³ug informacji w pliku.
// */
//
//class FDBTestPartitions : public PartitioningStrategy
//{
//	/**
//	 * @brief	Strumieñ pliku wejœciowego.
//	 */
//
//	std::ifstream* itsStream;
//
//	/**
//	 * @property	Int currentCUX, currentCUY, currentQP, currentCUSize
//	 *
//	 * @brief	Wspó³rzêdne, rozmiar i QP wczytanej jednostki CU.
//	 *
//	 */
//
//	Int currentCUX, currentCUY, currentQP, currentCUSize;
//
//	/**
//	 * @brief	Rozmiar aktualnego TU.
//	 */
//
//	Int currentTUSize;
//
//	/**
//	 * @property	Bool cuParamsReady, tuParamsReady
//	 *
//	 * @brief	Flagi mówi¹ce, czy ustalono ju¿ partycje wed³ug wczytanych informacji (nowe informacje nie bêd¹ wczytywane, dopóki aktualne nie zostan¹ u¿yte).
//	 *
//	 */
//
//	Bool cuParamsReady, tuParamsReady;
//
//public:
//
//	/**
//	 * @fn	FDBTestPartitions::FDBTestPartitions( std::ifstream* str )
//	 *
//	 * @brief	Konstruktor.
//	 *
//	 * @param [in]	str	Strumieñ wejœciowy pliku z opisem struktury.
//	 */
//
//	FDBTestPartitions(std::ifstream* str) :
//		itsStream(str),
//		cuParamsReady(false),
//		tuParamsReady(false)
//	{
//	}
//
//	/**
//	 * @fn	virtual FDBTestPartitions::~FDBTestPartitions( )
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	virtual ~FDBTestPartitions()
//	{
//	}
//
//	/**
//	 * @fn	virtual Void FDBTestPartitions::doPartitioning( std::shared_ptr<CTU> ctu )
//	 *
//	 * @brief	Partycjonowanie na poziomie CTU.
//	 *
//	 * @param	ctu	Aktualne CTU.
//	 *
//	 * @return	Void.
//	 */
//
//	virtual Void doPartitioning(std::shared_ptr<CTU> ctu)
//	{
//		doPartitioning(ctu->getCUQuadTree());
//	}
//
//	/**
//	 * @fn	Void FDBTestPartitions::doPartitioning( std::shared_ptr<CUQuadTree> CUtree )
//	 *
//	 * @brief	Partycjonowanie na poziomie drzewa jednostek kodowych.
//	 *
//	 * @param	CUtree	Drzewo jednostek kodowych.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doPartitioning(std::shared_ptr<CUQuadTree> CUtree)
//	{
//		if (!cuParamsReady)
//		{
//			goToMarker(*itsStream, "CU");
//			readValue(*itsStream, "X", currentCUX);
//			readValue(*itsStream, "Y", currentCUY);
//			readValue(*itsStream, "QP", currentQP);
//			readValue(*itsStream, "SIZE", currentCUSize);
//			cuParamsReady = true;
//		}
//		if (CUtree->getSize() == currentCUSize && CUtree->getX() == currentCUX && CUtree->getY() == currentCUY)
//		{
//			CUtree->setLeaf(std::make_shared<CUIntra>(CUtree->getParentCTU(), CUtree->getX(), CUtree->getY(), CUtree->getSize()));
//			CUtree->getLeaf()->setIntraChromaPredictionDerivationType(0);
//			CUtree->setQTMode(QTMODE_LEAF);
//			doPartitioning(CUtree->getLeaf());
//			cuParamsReady = false;
//		}
//		else
//		{
//			CUtree->createSubTrees();
//
//			for (UInt i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
//			{
//				if (CUtree->getSubTree((QTComponent)i) != nullptr)
//					doPartitioning(CUtree->getSubTree((QTComponent)i));
//			}
//		}
//	}
//
//	/**
//	 * @fn	Void FDBTestPartitions::doPartitioning( std::shared_ptr<CUIntra> cu )
//	 *
//	 * @brief	Partycjonowanie CU.
//	 *
//	 * @param	cu	Aktualne CU.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doPartitioning(std::shared_ptr<CUIntra> cu)
//	{
//		goToMarker(*itsStream, "CUSTRUCT");
//		tuParamsReady = false;
//		doPartitioning(cu->getTransformTree());
//		std::string mode = "";
//		readValue(*itsStream, "PU", mode);
//		cu->setPartitionMode(mode == "2Nx2N" ? PART_2Nx2N : PART_NxN);
//		cu->createPUs();
//		if (cu->getPartitionMode() == PART_2Nx2N)
//		{
//			cu->getPU()->setTransformArea(cu->getTransformTree());
//		}
//		else
//		{
//			for (QTComponent i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
//			{
//				if (cu->getPU(i) != nullptr)
//					cu->getPU(i)->setTransformArea(cu->getTransformTree()->getSubTree(i));
//			}
//		}
//	}
//
//	/**
//	 * @fn	Void FDBTestPartitions::doPartitioning( std::shared_ptr<TUQuadTree> TUTree )
//	 *
//	 * @brief	Partycjonowanie na poziomie drzewa TU.
//	 *
//	 * @param	TUTree	Drzewo jednostek transformacji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doPartitioning(std::shared_ptr<TUQuadTree> TUTree)
//	{
//		if (!tuParamsReady)
//		{
//			goToMarker(*itsStream, "TU");
//			readValue(*itsStream, "WIDTH", currentTUSize);
//			tuParamsReady = true;
//		}
//		if (TUTree->getSize() == currentTUSize)
//		{
//			TUTree->setLeaf(std::make_shared<TU>(TUTree->getParentCU(), TUTree->getX(), TUTree->getY(), TUTree->getSize()));
//			TUTree->setQTMode(QTMODE_LEAF);
//			tuParamsReady = false;
//		}
//		else
//		{
//			TUTree->createSubTrees();
//			TUTree->setQTMode(QTMODE_SPLIT);
//			for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
//			{
//				if (TUTree->getSubTree(position) != nullptr)
//					doPartitioning(TUTree->getSubTree(position));
//			}
//		}
//	}
//};
//
//
//class IntraPartitioning : public PartitioningStrategy
//{
//private:
//
//	/**
//	* @brief	Instancja.
//	*/
//
//	static IntraPartitioning* instance;
//
//	/**
//	* @fn	OptimalPartitioning::OptimalPartitioning( )
//	*
//	* @brief	Konstruktor domyœlny.
//	*/
//
//	IntraPartitioning()
//	{
//	}
//
//	/**
//	* @brief	QP dla sk³adowcyh obrazu.
//	*/
//
//	UInt QP[3];
//
//	/**
//	* @brief	Aktualnie dzielona CU.
//	*/
//
//	std::shared_ptr<CUIntra> currentCU;
//
//	/**
//	* @brief	Aktualnie dzielona PU.
//	*/
//
//	std::shared_ptr<PUIntra> currentPU;
//
//public:
//
//	/**
//	* @fn	static OptimalPartitioning* OptimalPartitioning::getStrategy( )
//	*
//	* @brief	Dostêp do strategii.
//	*
//	* @return	WskaŸnik na singleton.
//	*/
//
//	static IntraPartitioning* getStrategy()
//	{
//		if (instance == nullptr)
//			instance = new IntraPartitioning();
//		return instance;
//	}
//
//	/**
//	* @fn	virtual OptimalPartitioning::~OptimalPartitioning( )
//	*
//	* @brief	Destruktor.
//	*/
//
//	virtual ~IntraPartitioning()
//	{
//	}
//
//	/**
//	* @fn	Void OptimalPartitioning::doPartitioning( std::shared_ptr<CTU> ctu )
//	*
//	* @brief	Partycjonowanie na poziomie CTU.
//	*
//	* @param	ctu CTU do podzia³u.
//	*
//	* @return	Void.
//	*/
//
//	Void doPartitioning(std::shared_ptr<CTU> ctu)
//	{
//		QP[LUMA] = PicParams()->getQP(LUMA) + ctu->getParentSlice()->getQPDelta(LUMA);
//		QP[CB] = PicParams()->getQP(CB) + ctu->getParentSlice()->getQPDelta(CB);
//		QP[CR] = PicParams()->getQP(CR) + ctu->getParentSlice()->getQPDelta(CR);
//
//		doPreselection(ctu->getCUQuadTree());
//	}
//
//	Void doPreselection(std::shared_ptr<CUQuadTree> CUtree)
//	{
//		bool canMakeSinglePartition = CUtree->getSize() == 8;
//
//		//preselekcja
//		if (canMakeSinglePartition)
//		{
//			std::shared_ptr<CUIntra> singlePart = nullptr;
//			singlePart = std::make_shared<CUIntra>(CUtree->getParentCTU(), CUtree->getX(), CUtree->getY(), CUtree->getSize());
//			doPreselection(singlePart);
//			CUtree->clear();
//			CUtree->setLeaf(singlePart);
//		}
//		else
//		{
//			CUtree->createSubTrees();
//			for (UInt i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
//			{
//				if (CUtree->getSubTree((QTComponent)i) != nullptr)
//					doPreselection(CUtree->getSubTree((QTComponent)i));
//			}
//		}
//	}
//
//	/**
//	* @fn	Void OptimalPartitioning::optimizePartitions( std::shared_ptr<CUIntra> cu )
//	*
//	* @brief	Partycjonowanie na poziomie CU. (sprawdzenie NxN vs 2Nx2N)
//	*
//	* @param	cu	CU do otymalizacji.
//	*
//	* @return	Void.
//	*/
//
//	Void doPreselection(std::shared_ptr<CUIntra> cu)
//	{
//		currentCU = cu;
//
//		cu->setPartitionMode(PART_2Nx2N);
//		cu->createPUs();
//		std::shared_ptr<PUIntra> pu = cu->getPU();
//		cu->getTransformTree()->setQTMode(QTMODE_LEAF);
//		pu->setTransformArea(cu->getTransformTree());
//
//		doPreselection(pu);
//	}
//
//	/**
//	* @fn	Void OptimalPartitioning::optimizeCUForChroma( std::shared_ptr<CUIntra> cu )
//	*
//	* @brief Dobór optymalnego trybu predykcji chrominancji (bez zmiany wyników uzyskanych z optymalizacji luminancji).
//	*
//	* @param	cu	CU, w którym wybieramy tryb predykcji chrominancji.
//	*
//	* @return	Void.
//	*/
//
//	Void optimizeCUForChroma(std::shared_ptr<CUIntra> cu)
//	{
//		//LOG( "OPT" ) << "CU(" << cu->getX( ) << "," << cu->getY( ) << ") " << cu->getSize( ) << "x" << cu->getSize( ) << ", optymalizacja CHROMA" << std::endl;
//
//		UInt bestChromaMode = -1;
//		Double bestChromaCost = std::numeric_limits<Double>::max();
//
//		//for( UShort i = 0; i < 5; ++i )
//		for (UShort i = 0; i < 2; ++i)
//		{
//			//LOG( "OPT" ) << "proba dla trybu chroma " << i << std::endl;
//
//			cu->setIntraChromaPredictionDerivationType(i);
//			//LOG_TAB "OPT" );
//			optimizePUsForChroma(cu);
//			//LOG_UNTAB "OPT" );
//			Double costCB = RDCost::getInstance()->calcCost(cu->getTransformTree()->getTotalBins(CB), cu->getTransformTree()->getTotalDistortion(CB), QP[CB]);
//			Double costCR = RDCost::getInstance()->calcCost(cu->getTransformTree()->getTotalBins(CR), cu->getTransformTree()->getTotalDistortion(CR), QP[CR]);
//			//LOG( "OPT" ) << "costCR: " << costCR << ", costCB: " << costCB << ", total: " << costCR + costCB << ", current: " << bestChromaCost << std::endl;
//			if (bestChromaCost > costCR + costCB)
//			{
//				//LOG( "OPT" ) << "zmieniam tryb na " << i << std::endl;
//				bestChromaCost = costCR + costCB;
//				bestChromaMode = i;
//			}
//		}
//		if (bestChromaMode != 4)
//		{
//			cu->setIntraChromaPredictionDerivationType(bestChromaMode);
//			//LOG( "OPT" ) << "przywracam stan" << std::endl;
//			//LOG_TAB "OPT" );
//			optimizePUsForChroma(cu);
//			//LOG_UNTAB "OPT" );
//		}
//	}
//
//	/**
//	* @fn	Void OptimalPartitioning::optimizePUForLuma( std::shared_ptr<PUIntra> pu )
//	*
//	* @brief	Wybór najlepszego trybu predykcji (spoœród 1 i 2 - ze wzglêdu na szybkoœæ dzia³ania)
//	*
//	* @param	pu	Aktualna PU.
//	*
//	* @return	Void.
//	*/
//
//	Void doPreselection(std::shared_ptr<PUIntra> pu)
//	{
//		currentPU = pu;
//		PBIntra* pb = new PBIntra(pu->getPicture(), LUMA, pu->getX(), pu->getY(), pu->getSize());
//		pb->basePredictionOnOriginalPicture(true);
//
//		UInt modes[19] = { 0, 1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 33, 34 };
//		Double costs[19];
//		std::map<double, std::vector<UInt>> results;
//		std::map<UInt, Sample**> predictions;
//
//		for (int modeIdx = 0; modeIdx < 19; ++modeIdx)
//		{
//			UInt lumaMode = modes[modeIdx];
//			pb->setModeIdx(lumaMode);
//
//			Sample** pred = IntraPred::getInstance()->calcPred(pb);
//			predictions[lumaMode] = pred;
//
//			RDCost::getInstance()->setDistortionFunction(RDCost::SAD);
//			costs[modeIdx]=RDCost::getInstance()->calcDistortion(Submatrix<Sample>(pred, pu->getSize()), Submatrix<Sample>(pu->getPicture()->getSamples(LUMA), pu->getSize()));
//			results[costs[modeIdx]].push_back(modes[modeIdx]);
//
//			deleteMatrix(pred, pu->getSize());
//		}
//		
//		pu->bestSAD8x8modes.clear();
//
//		for (auto it1 = results.begin(); it1 != results.end(); ++it1)
//		{
//			for (auto it2 = (*it1).second.begin(); it2 != (*it1).second.end(); ++it2)
//			{
//				pu->bestSAD8x8modes.push_back(*it2);
//				if (pu->bestSAD8x8modes.size() == 8)
//					break;
//			}
//			if (pu->bestSAD8x8modes.size() == 8)
//				break;
//		}
//
//
//
//		for (UInt modeIdx = 0; modeIdx < 19; ++modeIdx)
//		{
//			for (int i = 0; i < pu->getSize(); ++i)
//			{
//				delete[] predictions[modeIdx][i];
//			}
//			delete[] predictions[modeIdx];
//		}
//
//		delete pb;
//		
//		currentPU = nullptr;
//	}
//
//	/**
//	* @fn	Void OptimalPartitioning::optimizeTUTreeForLuma( std::shared_ptr<TUQuadTree> TUTree )
//	*
//	* @brief	Optymalizacja drzewa TU (NxN vs 2Nx2N).
//	*
//	* @param	TUTree	Aktualne drzewo TU.
//	*
//	* @return	Void.
//	*/
//
//	Void optimizeTUTreeForLuma(std::shared_ptr<TUQuadTree> TUTree)
//	{
//		//LOG( "OPT" ) << "TUTree(" << TUTree->getX( ) << "," << TUTree->getY( ) << ") " << TUTree->getSize( ) << "x" << TUTree->getSize( ) << ", optymalizacja Luma" << std::endl;
//
//		UInt multiPartDistortion = 0;
//		UInt multiPartBins = 0;
//
//		bool atLeastOneQuad = false;
//
//		Bool canMakeQuadPart = TUTree->getSize() > SeqParams()->getMinTUSize();
//		if (canMakeQuadPart)
//		{
//			//LOG( "OPT" ) << "proba dla multi" << std::endl;
//			//LOG_TAB "OPT" );
//			TUTree->createSubTrees();
//			for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
//			{
//				if (TUTree->getSubTree(position) != nullptr)
//				{
//					atLeastOneQuad = true;
//					optimizeTUTreeForLuma(TUTree->getSubTree(position));
//					multiPartBins += TUTree->getSubTree(position)->getTotalBins(LUMA);
//					multiPartDistortion += TUTree->getSubTree(position)->getTotalDistortion(LUMA);
//				}
//			}
//			//LOG_UNTAB "OPT" );
//			//LOG( "OPT" ) << "multiPartBins: " << multiPartBins << ", multiPartDistortion: " << multiPartDistortion << std::endl;
//		}
//
//		//LOG( "OPT" ) << "proba dla single" << std::endl;
//		std::shared_ptr<TU> leaf = std::make_shared<TU>(TUTree->getParentCU(), TUTree->getX(), TUTree->getY(), TUTree->getSize());
//		std::shared_ptr<TB> tb = leaf->getTB(LUMA);
//		tb->transformAndReconstruct(currentPU);
//		UInt singlePartDistortion = tb->getDistortion();
//		UInt singlePartBins = tb->getNumBins();
//
//		Double singleCost = RDCost::getInstance()->calcCost(singlePartBins, singlePartDistortion, QP[LUMA]);
//		Double multiCost = RDCost::getInstance()->calcCost(multiPartBins, multiPartDistortion, QP[LUMA]);
//		//LOG( "OPT" ) << "singlePartDistortion: " << singlePartDistortion << ", singlePartBins: " << singlePartBins << std::endl;
//		//LOG( "OPT" ) << "singleCost: " << singleCost << ", multiCost: " << multiCost << std::endl;
//		if (singleCost > multiCost && atLeastOneQuad)
//		{
//			//LOG( "OPT" ) << "wybieram tryb multi" << std::endl;
//			TUTree->setQTMode(QTMODE_SPLIT);
//			TUTree->setTotalBins(multiPartBins, LUMA);
//			TUTree->setTotalDistortion(multiPartDistortion, LUMA);
//			TUTree->setLeaf(nullptr);
//		}
//		else
//		{
//			//LOG( "OPT" ) << "wybieram tryb single" << std::endl;
//			TUTree->clear();
//			TUTree->setQTMode(QTMODE_LEAF);
//			TUTree->setTotalBins(singlePartBins, LUMA);
//			TUTree->setTotalDistortion(singlePartDistortion, LUMA);
//			TUTree->setLeaf(leaf);
//		}
//	}
//
//	/**
//	* @fn	Void OptimalPartitioning::optimizeTUTreeForChroma( std::shared_ptr<TUQuadTree> TUTree )
//	*
//	* @brief	Przeprowadzanie pêtli rekonstrukcji dla chrominancji w celu policzenia kosztu.
//	*
//	* @param	TUTree	Aktualne drzewo TU.
//	*
//	* @return	Void.
//	*/
//
//	Void optimizeTUTreeForChroma(std::shared_ptr<TUQuadTree> TUTree)
//	{
//		//LOG( "OPT" ) << "TUTree(" << TUTree->getX( ) << "," << TUTree->getY( ) << ") " << TUTree->getSize( ) << "x" << TUTree->getSize( ) << ", optymalizacja CHROMA" << std::endl;
//
//		if (TUTree->getQTMode() == QTMODE_SPLIT)
//		{
//			//LOG( "OPT" ) << "QTMODE_SPLIT" << std::endl;
//			UInt multiPartDistortionCr = 0, multiPartDistortionCb = 0;
//			UInt multiPartBinsCr = 0, multiPartBinsCb = 0;
//
//			if (TUTree->getSize() > 8)
//			{
//				//LOG( "OPT" ) << "size > 8, liczymy chroma dla czterech partycji" << std::endl;
//				//LOG_TAB "OPT" );
//				for (QTComponent position = QTCOMPONENT_FIRST; position <= QTCOMPONENT_LAST; ++position)
//				{
//					if (TUTree->getSubTree(position) != nullptr)
//					{
//						optimizeTUTreeForChroma(TUTree->getSubTree(position));
//						multiPartBinsCr += TUTree->getSubTree(position)->getTotalBins(CR);
//						multiPartBinsCb += TUTree->getSubTree(position)->getTotalBins(CB);
//						multiPartDistortionCr += TUTree->getSubTree(position)->getTotalDistortion(CR);
//						multiPartDistortionCb += TUTree->getSubTree(position)->getTotalDistortion(CB);
//					}
//				}
//				//LOG_UNTAB "OPT" );
//				//LOG( "OPT" ) << "multiPartBinsCr: " << multiPartBinsCr << ", multiPartDistortionCr" << multiPartDistortionCr << std::endl;
//				//LOG( "OPT" ) << "multiPartBinsCb: " << multiPartBinsCb << ", multiPartDistortionCb" << multiPartDistortionCb << std::endl;
//			}
//			else if (TUTree->getSize() == 8)
//			{
//				//LOG( "OPT" ) << "size == 8, liczymy chroma dla jednej partycji" << std::endl;
//				//LOG_TAB "OPT" );
//				optimizeTUTreeForChroma(TUTree->getSubTree(UPPERLEFT));
//				//LOG_UNTAB "OPT" );
//				multiPartBinsCr += TUTree->getSubTree(UPPERLEFT)->getTotalBins(CR);
//				multiPartBinsCb += TUTree->getSubTree(UPPERLEFT)->getTotalBins(CB);
//				multiPartDistortionCr += TUTree->getSubTree(UPPERLEFT)->getTotalDistortion(CR);
//				multiPartDistortionCb += TUTree->getSubTree(UPPERLEFT)->getTotalDistortion(CB);
//			}
//			TUTree->setTotalBins(multiPartBinsCr, CR);
//			TUTree->setTotalBins(multiPartBinsCb, CB);
//			TUTree->setTotalDistortion(multiPartDistortionCb, CB);
//			TUTree->setTotalDistortion(multiPartDistortionCr, CR);
//		}
//		else if (TUTree->getX() % 8 == 0 && TUTree->getY() % 8 == 0) // LEAF; jesli tu wejdziemy, to z poprzednich petli wynika, ze na pewno jest skladnik chroma
//		{
//			//LOG( "OPT" ) << "lisc! robimy petle" << std::endl;
//			std::shared_ptr<TB> tbCR = TUTree->getLeaf()->getTB(CR);
//			std::shared_ptr<TB> tbCB = TUTree->getLeaf()->getTB(CB);
//			//LOG_TAB "OPT" );
//			tbCR->transformAndReconstruct(currentPU);
//			TUTree->setTotalDistortion(tbCR->getDistortion(), CR);
//			TUTree->setTotalBins(tbCR->getNumBins(), CR);
//			tbCB->transformAndReconstruct(currentPU);
//			TUTree->setTotalDistortion(tbCB->getDistortion(), CB);
//			TUTree->setTotalBins(tbCB->getNumBins(), CB);
//			//LOG_UNTAB "OPT" );
//		}
//		else
//		{
//			//LOG( "OPT" ) << "lisc bez TB chroma" << std::endl;
//			TUTree->setTotalDistortion(0, CR);
//			TUTree->setTotalBins(0, CR);
//			TUTree->setTotalDistortion(0, CB);
//			TUTree->setTotalBins(0, CB);
//		}
//	}
//
//	/**
//	* @fn	Void OptimalPartitioning::optimizePUsForChroma( std::shared_ptr<CUIntra> cu )
//	*
//	* @brief	Przekazanie optymalizacji do drzewa TU przez wszystkie PU w danym CU.
//	*
//	* @param	cu	Aktualne CU.
//	*
//	* @return	Void.
//	*/
//
//	Void optimizePUsForChroma(std::shared_ptr<CUIntra> cu)
//	{
//		for (QTComponent i = QTCOMPONENT_FIRST; i <= QTCOMPONENT_LAST; ++i)
//		{
//			std::shared_ptr<PUIntra> pu = cu->getPU(i);
//			if (pu != nullptr)
//			{
//				currentPU = pu;
//				currentPU->refreshChromaModeIdx();
//				//LOG( "OPT" ) << "PU(" << pu->getX( ) << "," << pu->getY( ) << ") " << pu->getSize( ) << "x" << pu->getSize( ) << ", optymalizacja CHROMA" << std::endl;
//				//LOG_TAB "OPT" );
//				optimizeTUTreeForChroma(pu->getTransformArea());
//				//LOG_UNTAB "OPT" );
//				currentPU = nullptr;
//			}
//		}
//	}
//};
//#endif