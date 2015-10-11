///**
//* @file	IntraPred.h
//*
//* @brief	Deklaracje klasy predyktora intra
//*/
//#ifndef INTRA_PRED_H
//#define INTRA_PRED_H
//
//#include <cassert>
//#include <algorithm>
//
//#include "Utils.h"
//#include "PU.h"
//#include "IntraMode.h"
//
///**
// * @class	IntraPred
// *
// * @brief	Kalkulator predykcji intra
// */
//
//class IntraPred
//{
//private:
//
//	/**
//	 * @brief	Instancja kalkulatora.
//	 */
//
//	static IntraPred *itsInstance;
//
//	/**
//	 * @brief	Wska�niki na 4 strategie do wyboru trybu.
//	 */
//
//	IntraMode **itsModes;
//
//	/**
//	 * @brief	Aktualny blok predykcji.
//	 */
//
//	PBIntra* itsCurrentPB;
//
//	/**
//	 * @brief	Lewa-g�rna wartos� naro�na.
//	 */
//
//	Sample itsCornerValue;
//
//	/**
//	 * @brief	Pr�bki referencyjne (w dw�ch rz�dach, pionowym i poziomym).
//	 */
//
//	Sample** itsReferenceValues;
//
//	/**
//	 * @fn	IntraPred::IntraPred( );
//	 *
//	 * @brief	Konstruktor domy�lny.
//	 */
//
//	IntraPred( );
//
//	/**
//	 * @fn	UShort IntraPred::getFilteringThreshold( ) const;
//	 *
//	 * @brief	Pobranie progu filtracji.
//	 *
//	 * @return	Pr�g filtracji.
//	 */
//
//	UShort getFilteringThreshold( ) const;
//
//	/**
//	 * @fn	Bool IntraPred::isFilteringRequired( ) const;
//	 *
//	 * @brief	Sprawdzenie, czy na danym bloku trzeba wykona� filtracj� pr�bek referencyjnych.
//	 *
//	 * @return	Prawda, je�li filtracja jest wymagana; fa�sz w p.p.
//	 */
//
//	Bool isFilteringRequired( ) const;
//
//	/**
//	 * @fn	Sample IntraPred::filtRef( const Sample mainRef, const Sample leftRef, const Sample rightRef ) const;
//	 *
//	 * @brief	Filtracja pr�bki referencyjnej.
//	 *
//	 * @param	mainRef 	FIltrowana pr�bka.
//	 * @param	leftRef 	Poprzendia pr�bka.
//	 * @param	rightRef	TNast�pna pr�bka.
//	 *
//	 * @return	Poprawiona warto�� pr�bki g��wnej.
//	 */
//
//	Sample filtRef( const Sample mainRef, const Sample leftRef, const Sample rightRef ) const;
//
//	/**
//	 * @fn	Void IntraPred::filterSideRefs( const IntraDirection referenceDirection );
//	 *
//	 * @brief	Filtracja wszystkich pr�bek referencyjnych po zadanej stronie.
//	 *
//	 * @param	referenceDirection	Strona , po kt�rej fr�bki zostan� odfiltrowane.
//	 *
//	 * @return	Void.
//	 */
//
//	Void filterSideRefs( const IntraDirection referenceDirection );
//
//	/**
//	 * @fn	Void IntraPred::doReferenceFiltering( );
//	 *
//	 * @brief	Filtracja pr�bek referencyjnych.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doReferenceFiltering( );
//
//	/**
//	 * @fn	Bool IntraPred::checkSmoothConditions( const IntraDirection direction ) const;
//	 *
//	 * @brief	Sprawdzenie mo�liwo�ci silnej filtracji pr�bek w zadanym kierunku.
//	 *
//	 * @param	Kierunek, na kt�rym le�� rozwa�ane pr�bki. 
//	 *
//	 * @return	true, jesli potrzebna jest silna filtracja; false w p.p.
//	 */
//
//	Bool checkSmoothConditions( const IntraDirection direction ) const;
//
//	/**
//	 * @fn	Bool IntraPred::isSmoothingRequired( ) const;
//	 *
//	 * @brief	Sprawdzenie konieczno�ci silnej filtracji pr�bek.
//	 *
//	 * @return	true je�li nale�y je wyg�adzi�, false w p.p..
//	 */
//
//	Bool isSmoothingRequired( ) const;
//
//	/**
//	 * @fn	Sample IntraPred::getSmoothedReferenceAtPosition( const IntraDirection dir, const UShort offset ) const;
//	 *
//	 * @brief	Pobranie silnie wyg�adzonej pr�bki na danej pozycji.
//	 *
//	 * @param	dir			Kierunek, na kt�rym le�� pr�bki.
//	 * @param	offset	Przesuniecie pr�bki wzgl�dem lewego g�rnego rogu rozwa�anego bloku.
//	 *
//	 * @return	Silnie wyg�adzona pr�bka na zadanej pozycji.
//	 */
//
//	Sample getSmoothedReferenceAtPosition( const IntraDirection dir, const UShort offset ) const;
//
//	/**
//	 * @fn	Void IntraPred::smoothSideRefs( const IntraDirection dir );
//	 *
//	 * @brief	Silna filtracja pr�bek w danym kierunku
//	 *
//	 * @param	dir	Kierunek rozwa�anych pr�bek
//	 *
//	 * @return	Void.
//	 */
//
//	Void smoothSideRefs( const IntraDirection dir );
//
//	/**
//	 * @fn	Void IntraPred::doReferenceSmoothing( );
//	 *
//	 * @brief	Bazowa metoda wyg�adzaj�ca do wywo�ania przy zliczaniu bit�w.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doReferenceSmoothing( );
//
//	/**
//	 * @fn	IntraMode* IntraPred::getPredictionStrategy( );
//	 *
//	 * @brief	Wybranie strategii predykcji.
//	 *
//	 * @return	Wska�nik na strategi� predykcji.
//	 */
//
//	IntraMode* getPredictionStrategy( );
//public:
//
//	/**
//	 * @fn	IntraPred::~IntraPred( );
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	~IntraPred( );
//
//	/**
//	 * @fn	static IntraPred IntraPred::*getInstance( );
//	 *
//	 * @brief	Dost�p do instancji.
//	 *
//	 * @return	Wska�nik na instancj�.
//	 */
//
//	static IntraPred *getInstance( );
//
//	/**
//	 * @fn	Sample** IntraPred::calcPred( PBIntra* targetPB );
//	 *
//	 * @brief	Policzenie predykcji dla zadanego bloku.
//	 *
//	 * @param [in]	targetPB	blok predykcji, na bazie kt�rego zachodz� obliczenia.
//	 *
//	 * @return	Wska�nik na macierz predykcji.
//	 */
//
//	Sample** calcPred( PBIntra* targetPB );
//
//	/**
//	 * @fn	Sample** IntraPred::calcPredForceRefs( PBIntra* tergetPB, Sample* leftRefs, Sample* topRefs, const Sample corner );
//	 *
//	 * @brief	Policzenie predykcji dla danego bloku z wymuszeniem pr�bek referencyjnych.
//	 *
//	 * @param [in,out]	tergetPB	Wska�nik na rozpatrywany blok ptransformacji.
//	 * @param [in,out]	leftRefs	Tablica pr�bek z lewej.
//	 * @param [in,out]	topRefs 	Tablica pr�bek powy�ej.
//	 * @param	corner							Warto�� naro�nika.
//	 *
//	 * @return	Macierz predykcji.
//	 */
//
//	Sample** calcPredForceRefs( PBIntra* tergetPB, Sample* leftRefs, Sample* topRefs, const Sample corner );
//
//};
//
//#endif