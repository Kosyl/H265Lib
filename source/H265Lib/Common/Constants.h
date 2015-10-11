#pragma once
#include <Common\TypeDef.h>

namespace H265Lib
{
	namespace Constants
	{
		/**
		* @brief Rozmiar grupy CG.
		*/

		static const Int CG_SIZE = 4;

		/**
		* @brief	Ilość współczyników w grupie CG.
		*/

		static const Int NUM_COEFFS_IN_CG = 16;

		/**
		* @brief Próg zmiany rodzaju kodowania reszty współczynnika na kodowanie Golomba
		*/

		static const Int COEFF_REMAIN_THRESHOLD = 3;

		/**
		* @brief	Próg odległości między współczynnikami pozwalający na użycie trybu SDH.
		*/

		static const Int SDH_THRESHOLD = 4;
	};
}
