///**
//* @file	DeblockingFilter.h
//*
//* @brief	Deklaracje klasy filtra deblokuj�cego
//*/
//#ifndef DeblockingFilter_h__
//#define DeblockingFilter_h__
//
//#include "Utils.h"
//#include "Picture.h"
//#include "CU.h"
//#include "Slice.h"
//#include "Tile.h"
//#include "QuadTree.h"
//
///**
// * @class	DeblockingFilter
// *
// * @brief Klasa filtra deblokuj�cego.
// */
//
//class DeblockingFilter
//{
//private:
//
//	/**
//	 * @brief	Tablica warto�ci Beta prim ze standardu.
//	 */
//
//	static UShort BetaPrim[ 52 ];
//
//	/**
//	 * @brief	Tablica warto�ci tc prim ze standardu.
//	 */
//
//	static UShort TcPrim[ 54 ];
//
//	/**
//	 * @brief	Tablica warto�ci QPc ze standardu.
//	 */
//
//	static UShort QPc[ 14 ];
//
//	/**
//	 * @brief	Macierz kraw�dzi przeznaczonych do filtracji.
//	 */
//
//	Bool** itsEdgeFlags;
//
//	/**
//	 * @brief	Macierz si�y filtracji na danej kraw�dzi.
//	 */
//
//	UShort** itsFilterStrength;
//
//	/**
//	 * @brief	Zezwolenie na wiltracj� kraw�dzi skrajnej (lewej lub g�rnej).
//	 */
//
//	Bool itsShouldFilterCBEdge;
//
//	/**
//	 * @brief	Rozmiar aktualnego bloku.
//	 */
//
//	UInt itsCurrentSize;
//
//	/**
//	 * @property	Short itsCurrentBeta, itsCurrentTc, itsCurrentdE, itsCurrentdEp, itsCurrentdEq
//	 *
//	 * @brief	Aktualne warto�ci parametr�w progowych Beta i tc oraz decyzji dE, dEp i dEq.
//	 *
//	 */
//
//	Short itsCurrentBeta, itsCurrentTc, itsCurrentdE, itsCurrentdEp, itsCurrentdEq;
//
//	/**
//	 * @property	Sample itsCurrentP[ 4 ], itsCurrentQ[ 4 ]
//	 *
//	 * @brief	Aktualnie przetwarzane pr�bki (maks. 4 z ka�dej strony kraw�dzi)
//	 *
//	 */
//
//	Sample itsCurrentP[ 4 ], itsCurrentQ[ 4 ];
//
//	/**
//	 * @property	UInt itsCurrentPX[ 3 ], itsCurrentPY[ 3 ], itsCurrentQX[ 3 ], itsCurrentQY[ 3 ]
//	 *
//	 * @brief	Wsp�rz�dne aktualnie przetwarzanych pr�bek.
//	 *
//	 */
//
//	UInt itsCurrentPX[ 3 ], itsCurrentPY[ 3 ], itsCurrentQX[ 3 ], itsCurrentQY[ 3 ];
//
//	/**
//	 * @property	UShort itsCurrentNDp, itsCurrentNDq
//	 *
//	 * @brief	Liczby pr�bek do nadpisania z ka�dej strony kraw�dzi.
//	 *
//	 */
//
//	UShort itsCurrentNDp, itsCurrentNDq;
//
//	/**
//	 * @property	Sample itsCurrentFilteredP[ 3 ], itsCurrentFilteredQ[ 3 ]
//	 *
//	 * @brief	Poprawione wartosci pr�bek.
//	 *
//	 */
//
//	Sample itsCurrentFilteredP[ 3 ], itsCurrentFilteredQ[ 3 ];
//
//	/**
//	 * @brief	Instancja singletona filtru.
//	 */
//
//	static DeblockingFilter* instance;
//
//	/**
//	 * @fn	DeblockingFilter::DeblockingFilter( );
//	 *
//	 * @brief	Konstruktor domy�lny.
//	 */
//
//	DeblockingFilter( );
//
//	/**
//	 * @fn	Void DeblockingFilter::resetMatrices( );
//	 *
//	 * @brief	Wyzerowanie macierzy stanu, np. w celu przygotowania si� na nast�pn� jednostk� CU.
//	 *
//	 * @return	Void.
//	 */
//
//	Void resetMatrices( );
//public:
//
//	/**
//	 * @fn	static DeblockingFilter* DeblockingFilter::getInstance( );
//	 *
//	 * @brief	Dost�p do instancji filtru.
//	 *
//	 * @return	Wska�nik na klas� filtru deblokuj�cego.
//	 */
//
//	static DeblockingFilter* getInstance( );
//
//	/**
//	 * @fn	DeblockingFilter::~DeblockingFilter( );
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	~DeblockingFilter( );
//
//	/**
//	 * @fn	static Short DeblockingFilter::getQPc( Short qPi );
//	 *
//	 * @brief	Oblicza QPc na podstawie QPi (tabela w standardzie)
//	 *
//	 * @param	qPi	Parametr qPi.
//	 *
//	 * @return	Warto�� qPc.
//	 */
//
//	static Short getQPc( Short qPi );
//
//	/**
//	 * @fn	Void DeblockingFilter::doFiltering( std::shared_ptr<Picture> pic )
//	 *
//	 * @brief	Filtruje obraz.
//	 *
//	 * @param	pic	Wska�nik na obraz do filtracji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doFiltering( std::shared_ptr<Picture> pic )
//	{
//		//LOG( "FDB" ) << "FILTRACJA VER" << std::endl;
//		//LOG_TAB "FDB" );
//		doFiltering( pic, EDGE_VER );
//		//LOG_UNTAB "FDB" );
//		//LOG( "FDB" ) << "FILTRACJA HOR" << std::endl;
//		//LOG_TAB "FDB" );
//		doFiltering( pic, EDGE_HOR );
//		//LOG_UNTAB "FDB" );
//	}
//
//	/**
//	 * @fn	Bool DeblockingFilter::checkCUEdge( std::shared_ptr<CUIntra> cu, EdgeType direction );
//	 *
//	 * @brief	Sprawdza, czy dla danej CU nale�y filtrowa� skrajn� kraw�d�.
//	 *
//	 * @param	cu			 	Wska�nik na jednostk� CU do sprawdzenia.
//	 * @param	direction	Kierunek sprawdzenia.
//	 *
//	 * @return	true, je�li kraw�d� nale�y podda� filtracji; false q p.p.
//	 */
//
//	Bool checkCUEdge( std::shared_ptr<CUIntra> cu, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::doFiltering( std::shared_ptr<Picture> pic, EdgeType direction );
//	 *
//	 * @brief	Filtracja zadanych kraw�dzi obrazu (pionowych lub poziomych)
//	 *
//	 * @param	pic			 	Obraz do filtracji.
//	 * @param	direction	Kierunek filtracji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doFiltering( std::shared_ptr<Picture> pic, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::doFiltering( std::shared_ptr<CUQuadTree> qt, EdgeType direction );
//	 *
//	 * @brief	Operacja filtracji dla ca�ego drzewa jednostek kodowych.
//	 *
//	 * @param	qt			 	Drzewo CU.
//	 * @param	direction Kierunek filtracji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doFiltering( std::shared_ptr<CUQuadTree> qt, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::doFiltering( std::shared_ptr<CUIntra> cu, EdgeType direction );
//	 *
//	 * @brief	POdstawowe filtrowanie (na bazie CU).
//	 *
//	 * @param	cu			 	Jendostka CU poddawana filtracji.
//	 * @param	direction	Kierunek filtracji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doFiltering( std::shared_ptr<CUIntra> cu, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::fillInTransformEdges( std::shared_ptr<TUQuadTree> TUtree, EdgeType direction );
//	 *
//	 * @brief	Zebranie informacji o kraw�dziach mi�dzy blokami transformacji.
//	 *
//	 * @param	TUtree	 	Korze� drzewa transformacji.
//	 * @param	direction	Kierunek filtracji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void fillInTransformEdges( std::shared_ptr<TUQuadTree> TUtree, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::fillInPredictionEdges( std::shared_ptr<CUIntra> cu, EdgeType direction );
//	 *
//	 * @brief	Zebranie informacji o kraw�dziach mi�dzy blokami predykcji.
//	 *
//	 * @param	cu			 	Jendostka kodowa zawierajaca blok(i) predykcji.
//	 * @param	direction	Kierunek filtracji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void fillInPredictionEdges( std::shared_ptr<CUIntra> cu, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::calcFilterStrength( std::shared_ptr<CUIntra> cu, EdgeType direction );
//	 *
//	 * @brief	Obliczenie si�y filtru dla wyznaczonych kraw�dzi w danycm CU.
//	 *
//	 * @param	cu			 	Aktualne CU.
//	 * @param	direction	Kierunek filtracji.
//	 *
//	 */
//
//	Void calcFilterStrength( std::shared_ptr<CUIntra> cu, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::filtreEdges( std::shared_ptr<CUIntra> cu, EdgeType direction );
//	 *
//	 * @brief	Filtracja kraw�dzi.
//	 *
//	 * @param	cu			 	Aktualne CU.
//	 * @param	direction	Kierunek filtracji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void filtreEdges( std::shared_ptr<CUIntra> cu, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::caclDecisions( std::shared_ptr<CUIntra> cu, UInt x, UInt y, EdgeType direction );
//	 *
//	 * @brief	Obliczenie decyzji o filtracji danego fragmentu kraw�dzi.
//	 *
//	 * @param	cu			 	Aktualne Cu.
//	 * @param	x				 	Wsp�rz�dna X lewego/g�rnego piksela danej kraw�dzi.
//	 * @param	y				 	Wsp�rz�dna Y lewego/g�rnego piksela danej kraw�dzi.
//	 * @param	direction	Kierunek filtracji.
//	 *
//	 * @return	Void.
//	 */
//
//	Void caclDecisions( std::shared_ptr<CUIntra> cu, UInt x, UInt y, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::doChromaFiltering( std::shared_ptr<CUIntra> cu, UInt x, UInt y, EdgeType direction, ImgComp comp, Short qpOffset = 0 );
//	 *
//	 * @brief	Filtracja chrominancji.
//	 *
//	 * @param	cu			 	Aktualne CU.
//	 * @param	x				 	Wsp�rz�dna X lewego/g�rnego piksela danej kraw�dzi.
//	 * @param	y				 	Wsp�rz�dna Y lewego/g�rnego piksela danej kraw�dzi.
//	 * @param	direction	Kierunek filtracji.
//	 * @param	comp		 	Sk�adowa obrazu przeznaczona do filtracji.
//	 * @param	qpOffset 	Zmiana QP na poziomie CU.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doChromaFiltering( std::shared_ptr<CUIntra> cu, UInt x, UInt y, EdgeType direction, ImgComp comp, Short qpOffset = 0 );
//
//	/**
//	 * @fn	Void DeblockingFilter::doLumaFilteringVertical( std::shared_ptr<CUIntra> cu, UInt x, UInt y );
//	 *
//	 * @brief	Filtracja pionowych kraw�dzi w macierzy luminancji.
//	 *
//	 * @param	cu	Aktualne CU.
//	 * @param	x				 	Wsp�rz�dna X lewego/g�rnego piksela danej kraw�dzi.
//	 * @param	y				 	Wsp�rz�dna Y lewego/g�rnego piksela danej kraw�dzi..
//	 *
//	 * @return	Void.
//	 */
//
//	Void doLumaFilteringVertical( std::shared_ptr<CUIntra> cu, UInt x, UInt y );
//
//	/**
//	 * @fn	Void DeblockingFilter::doLumaFilteringHorizontal( std::shared_ptr<CUIntra> cu, UInt x, UInt y );
//	 *
//	 * @brief	Filtracja poziomych kraw�dzi w macierzy luminancji.
//	 *
//	 * @param	cu	Aktualne CU.
//	 * @param	x				 	Wsp�rz�dna X lewego/g�rnego piksela danej kraw�dzi.
//	 * @param	y				 	Wsp�rz�dna Y lewego/g�rnego piksela danej kraw�dzi.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doLumaFilteringHorizontal( std::shared_ptr<CUIntra> cu, UInt x, UInt y );
//
//	/**
//	 * @fn	Bool DeblockingFilter::decideFilterLuma( Sample p0, Sample p3, Sample q0, Sample q3, Sample dpq, Short beta, Short tc );
//	 *
//	 * @brief	Podj�cie decyzji o filtracji fragmentu kraw�dzi luminancji.
//	 *
//	 * @param	p0		Pr�bka tu� przy kraw�dzi w aktualnym bloku.
//	 * @param	p3		Pr�bka najdalej od kraw�dzi w aktualnym bloku.
//	 * @param	q0		Pr�bka tu� przy kraw�dzi w s�siednim bloku.
//	 * @param	q3		Pr�bka najdalej od kraw�dzi w s�siednim bloku.
//	 * @param	dpq 	Decyzja pomocnicza dpq.
//	 * @param	beta	Beta.
//	 * @param	tc		tc.
//	 *
//	 * @return	true je�li nale�y filtrowa� dane miejsce na kraw�dzi; false w p.p.
//	 */
//
//	Bool decideFilterLuma( Sample p0, Sample p3, Sample q0, Sample q3, Sample dpq, Short beta, Short tc );
//
//	/**
//	 * @fn	Void DeblockingFilter::filteringProcess( std::shared_ptr<CUIntra> cu );
//	 *
//	 * @brief	Proces filtracji dla jednostki CU.
//	 *
//	 * @param	cu	Aktualne CU
//	 *
//	 * @return	Void.
//	 */
//
//	Void filteringProcess( std::shared_ptr<CUIntra> cu );
//
//	/**
//	 * @fn	Void DeblockingFilter::filterChromaSamples( std::shared_ptr<CUIntra> cu, Sample p[ 2 ], Sample q[ 2 ], UInt xP, UInt yP, UInt xQ, UInt yQ, ImgComp comp, Sample& out_p, Sample& out_q );
//	 *
//	 * @brief	FIltracja pr�bke chrominancji.
//	 *
//	 * @param	cu						 	Aktualna CU.
//	 * @param	p							 	Pr�bki przy kraw�dzi w aktualnym bloku.
//	 * @param	q							 	Pr�bki przy kraw�dzi w s�siednim bloku.
//	 * @param	xP						 	Wsp�rz�dna X pr�bki p[0].
//	 * @param	yP						  Wsp�rz�dna Y pr�bki p[0].
//	 * @param	xQ						 	Wsp�rz�dna X pr�bki q[0].
//	 * @param	yQ						 	Wsp�rz�dna Y pr�bki q[0].
//	 * @param	comp					 	Sk��dowa obrazu.
//	 * @param [out]	out_p	Poprawiona wartos� p[0].
//	 * @param [out]	out_q	Poprawiona wartos� q[0].
//	 *
//	 * @return	A Void.
//	 */
//
//	Void filterChromaSamples( std::shared_ptr<CUIntra> cu, Sample p[ 2 ], Sample q[ 2 ], UInt xP, UInt yP, UInt xQ, UInt yQ, ImgComp comp, Sample& out_p, Sample& out_q );
//
//};
//
//
//#endif // DeblockingFilter_h__