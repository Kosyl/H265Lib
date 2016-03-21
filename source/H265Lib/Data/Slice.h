#pragma once

namespace HEVC
{
	class Slice
	{
		//private:
		//
		//	/**
		//	 * @brief	Typ plastra (I/P/B).
		//	 */
		//
		//	SliceMode itsSliceType;
		//
		//	/**
		//	 * @property	bool itsSAOLumaEnabled, itsSAOChromaEnabled
		//	 *
		//	 * @brief	Zezwolenie na filtracj� SAO pr�bek luminancji i chrominancji.
		//	 */
		//
		//	bool itsSAOLumaEnabled, itsSAOChromaEnabled;
		//
		//	/**
		//	 * @brief	Zezwolenie na u�ycie filtru deblokuj�cego.
		//	 */
		//
		//	bool itsDeblockingFilterEnabled;
		//
		//	/**
		//	 * @brief	Zezwolenie na u�ycie filtru deblokuj�cego oraz SAO.
		//	 */
		//
		//	bool itsLoopFilterEnabled;
		//
		//	/**
		//	 * @brief	Indeks porz�dkowy.
		//	 */
		//
		//	int itsSliceIdx;
		//
		//	/**
		//	 * @brief	Zmiana QP wzgl�dem warto�ci bazowej z parametr�w obrazu.
		//	 */
		//
		//	Short itsQPDelta;
		//
		//	/**
		//	 * @property	Short _CrQPOffset, _CbQPOffset
		//	 *
		//	 * @brief	Offsety QP dla chrominancji.
		//	 */
		//
		//	Short _CrQPOffset, _CbQPOffset;
		//
		//	/**
		//	 * @brief	W��cza/wy��cza synchronizacj� kodera entropijnego na tym plastrze.
		//	 */
		//
		//	bool itsECSyncEnabled;
		//
		//	/**
		//	 * @brief	Zmiana parametru Beta w filtrze deblokuj�cym.
		//	 */
		//
		//	Short itsBetaOffset;
		//
		//	/**
		//	 * @brief	Zmiana parametru tc w filtrze deblokuj�cym.
		//	 */
		//
		//	Short itsTcOffset;
		//
		//	/**
		//	 * @property	std::vector<std::weak_ptr<CTU> > itsCTUs
		//	 *
		//	 * @brief CTU zawieraj�ce si� w plastrze.
		//	 *
		//	 */
		//
		//	std::vector<std::weak_ptr<CTU> > itsCTUs;
		//
		//	/**
		//	 * @brief	Nadrz�dny obraz plastra.
		//	 */
		//
		//	Picture* itsParentPicture;
		//
		//public:
		//
		//	/**
		//	 * @fn	Slice::Slice( Picture* picture, SliceMode type = SliceMode::SLICE_I );
		//	 *
		//	 * @brief	Konstruktor.
		//	 *
		//	 * @param [in]	Obraz, w kt�rym jest plaster.
		//	 * @param	type						 	Rodzaj plastra.
		//	 */
		//
		//	Slice( Picture* picture, SliceMode type = SliceMode::SLICE_I );
		//
		//	/**
		//	 * @fn	Slice::~Slice( );
		//	 *
		//	 * @brief	Destruktor.
		//	 */
		//
		//	~Slice( );
		//
		//#pragma region Akcesory
		//
		//	/**
		//	 * @fn	Picture* Slice::getParentPicture( ) const
		//	 *
		//	 * @brief	Zwraca obraz w k�rym le�y plaster.
		//	 */
		//
		//	Picture* getParentPicture( ) const
		//	{
		//		return itsParentPicture;
		//	}
		//
		//	/**
		//	 * @fn	SliceMode Slice::getType( ) const
		//	 *
		//	 * @brief	Rodzaj plastra.
		//	 */
		//
		//	SliceMode getType( ) const
		//	{
		//		return itsSliceType;
		//	}
		//
		//	/**
		//	 * @fn	void Slice::setSliceType( SliceMode type )
		//	 *
		//	 * @brief	Ustawia tryb plastra.
		//	 *
		//	 * @param	type	Typ plastra.
		//	 *
		//	 * @return	void.
		//	 */
		//
		//	void setSliceType( SliceMode type )
		//	{
		//		itsSliceType = type;
		//	}
		//
		//	/**
		//	 * @fn	bool Slice::getSAOChromaEnabled( ) const
		//	 *
		//	 * @brief	Zezwolenie na u�ycie SAO dla chrominancji.
		//	 */
		//
		//	bool getSAOChromaEnabled( ) const
		//	{
		//		return itsSAOChromaEnabled;
		//	}
		//
		//	/**
		//	 * @fn	void Slice::setSAOChromaEnabled( bool val )
		//	 *
		//	 * @brief	Ustawia zezwolenie na u�ycie SAO dla chrominancji.
		//	 *
		//	 * @param	val	true zezwala, false blokuje SAO dla chrominancji.
		//	 *
		//	 * @return	void.
		//	 */
		//
		//	void setSAOChromaEnabled( bool val )
		//	{
		//		itsSAOChromaEnabled = val;
		//	}
		//
		//	/**
		//	 * @fn	bool Slice::getSAOLumaEnabled( ) const
		//	 *
		//	 * @brief	Zezwolenie na u�ycie SAO dla luminancji.
		//	 */
		//
		//	bool getSAOLumaEnabled( ) const
		//	{
		//		return itsSAOLumaEnabled;
		//	}
		//
		//	/**
		//	 * @fn	void Slice::setSAOLumaEnabled( bool val )
		//	 *
		//	 * @brief	Ustawia zezwolenie na u�ycie SAO dla luminancji.
		//	 *
		//	 * @param	val	true zezwala, false blokuje SAO dla luminancji.
		//	 *
		//	 * @return	void.
		//	 */
		//
		//	void setSAOLumaEnabled( bool val )
		//	{
		//		itsSAOLumaEnabled = val;
		//	}
		//
		//	/**
		//	 * @fn	bool Slice::getsLoopFilterEnabled( ) const
		//	 *
		//	 * @brief		Zezwolenie na u�ycie FDB i SAO.
		//	 */
		//
		//	bool getsLoopFilterEnabled( ) const
		//	{
		//		return itsLoopFilterEnabled;
		//	}
		//
		//	/**
		//	 * @fn	void Slice::setLoopFilterEnabled( bool val )
		//	 *
		//	 * @brief	Ustawia zezwolenie na u�ycie FDB i SAO.
		//	 *
		//	 * @param	val	true zezwala, false u�ycie FDB i SAO.
		//	 *
		//	 * @return	void.
		//	 */
		//
		//	void setLoopFilterEnabled( bool val )
		//	{
		//		itsLoopFilterEnabled = val;
		//	}
		//
		//	/**
		//	 * @fn	bool Slice::getDeblockingFilterEnabled( ) const
		//	 *
		//	 * @brief	Zezwolenie na u�ycie FDB.
		//	 */
		//
		//	bool getDeblockingFilterEnabled( ) const
		//	{
		//		return itsDeblockingFilterEnabled;
		//	}
		//
		//	/**
		//	 * @fn	void Slice::setDeblockingFilterEnabled( bool val )
		//	 *
		//	 * @brief	Ustawia zezwolenie na u�ycie FDB.
		//	 *
		//	 * @param	val	true zezwala, false u�ycie FDB.
		//	 *
		//	 * @return	void.
		//	 */
		//
		//	void setDeblockingFilterEnabled( bool val )
		//	{
		//		itsDeblockingFilterEnabled = val;
		//	}
		//
		//	/**
		//	 * @fn	Short Slice::getQPDelta( ImgComp comp )
		//	 *
		//	 * @brief	Zwraca offset QP wnoszony przez plaster dla danej sk�adowej obrazu.
		//	 *
		//	 * @param	comp	Sk�adowa obrazu.
		//	 *
		//	 * @return	Offset warto�ci QP.
		//	 */
		//
		//	Short getQPDelta( ImgComp comp )
		//	{
		//		switch( comp )
		//		{
		//		case CB:
		//			return itsQPDelta + _CbQPOffset;
		//		case CR:
		//			return itsQPDelta + _CrQPOffset;
		//		case LUMA:
		//		default:
		//			return itsQPDelta;
		//		}
		//	}
		//
		//	/**
		//	 * @fn	int Slice::getSliceIdx( ) const
		//	 *
		//	 * @brief	Indeks plastra.
		//	 */
		//
		//	int getSliceIdx( ) const
		//	{
		//		return itsSliceIdx;
		//	}
		//
		//	/**
		//	 * @fn	void Slice::setSliceIdx( int val )
		//	 *
		//	 * @brief	Ustawia indeks plastra.
		//	 *
		//	 * @param	val	Nowy indeks.
		//	 */
		//
		//	void setSliceIdx( int val )
		//	{
		//		itsSliceIdx = val;
		//	}
		//
		//	/**
		//	 * @fn	Short Slice::getBetaOffset( ) const
		//	 *
		//	 * @brief	Offset warto�ci Beta w FDB.
		//	 */
		//
		//	Short getBetaOffset( ) const
		//	{
		//		return itsBetaOffset;
		//	}
		//
		//	/**
		//	 * @fn	void Slice::setBetaOffset( Short val )
		//	 *
		//	 * @brief	Ustawia warto�� offsetu Beta w FDB.
		//	 *
		//	 * @param	val	Nowa warto�� offsetu Beta.
		//	 */
		//
		//	void setBetaOffset( Short val )
		//	{
		//		itsBetaOffset = val;
		//	}
		//
		//	/**
		//	 * @fn	Short Slice::getTcOffset( ) const
		//	 *
		//	 * @brief	Offset warto�ci tc w FDB.
		//	 */
		//
		//	Short getTcOffset( ) const
		//	{
		//		return itsTcOffset;
		//	}
		//
		//	/**
		//	 * @fn	void Slice::setTcOffset( Short val )
		//	 *
		//	 * @brief	Ustawia warto�� offsetu tc w FDB.
		//	 *
		//	 * @param	val	Nowa warto�� offsetu tc.
		//	 */
		//
		//	void setTcOffset( Short val )
		//	{
		//		itsTcOffset = val;
		//	}
		//#pragma endregion
		//
		//	/**
		//	 * @fn	void Slice::appendCTU( std::shared_ptr<CTU> ctu );
		//	 *
		//	 * @brief	Dodaje do plastra nowe CTU.
		//	 *
		//	 * @param	ctu	Nowe CTU.
		//	 *
		//	 * @return	void.
		//	 */
		//
		//	void appendCTU( std::shared_ptr<CTU> ctu );
	};
}