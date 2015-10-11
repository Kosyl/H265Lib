///**
//* @file	IntraMode.h
//*
//* @brief	Deklaracje klasy strategii intra
//*/
//#ifndef INTRA_MODE_H
//#define INTRA_MODE_H
//
//#include <cassert>
//
//class PBIntra;
//
///**
// * @class	IntraMode
// *
// * @brief	Klasa strategii predykcji intra.
// */
//
//class IntraMode
//{
//protected:
//
//	/**
//	 * @brief	Aktualny blok predykcji.
//	 */
//
//	PBIntra *itsCurrentPB;
//
//	/**
//	 * @brief	Próbka "naro¿na"
//	 */
//
//	Sample itsCornerReference;
//
//	/**
//	 * @brief	Dwuwymiarowa macierz próbek referencyjncyh.
//	 */
//
//	Sample** itsReferenceValues;
//
//	/**
//	 * @fn	Sample** IntraMode::initPred( ) const;
//	 *
//	 * @brief	Inicjalizacja macierzy predykcji.
//	 *
//	 * @return	WskaŸnik na macierz predykcji.
//	 */
//
//	Sample** initPred( ) const;
//public:
//
//	/**
//	 * @fn	IntraMode::IntraMode( );
//	 *
//	 * @brief	Konstruktor domyœlny.
//	 */
//
//	IntraMode( );
//
//	/**
//	 * @fn	virtual IntraMode::~IntraMode( );
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	virtual ~IntraMode( );
//
//	/**
//	 * @fn	virtual Sample** IntraMode::calcPred( ) = 0;
//	 *
//	 * @brief	Metoda licz¹ca predykcjê.
//	 *
//	 * @return	WskaŸnik na macierz wynikow¹ z sygna³em predykcji.
//	 */
//
//	virtual Sample** calcPred( ) = 0;
//
//	/**
//	 * @fn	virtual Void IntraMode::setPb( PBIntra* PB );
//	 *
//	 * @brief	Ustawia aktualny blok predykcji.
//	 *
//	 * @param [in]	WskaŸnik na blok predykcji.
//	 *
//	 * @return	Void.
//	 */
//
//	virtual Void setPb( PBIntra* PB );
//
//	/**
//	 * @fn	virtual Void IntraMode::setCorner( const Sample corner );
//	 *
//	 * @brief Ustawia wartosæ naroznej próbki referencyjnej.
//	 *
//	 * @param	corner Wartoœæ próbki.
//	 *
//	 * @return	Void.
//	 */
//
//	virtual Void setCorner( const Sample corner );
//
//	/**
//	 * @fn	virtual Void IntraMode::setSideRefs( const IntraDirection dir, Sample* ref );
//	 *
//	 * @brief	Ustawia podan¹ tablicê próbek jako referencyjne dla danego kierunku.
//	 *
//	 * @param	dir					 	Wskazanie, czy s¹ to próbki z lewej czy z góry.
//	 * @param [in] ref	WskaŸnik na tablicê próbek referencyjnych.
//	 *
//	 * @return	Void.
//	 */
//
//	virtual Void setSideRefs( const IntraDirection dir, Sample* ref );
//
//};
//
///**
// * @class	PlanarMode
// *
// * @brief	Strategia planarna.
// */
//
//class PlanarMode : public IntraMode
//{
//public:
//
//	/**
//	 * @fn	PlanarMode::PlanarMode( );
//	 *
//	 * @brief Konstruktor domyœlny.
//	 */
//
//	PlanarMode( );
//
//	/**
//	 * @fn	PlanarMode::~PlanarMode( );
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	~PlanarMode( );
//
//	/**
//	 * @fn	Sample PlanarMode::**calcPred( );
//	 *
//	 * @brief	Obliczenie predykcji w trybie Planar.
//	 *
//	 * @return	Macierz na sygna³ predykcji.
//	 */
//
//	Sample **calcPred( );
//
//};
//
///**
// * @class	DcMode
// *
// * @brief	Strategia trybu DC.
// */
//
//class DcMode : public IntraMode
//{
//private:
//
//	/**
//	 * @brief	Wartoœæ œrednia z próbek.
//	 */
//
//	Sample itsDCVal;
//
//	/**
//	 * @brief Flaga mówi¹ca, czy nale¿y dodatkowo odfiltrowaæ lewy i górny fragment sygna³u predykcji.
//	 */
//
//	Bool itsFiltreEdges;
//
//	/**
//	 * @fn	Void DcMode::calcDcVal( );
//	 *
//	 * @brief	Liczy wartoœæ sta³¹ z próbek referencyjnych.
//	 *
//	 */
//
//	Void calcDcVal( );
//
//	/**
//	 * @fn	Sample DcMode::getFiltCorner( ) const;
//	 *
//	 * @brief	Pobiera filtrowan¹ wartoœæ wspó³czynnika naro¿nego.
//	 *
//	 * @return	Wyg³adona wartoœæ wspó³czynnika naro¿nego.
//	 */
//
//	Sample getFiltCorner( ) const;
//
//	/**
//	 * @fn	Sample DcMode::getFiltEdge( const IntraDirection dir, const UInt offset) const;
//	 *
//	 * @brief	Pobranie wyg³adzonej próbki referencyjnej z danego kierunku i pozycji.
//	 *
//	 * @param	dir			Kierunek pobrania próbki.
//	 * @param	offset	Przesuniêcie próbki wzglêdem lewego górnego rogu aktualnego bloku.
//	 *
//	 * @return	Wartoœæ próbki referencyjnej.
//	 */
//
//	Sample getFiltEdge( const IntraDirection dir, const UInt offset) const;
//public:
//
//	/**
//	 * @fn	DcMode::DcMode( );
//	 *
//	 * @brief	Konstruktor domyœlny.
//	 */
//
//	DcMode( );
//
//	/**
//	 * @fn	DcMode::~DcMode( );
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	~DcMode( );
//
//	/**
//	 * @fn	Sample DcMode::**calcPred( );
//	 *
//	 * @brief	Obliczenie sygna³u predykcji.
//	 *
//	 * @return	Macierz z sygna³em predykcji.
//	 */
//
//	Sample **calcPred( );
//
//};
//
///**
// * @class	LinearMode
// *
// * @brief	Strategia predykcji liniowej.
// */
//
//class LinearMode : public IntraMode
//{
//private:
//
//	/**
//	* @fn	Sample LinearMode::getFiltEdge( const IntraDirection dir, const UInt offset) const;
//	*
//	* @brief	Pobranie wyg³adzonej próbki referencyjnej z danego kierunku i pozycji.
//	*
//	* @param	dir			Kierunek pobrania próbki.
//	* @param	offset	Przesuniêcie próbki wzglêdem lewego górnego rogu aktualnego bloku.
//	*
//	* @return	Wartoœæ próbki referencyjnej.
//	*/
//
//	Sample getFiltEdge( const IntraDirection dir, const UInt offset );
//public:
//
//	/**
//	 * @fn	LinearMode::LinearMode( );
//	 *
//	 * @brief	Konstruktor domyœ³ny.
//	 */
//
//	LinearMode( );
//
//	/**
//	 * @fn	LinearMode::~LinearMode( );
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	~LinearMode( );
//
//	/**
//	 * @fn	Sample LinearMode::**calcPred( );
//	 *
//	 * @brief	Policzenie sygna³u predykcji.
//	 *
//	 * @return	Macierz z sygna³em predykcji.
//	 */
//
//	Sample **calcPred( );
//
//};
//
///**
// * @class	AngMode
// *
// * @brief	Strategia predykcji ukoœnej.
// */
//
//class AngMode : public IntraMode
//{
//private:
//
//	/**
//	 * @brief	Tabela k¹tów.
//	 */
//
//	const static Int angles[];
//
//	/**
//	 * @brief	Tabela "k¹tów odwróconych".
//	 */
//
//	const static Int invAngles[];
//
//	/**
//	 * @brief	Po³¹czona macerz próbek referencyjncyh.
//	 */
//
//	Sample* refsArray;
//
//	/**
//	 * @fn	Int AngMode::getAngle( ) const;
//	 *
//	 * @brief	Pobranie aktualnego kata predykcji.
//	 *
//	 * @return	K¹t odwróconej predykcji.
//	 */
//
//	Int getAngle( ) const;
//
//	/**
//	 * @fn	Int AngMode::getInvAngle( ) const;
//	 *
//	 * @brief Pobranie aktualnego kata predykcji.
//	 *
//	 * @return	K¹t odwróconej predykcji.
//	 */
//
//	Int getInvAngle( ) const;
//
//	/**
//	 * @fn	Void AngMode::getRefsArray( );
//	 *
//	 * @brief	Gets references array.
//	 *
//	 * @return	The references array.
//	 */
//
//	Void getRefsArray( );
//public:
//
//	/**
//	 * @fn	AngMode::AngMode( );
//	 *
//	 * @brief	Konstruktor domyœlny.
//	 */
//
//	AngMode( );
//
//	/**
//	 * @fn	AngMode::~AngMode( );
//	 *
//	 * @brief	Destruktor.
//	 */
//
//	~AngMode( );
//
//	/**
//	* @fn	Sample AngMode::**calcPred( );
//	*
//	* @brief	Policzenie sygna³u predykcji.
//	*
//	* @return	Macierz z sygna³em predykcji.
//	*/
//
//	Sample **calcPred( );
//
//};
//
//#endif