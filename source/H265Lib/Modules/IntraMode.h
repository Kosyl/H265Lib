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
//	 * @brief	Pr�bka "naro�na"
//	 */
//
//	Sample itsCornerReference;
//
//	/**
//	 * @brief	Dwuwymiarowa macierz pr�bek referencyjncyh.
//	 */
//
//	Sample** itsReferenceValues;
//
//	/**
//	 * @fn	Sample** IntraMode::initPred( ) const;
//	 *
//	 * @brief	Inicjalizacja macierzy predykcji.
//	 *
//	 * @return	Wska�nik na macierz predykcji.
//	 */
//
//	Sample** initPred( ) const;
//public:
//
//	/**
//	 * @fn	IntraMode::IntraMode( );
//	 *
//	 * @brief	Konstruktor domy�lny.
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
//	 * @brief	Metoda licz�ca predykcj�.
//	 *
//	 * @return	Wska�nik na macierz wynikow� z sygna�em predykcji.
//	 */
//
//	virtual Sample** calcPred( ) = 0;
//
//	/**
//	 * @fn	virtual Void IntraMode::setPb( PBIntra* PB );
//	 *
//	 * @brief	Ustawia aktualny blok predykcji.
//	 *
//	 * @param [in]	Wska�nik na blok predykcji.
//	 *
//	 * @return	Void.
//	 */
//
//	virtual Void setPb( PBIntra* PB );
//
//	/**
//	 * @fn	virtual Void IntraMode::setCorner( const Sample corner );
//	 *
//	 * @brief Ustawia wartos� naroznej pr�bki referencyjnej.
//	 *
//	 * @param	corner Warto�� pr�bki.
//	 *
//	 * @return	Void.
//	 */
//
//	virtual Void setCorner( const Sample corner );
//
//	/**
//	 * @fn	virtual Void IntraMode::setSideRefs( const IntraDirection dir, Sample* ref );
//	 *
//	 * @brief	Ustawia podan� tablic� pr�bek jako referencyjne dla danego kierunku.
//	 *
//	 * @param	dir					 	Wskazanie, czy s� to pr�bki z lewej czy z g�ry.
//	 * @param [in] ref	Wska�nik na tablic� pr�bek referencyjnych.
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
//	 * @brief Konstruktor domy�lny.
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
//	 * @return	Macierz na sygna� predykcji.
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
//	 * @brief	Warto�� �rednia z pr�bek.
//	 */
//
//	Sample itsDCVal;
//
//	/**
//	 * @brief Flaga m�wi�ca, czy nale�y dodatkowo odfiltrowa� lewy i g�rny fragment sygna�u predykcji.
//	 */
//
//	Bool itsFiltreEdges;
//
//	/**
//	 * @fn	Void DcMode::calcDcVal( );
//	 *
//	 * @brief	Liczy warto�� sta�� z pr�bek referencyjnych.
//	 *
//	 */
//
//	Void calcDcVal( );
//
//	/**
//	 * @fn	Sample DcMode::getFiltCorner( ) const;
//	 *
//	 * @brief	Pobiera filtrowan� warto�� wsp�czynnika naro�nego.
//	 *
//	 * @return	Wyg�adona warto�� wsp�czynnika naro�nego.
//	 */
//
//	Sample getFiltCorner( ) const;
//
//	/**
//	 * @fn	Sample DcMode::getFiltEdge( const IntraDirection dir, const UInt offset) const;
//	 *
//	 * @brief	Pobranie wyg�adzonej pr�bki referencyjnej z danego kierunku i pozycji.
//	 *
//	 * @param	dir			Kierunek pobrania pr�bki.
//	 * @param	offset	Przesuni�cie pr�bki wzgl�dem lewego g�rnego rogu aktualnego bloku.
//	 *
//	 * @return	Warto�� pr�bki referencyjnej.
//	 */
//
//	Sample getFiltEdge( const IntraDirection dir, const UInt offset) const;
//public:
//
//	/**
//	 * @fn	DcMode::DcMode( );
//	 *
//	 * @brief	Konstruktor domy�lny.
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
//	 * @brief	Obliczenie sygna�u predykcji.
//	 *
//	 * @return	Macierz z sygna�em predykcji.
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
//	* @brief	Pobranie wyg�adzonej pr�bki referencyjnej z danego kierunku i pozycji.
//	*
//	* @param	dir			Kierunek pobrania pr�bki.
//	* @param	offset	Przesuni�cie pr�bki wzgl�dem lewego g�rnego rogu aktualnego bloku.
//	*
//	* @return	Warto�� pr�bki referencyjnej.
//	*/
//
//	Sample getFiltEdge( const IntraDirection dir, const UInt offset );
//public:
//
//	/**
//	 * @fn	LinearMode::LinearMode( );
//	 *
//	 * @brief	Konstruktor domy��ny.
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
//	 * @brief	Policzenie sygna�u predykcji.
//	 *
//	 * @return	Macierz z sygna�em predykcji.
//	 */
//
//	Sample **calcPred( );
//
//};
//
///**
// * @class	AngMode
// *
// * @brief	Strategia predykcji uko�nej.
// */
//
//class AngMode : public IntraMode
//{
//private:
//
//	/**
//	 * @brief	Tabela k�t�w.
//	 */
//
//	const static Int angles[];
//
//	/**
//	 * @brief	Tabela "k�t�w odwr�conych".
//	 */
//
//	const static Int invAngles[];
//
//	/**
//	 * @brief	Po��czona macerz pr�bek referencyjncyh.
//	 */
//
//	Sample* refsArray;
//
//	/**
//	 * @fn	Int AngMode::getAngle( ) const;
//	 *
//	 * @brief	Pobranie aktualnego kata predykcji.
//	 *
//	 * @return	K�t odwr�conej predykcji.
//	 */
//
//	Int getAngle( ) const;
//
//	/**
//	 * @fn	Int AngMode::getInvAngle( ) const;
//	 *
//	 * @brief Pobranie aktualnego kata predykcji.
//	 *
//	 * @return	K�t odwr�conej predykcji.
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
//	 * @brief	Konstruktor domy�lny.
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
//	* @brief	Policzenie sygna�u predykcji.
//	*
//	* @return	Macierz z sygna�em predykcji.
//	*/
//
//	Sample **calcPred( );
//
//};
//
//#endif