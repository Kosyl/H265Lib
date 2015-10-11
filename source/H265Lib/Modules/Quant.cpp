///**
// * @file	Quant.cpp
// *
// * @brief	Implementacja kalkulatora kwantyzacji
// */
//
//#include "Quant.h"
//#include <assert.h>
//#include <algorithm>
//
///** Konstruktor domyslny, wylaczone logowanie
// *
// */
//Quant::Quant()
//{
//}
//
//Quant* Quant::instance = nullptr;
//
//Quant* Quant::getInstance()
//{
//	if (instance == nullptr)
//		instance = new Quant();
//	return instance;
//}
//
///** Konstruktor przyjmuje sciezke do logu i rozkaz wlaczenia/nie wlaczania logu
// *	\param path sciezka do pliku logu
// \param isLogging pisze log, jesli true
// */
//void Quant::initLog(std::string path, bool isLogging)
//{
//	/*log.changeLogPath( path );
//	log.setLogging( isLogging );*/
//}
//
//Quant::~Quant()
//{
//}
//
///** Kwantowanie podanego bloku
// *	\param inputCoeff tablica 2D probek wejsciowych
// *	\param result pusta tablica 2D wspolczynnikow wyjsciowych
// *	\param QP parametr kwantyzacji
// *	\param bitDepth glebia bitowa probek (8 bitow oznacza zakres <-256;255> )
// *	\param totalSize rozmiar blokow wejsciowych/wyjsciowych
// *	\param trSize rozmiar transformaty - tylko na potrzeby logowania, jesli np. transformata 4x4 w bloku 8x8 to funkcja moze zalogowac 4x4
// *	\return void
// */
//void Quant::quant(Coeff** const inputCoeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize)
//{
//	Short log2TrSize;
//	switch (trSize)
//	{
//	case 4:
//		log2TrSize = 2;
//		break;
//	case 8:
//		log2TrSize = 3;
//		break;
//	case 16:
//		log2TrSize = 4;
//		break;
//	case 32:
//		log2TrSize = 5;
//		break;
//	default:
//		assert(0);
//	}
//
//	Int Q = forfardQuantCoeffs[QP % 6];
//	Int offset = 171 << (12 + QP / 6 - log2TrSize - (bitDepth - 8));
//	Int shift = (21 + QP / 6 - log2TrSize - (bitDepth - 8));
//
//	for (int i = 0; i < totalSize; ++i)
//		for (int j = 0; j < totalSize; ++j)
//		{
//			Int tmp;
//			Int sign = inputCoeff[i][j] < 0 ? -1 : 1;
//
//			tmp = static_cast<long long>(inputCoeff[i][j]) *Q + (sign == 1 ? offset : 2 * offset) >> shift;
//			result[i][j] = std::min<Int>(std::max<Int>(-32768, tmp), 32767);
//		}
//}
//
//void Quant::quant(Coeff** const inputCoeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& trSize)
//{
//	quant(inputCoeff, result, QP, bitDepth, trSize, trSize);
//}
//
///** Dekwantyzacja podanego bloku
// *	\param inputCoeff tablica 2D probek wejsciowych
// *	\param result pusta tablica 2D wspolczynnikow wyjsciowych
// *	\param QP parametr kwantyzacji
// *	\param bitDepth glebia bitowa probek (8 bitow oznacza zakres <-256;255> )
// *	\param totalSize rozmiar blokow wejsciowych/wyjsciowych
// *	\param trSize rozmiar transformaty - tylko na potrzeby logowania, jesli np. transformata 4x4 w bloku 8x8 to funkcja moze zalogowac 4x4
// *	\return void
// */
//void Quant::deQuant(Coeff** const inputCoeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize)
//{
//	Short log2TrSize;
//	switch (trSize)
//	{
//	case 4:
//		log2TrSize = 2;
//		break;
//	case 8:
//		log2TrSize = 3;
//		break;
//	case 16:
//		log2TrSize = 4;
//		break;
//	case 32:
//		log2TrSize = 5;
//		break;
//	default:
//		assert(0);
//	}
//	Int IQ = inverseQuantCoeffs[QP % 6];
//	Int offset = 1 << (log2TrSize - 2 + (bitDepth - 8));
//	Int shift = log2TrSize - 1 + (bitDepth - 8);
//
//	for (auto i = 0; i < totalSize; ++i)
//	{
//		for (auto j = 0; j < totalSize; ++j)
//		{
//			result[i][j] = std::min<Int>(std::max<Int>(-32768, inputCoeff[i][j]), 32767);
//
//			result[i][j] = (result[i][j] * IQ << QP / 6) + offset >> shift;
//			result[i][j] = std::min<Int>(std::max<Int>(-32768, result[i][j]), 32767);
//		}
//	}
//}
//
//void Quant::deQuant(Coeff** const inputCoeff, Coeff** result, const Int& QP, const Int& bitDepth, const Int& trSize)
//{
//	deQuant(inputCoeff, result, QP, bitDepth, trSize, trSize);
//}
//
//void Quant::test(Coeff input, Int QP, Int bitDepth, int trSize)
//{
//	Short log2TrSize;
//	switch (trSize)
//	{
//	case 4:
//		log2TrSize = 2;
//		break;
//	case 8:
//		log2TrSize = 3;
//		break;
//	case 16:
//		log2TrSize = 4;
//		break;
//	case 32:
//		log2TrSize = 5;
//		break;
//	default:
//		assert(0);
//	}
//
//	Int Q = forfardQuantCoeffs[QP % 6];
//	Int offset = 171 << (12 + QP / 6 - log2TrSize - (bitDepth - 8));
//	Int shift = (21 + QP / 6 - log2TrSize - (bitDepth - 8));
//
//	Int tmp;
//	Int sign = input < 0 ? -1 : 1;
//
//	LOGLN(Logs::Quant, PRINTVAR(input));
//	tmp = ((((long long)(input)) *Q + (sign == 1 ? offset : 2 * offset)) >> shift);
//	Int result = std::min<Int>(std::max<Int>(-32768, tmp), 32767);
//
//	LOGLN(Logs::Quant, PRINTVAR(result));
//	Int IQ = inverseQuantCoeffs[QP % 6];
//	offset = 1 << (log2TrSize - 2 + (bitDepth - 8));
//	shift = log2TrSize - 1 + (bitDepth - 8);
//
//	result = ((((result * IQ) << QP / 6) + offset) >> shift);
//	result = std::min<Int>(std::max<Int>(-32768, result), 32767);
//	LOGLN(Logs::Quant, PRINTVAR(result));
//}
//
//void Quant::test2(Coeff input, Int QP, Int bitDepth, int trSize)
//{
//	Short log2TrSize;
//	switch (trSize)
//	{
//	case 4:
//		log2TrSize = 2;
//		break;
//	case 8:
//		log2TrSize = 3;
//		break;
//	case 16:
//		log2TrSize = 4;
//		break;
//	case 32:
//		log2TrSize = 5;
//		break;
//	default:
//		assert(0);
//	}
//
//	Int MF = forfardQuantCoeffs[QP % 6];
//	Int SD = (21 - log2TrSize - (bitDepth - 8));
//	Int f = (5 * (1 << (SD - 1 + QP / 6))) / 6;
//
//	LOGLN(Logs::Quant, "+++++++++++++++++++");
//	//LOGLN(Logs::Quant, PRINTVAR(QP));
//	//LOGLN(Logs::Quant, PRINTVAR(MF));
//	//LOGLN(Logs::Quant, PRINTVAR(f));
//	LOGLN(Logs::Quant, PRINTVAR(input));
//
//	long long quanted = (((long long)(input)) *MF + f) >> (SD + QP / 6);
//	LOGLN(Logs::Quant, PRINTVAR(quanted));
//
//	MF = inverseQuantCoeffs[QP % 6];
//	SD = log2TrSize + 3;
//	Int deq = (((quanted * 16 * MF) << (QP / 6)) + (1 << (SD - 1))) >> SD;
//	LOGLN(Logs::Quant, PRINTVAR(deq));
//}
//
//const Short Quant::forfardQuantCoeffs[6] = { 26214, 23302, 20560, 18396, 16384, 14564 };
//const Short Quant::inverseQuantCoeffs[6] = { 40, 45, 51, 57, 64, 72 };