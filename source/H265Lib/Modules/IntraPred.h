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
//	 * @brief	WskaŸniki na 4 strategie do wyboru trybu.
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
//	 * @brief	Lewa-górna wartosæ naro¿na.
//	 */
//
//	Sample itsCornerValue;
//
//	/**
//	 * @brief	Próbki referencyjne (w dwóch rzêdach, pionowym i poziomym).
//	 */
//
//	Sample** itsReferenceValues;
//
//	/**
//	 * @fn	IntraPred::IntraPred( );
//	 *
//	 * @brief	Konstruktor domyœlny.
//	 */
//
//	IntraPred( );
//
//	/**
//	 * @fn	UShort IntraPred::getFilteringThreshold( ) const;
//	 *
//	 * @brief	Pobranie progu filtracji.
//	 *
//	 * @return	Próg filtracji.
//	 */
//
//	UShort getFilteringThreshold( ) const;
//
//	/**
//	 * @fn	Bool IntraPred::isFilteringRequired( ) const;
//	 *
//	 * @brief	Sprawdzenie, czy na danym bloku trzeba wykonaæ filtracjê próbek referencyjnych.
//	 *
//	 * @return	Prawda, jeœli filtracja jest wymagana; fa³sz w p.p.
//	 */
//
//	Bool isFilteringRequired( ) const;
//
//	/**
//	 * @fn	Sample IntraPred::filtRef( const Sample mainRef, const Sample leftRef, const Sample rightRef ) const;
//	 *
//	 * @brief	Filtracja próbki referencyjnej.
//	 *
//	 * @param	mainRef 	FIltrowana próbka.
//	 * @param	leftRef 	Poprzendia próbka.
//	 * @param	rightRef	TNastêpna próbka.
//	 *
//	 * @return	Poprawiona wartoœæ próbki g³ównej.
//	 */
//
//	Sample filtRef( const Sample mainRef, const Sample leftRef, const Sample rightRef ) const;
//
//	/**
//	 * @fn	Void IntraPred::filterSideRefs( const IntraDirection referenceDirection );
//	 *
//	 * @brief	Filtracja wszystkich próbek referencyjnych po zadanej stronie.
//	 *
//	 * @param	referenceDirection	Strona , po której fróbki zostan¹ odfiltrowane.
//	 *
//	 * @return	Void.
//	 */
//
//	Void filterSideRefs( const IntraDirection referenceDirection );
//
//	/**
//	 * @fn	Void IntraPred::doReferenceFiltering( );
//	 *
//	 * @brief	Filtracja próbek referencyjnych.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doReferenceFiltering( );
//
//	/**
//	 * @fn	Bool IntraPred::checkSmoothConditions( const IntraDirection direction ) const;
//	 *
//	 * @brief	Sprawdzenie mo¿liwoœci silnej filtracji próbek w zadanym kierunku.
//	 *
//	 * @param	Kierunek, na którym le¿¹ rozwa¿ane próbki. 
//	 *
//	 * @return	true, jesli potrzebna jest silna filtracja; false w p.p.
//	 */
//
//	Bool checkSmoothConditions( const IntraDirection direction ) const;
//
//	/**
//	 * @fn	Bool IntraPred::isSmoothingRequired( ) const;
//	 *
//	 * @brief	Sprawdzenie koniecznoœci silnej filtracji próbek.
//	 *
//	 * @return	true jeœli nale¿y je wyg³adziæ, false w p.p..
//	 */
//
//	Bool isSmoothingRequired( ) const;
//
//	/**
//	 * @fn	Sample IntraPred::getSmoothedReferenceAtPosition( const IntraDirection dir, const UShort offset ) const;
//	 *
//	 * @brief	Pobranie silnie wyg³adzonej próbki na danej pozycji.
//	 *
//	 * @param	dir			Kierunek, na którym le¿¹ próbki.
//	 * @param	offset	Przesuniecie próbki wzglêdem lewego górnego rogu rozwa¿anego bloku.
//	 *
//	 * @return	Silnie wyg³adzona próbka na zadanej pozycji.
//	 */
//
//	Sample getSmoothedReferenceAtPosition( const IntraDirection dir, const UShort offset ) const;
//
//	/**
//	 * @fn	Void IntraPred::smoothSideRefs( const IntraDirection dir );
//	 *
//	 * @brief	Silna filtracja próbek w danym kierunku
//	 *
//	 * @param	dir	Kierunek rozwa¿anych próbek
//	 *
//	 * @return	Void.
//	 */
//
//	Void smoothSideRefs( const IntraDirection dir );
//
//	/**
//	 * @fn	Void IntraPred::doReferenceSmoothing( );
//	 *
//	 * @brief	Bazowa metoda wyg³adzaj¹ca do wywo³ania przy zliczaniu bitów.
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
//	 * @return	WskaŸnik na strategiê predykcji.
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
//	 * @brief	Dostêp do instancji.
//	 *
//	 * @return	WskaŸnik na instancjê.
//	 */
//
//	static IntraPred *getInstance( );
//
//	/**
//	 * @fn	Sample** IntraPred::calcPred( PBIntra* targetPB );
//	 *
//	 * @brief	Policzenie predykcji dla zadanego bloku.
//	 *
//	 * @param [in]	targetPB	blok predykcji, na bazie którego zachodz¹ obliczenia.
//	 *
//	 * @return	WskaŸnik na macierz predykcji.
//	 */
//
//	Sample** calcPred( PBIntra* targetPB );
//
//	/**
//	 * @fn	Sample** IntraPred::calcPredForceRefs( PBIntra* tergetPB, Sample* leftRefs, Sample* topRefs, const Sample corner );
//	 *
//	 * @brief	Policzenie predykcji dla danego bloku z wymuszeniem próbek referencyjnych.
//	 *
//	 * @param [in,out]	tergetPB	WskaŸnik na rozpatrywany blok ptransformacji.
//	 * @param [in,out]	leftRefs	Tablica próbek z lewej.
//	 * @param [in,out]	topRefs 	Tablica próbek powy¿ej.
//	 * @param	corner							Wartoœæ naro¿nika.
//	 *
//	 * @return	Macierz predykcji.
//	 */
//
//	Sample** calcPredForceRefs( PBIntra* tergetPB, Sample* leftRefs, Sample* topRefs, const Sample corner );
//
//};
//
//#endif