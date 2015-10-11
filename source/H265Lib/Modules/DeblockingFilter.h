///**
//* @file	DeblockingFilter.h
//*
//* @brief	Deklaracje klasy filtra deblokuj¹cego
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
// * @brief Klasa filtra deblokuj¹cego.
// */
//
//class DeblockingFilter
//{
//private:
//
//	/**
//	 * @brief	Tablica wartoœci Beta prim ze standardu.
//	 */
//
//	static UShort BetaPrim[ 52 ];
//
//	/**
//	 * @brief	Tablica wartoœci tc prim ze standardu.
//	 */
//
//	static UShort TcPrim[ 54 ];
//
//	/**
//	 * @brief	Tablica wartoœci QPc ze standardu.
//	 */
//
//	static UShort QPc[ 14 ];
//
//	/**
//	 * @brief	Macierz krawêdzi przeznaczonych do filtracji.
//	 */
//
//	Bool** itsEdgeFlags;
//
//	/**
//	 * @brief	Macierz si³y filtracji na danej krawêdzi.
//	 */
//
//	UShort** itsFilterStrength;
//
//	/**
//	 * @brief	Zezwolenie na wiltracjê krawêdzi skrajnej (lewej lub górnej).
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
//	 * @brief	Aktualne wartoœci parametrów progowych Beta i tc oraz decyzji dE, dEp i dEq.
//	 *
//	 */
//
//	Short itsCurrentBeta, itsCurrentTc, itsCurrentdE, itsCurrentdEp, itsCurrentdEq;
//
//	/**
//	 * @property	Sample itsCurrentP[ 4 ], itsCurrentQ[ 4 ]
//	 *
//	 * @brief	Aktualnie przetwarzane próbki (maks. 4 z ka¿dej strony krawêdzi)
//	 *
//	 */
//
//	Sample itsCurrentP[ 4 ], itsCurrentQ[ 4 ];
//
//	/**
//	 * @property	UInt itsCurrentPX[ 3 ], itsCurrentPY[ 3 ], itsCurrentQX[ 3 ], itsCurrentQY[ 3 ]
//	 *
//	 * @brief	Wspó³rzêdne aktualnie przetwarzanych próbek.
//	 *
//	 */
//
//	UInt itsCurrentPX[ 3 ], itsCurrentPY[ 3 ], itsCurrentQX[ 3 ], itsCurrentQY[ 3 ];
//
//	/**
//	 * @property	UShort itsCurrentNDp, itsCurrentNDq
//	 *
//	 * @brief	Liczby próbek do nadpisania z ka¿dej strony krawêdzi.
//	 *
//	 */
//
//	UShort itsCurrentNDp, itsCurrentNDq;
//
//	/**
//	 * @property	Sample itsCurrentFilteredP[ 3 ], itsCurrentFilteredQ[ 3 ]
//	 *
//	 * @brief	Poprawione wartosci próbek.
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
//	 * @brief	Konstruktor domyœlny.
//	 */
//
//	DeblockingFilter( );
//
//	/**
//	 * @fn	Void DeblockingFilter::resetMatrices( );
//	 *
//	 * @brief	Wyzerowanie macierzy stanu, np. w celu przygotowania siê na nastêpn¹ jednostkê CU.
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
//	 * @brief	Dostêp do instancji filtru.
//	 *
//	 * @return	WskaŸnik na klasê filtru deblokuj¹cego.
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
//	 * @return	Wartoœæ qPc.
//	 */
//
//	static Short getQPc( Short qPi );
//
//	/**
//	 * @fn	Void DeblockingFilter::doFiltering( std::shared_ptr<Picture> pic )
//	 *
//	 * @brief	Filtruje obraz.
//	 *
//	 * @param	pic	WskaŸnik na obraz do filtracji.
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
//	 * @brief	Sprawdza, czy dla danej CU naleœy filtrowaæ skrajn¹ krawêdŸ.
//	 *
//	 * @param	cu			 	WskaŸnik na jednostkê CU do sprawdzenia.
//	 * @param	direction	Kierunek sprawdzenia.
//	 *
//	 * @return	true, jeœli krawêdŸ nale¿y poddaæ filtracji; false q p.p.
//	 */
//
//	Bool checkCUEdge( std::shared_ptr<CUIntra> cu, EdgeType direction );
//
//	/**
//	 * @fn	Void DeblockingFilter::doFiltering( std::shared_ptr<Picture> pic, EdgeType direction );
//	 *
//	 * @brief	Filtracja zadanych krawêdzi obrazu (pionowych lub poziomych)
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
//	 * @brief	Operacja filtracji dla ca³ego drzewa jednostek kodowych.
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
//	 * @brief	Zebranie informacji o krawêdziach miêdzy blokami transformacji.
//	 *
//	 * @param	TUtree	 	Korzeñ drzewa transformacji.
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
//	 * @brief	Zebranie informacji o krawêdziach miêdzy blokami predykcji.
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
//	 * @brief	Obliczenie si³y filtru dla wyznaczonych krawêdzi w danycm CU.
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
//	 * @brief	Filtracja krawêdzi.
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
//	 * @brief	Obliczenie decyzji o filtracji danego fragmentu krawêdzi.
//	 *
//	 * @param	cu			 	Aktualne Cu.
//	 * @param	x				 	Wspó³rzêdna X lewego/górnego piksela danej krawêdzi.
//	 * @param	y				 	Wspó³rzêdna Y lewego/górnego piksela danej krawêdzi.
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
//	 * @param	x				 	Wspó³rzêdna X lewego/górnego piksela danej krawêdzi.
//	 * @param	y				 	Wspó³rzêdna Y lewego/górnego piksela danej krawêdzi.
//	 * @param	direction	Kierunek filtracji.
//	 * @param	comp		 	Sk³adowa obrazu przeznaczona do filtracji.
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
//	 * @brief	Filtracja pionowych krawêdzi w macierzy luminancji.
//	 *
//	 * @param	cu	Aktualne CU.
//	 * @param	x				 	Wspó³rzêdna X lewego/górnego piksela danej krawêdzi.
//	 * @param	y				 	Wspó³rzêdna Y lewego/górnego piksela danej krawêdzi..
//	 *
//	 * @return	Void.
//	 */
//
//	Void doLumaFilteringVertical( std::shared_ptr<CUIntra> cu, UInt x, UInt y );
//
//	/**
//	 * @fn	Void DeblockingFilter::doLumaFilteringHorizontal( std::shared_ptr<CUIntra> cu, UInt x, UInt y );
//	 *
//	 * @brief	Filtracja poziomych krawêdzi w macierzy luminancji.
//	 *
//	 * @param	cu	Aktualne CU.
//	 * @param	x				 	Wspó³rzêdna X lewego/górnego piksela danej krawêdzi.
//	 * @param	y				 	Wspó³rzêdna Y lewego/górnego piksela danej krawêdzi.
//	 *
//	 * @return	Void.
//	 */
//
//	Void doLumaFilteringHorizontal( std::shared_ptr<CUIntra> cu, UInt x, UInt y );
//
//	/**
//	 * @fn	Bool DeblockingFilter::decideFilterLuma( Sample p0, Sample p3, Sample q0, Sample q3, Sample dpq, Short beta, Short tc );
//	 *
//	 * @brief	Podjêcie decyzji o filtracji fragmentu krawêdzi luminancji.
//	 *
//	 * @param	p0		Próbka tu¿ przy krawêdzi w aktualnym bloku.
//	 * @param	p3		Próbka najdalej od krawêdzi w aktualnym bloku.
//	 * @param	q0		Próbka tu¿ przy krawêdzi w s¹siednim bloku.
//	 * @param	q3		Próbka najdalej od krawêdzi w s¹siednim bloku.
//	 * @param	dpq 	Decyzja pomocnicza dpq.
//	 * @param	beta	Beta.
//	 * @param	tc		tc.
//	 *
//	 * @return	true jeœli nale¿y filtrowaæ dane miejsce na krawêdzi; false w p.p.
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
//	 * @brief	FIltracja próbke chrominancji.
//	 *
//	 * @param	cu						 	Aktualna CU.
//	 * @param	p							 	Próbki przy krawêdzi w aktualnym bloku.
//	 * @param	q							 	Próbki przy krawêdzi w s¹siednim bloku.
//	 * @param	xP						 	Wspó³rzêdna X próbki p[0].
//	 * @param	yP						  Wspó³rzêdna Y próbki p[0].
//	 * @param	xQ						 	Wspó³rzêdna X próbki q[0].
//	 * @param	yQ						 	Wspó³rzêdna Y próbki q[0].
//	 * @param	comp					 	Sk³¹dowa obrazu.
//	 * @param [out]	out_p	Poprawiona wartosæ p[0].
//	 * @param [out]	out_q	Poprawiona wartosæ q[0].
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