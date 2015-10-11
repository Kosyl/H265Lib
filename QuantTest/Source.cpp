#include <iostream>
#include <algorithm>
#include <assert.h>
//
//typedef       void                Void;
//typedef       bool                Bool;
//typedef       char                Char;
//typedef       unsigned char       UChar;
//typedef       short               Short;
//typedef       unsigned short      UShort;
//typedef       int                 Int;
//typedef		  long				  Long;
//typedef       unsigned int        UInt;
//typedef       double              Double;
//typedef       float               Float;
//typedef       UChar           Pixel;
//typedef       Short           Sample;
//typedef       Int             Coeff;
//
//const Short forfardQuantCoeffs[6] = { 26214, 23302, 20560, 18396, 16384, 14564 };
//const Short inverseQuantCoeffs[6] = { 40, 45, 51, 57, 64, 72 };
//
//void quant(Coeff inputCoeff[][4], Coeff result[][4], const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize)
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
//	//Int offset = 1 << (20 + QP/6 - std::cout2TrSize - (bitDepth-8) ); //offset, ALE w modelu jest wzor offset = 171 << (12+QP/6-std::cout2TrSize-(bitDepth-8)); daje podobne wyniki
//	Int offset = 171 << (12 + QP / 6 - log2TrSize - (bitDepth - 8));
//	Int shift = (21 + QP / 6 - log2TrSize - (bitDepth - 8));
//
//	for (int i = 0; i < totalSize; ++i)
//		for (int j = 0; j < totalSize; ++j)
//		{
//			Int tmp;
//			Int sign = inputCoeff[i][j] < 0 ? -1 : 1;
//			tmp = ((((long long)(inputCoeff[i][j])) *Q + (sign == 1 ? offset : 2 * offset)) >> shift);
//			result[i][j] = std::min<Int>(std::max<Int>(-32768, tmp), 32767);
//		}
//
//	std::cout << "*** coefficients " << trSize << "x" << trSize << " QP=" << QP << " ***\n";
//		std::cout << "*** bitDepth: " << bitDepth;
//
//		std::cout << "*** QP: " << QP << ", Q: " << Q << ", offset: " << offset << ", shift: " << shift << std::endl;
//	for (Int i = 0; i < totalSize; ++i)
//	{
//		for (Int j = 0; j < totalSize; ++j)
//		{
//			std::cout << inputCoeff[i][j] << ((j == totalSize - 1) ? "" : " ");
//		}
//		std::cout << std::endl;
//	}
//	std::cout << "*** quantization " << trSize << "x" << trSize << " QP=" << QP << " ***\n";
//	for (int i = 0; i < totalSize; ++i)
//	{
//		for (int j = 0; j < totalSize; ++j)
//		{
//			std::cout << result[i][j] << ((j == totalSize - 1) ? "" : " ");
//		}
//		std::cout << std::endl;
//	}
//}
//
//void deQuant(Coeff const inputCoeff[][4], Coeff result[][4], const Int& QP, const Int& bitDepth, const Int& totalSize, const Int& trSize)
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
//	for (int i = 0; i < totalSize; ++i)
//	{
//		for (int j = 0; j < totalSize; ++j)
//		{
//			result[i][j] = std::min<Int>(std::max<Int>(-32768, inputCoeff[i][j]), 32767);
//
//			result[i][j] = ((((result[i][j] * IQ) << QP / 6) + offset) >> shift);
//			result[i][j] = std::min<Int>(std::max<Int>(-32768, result[i][j]), 32767);
//		}
//	}
//	std::cout << "*** dequantization " << trSize << "x" << trSize << " QP=" << QP << " ***\n";
//		std::cout << "+++ IQ: " << IQ << ", offset: " << offset << ", shift: " << shift << " ***\n";
//
//	for (int i = 0; i < totalSize; ++i)
//	{
//		for (int j = 0; j < totalSize; ++j)
//		{
//			std::cout << result[i][j] << ((j == totalSize - 1) ? "" : " ");
//		}
//		std::cout << std::endl;
//	}
//}

int main()
{
	/*Coeff c[4][4] =
	{
		{ 135, -183, -112, -80 },
		{-1781, -203, -12, 69},
		{175, 81, -22, -45},
		{-1300, -185, -40, -8}
	};

	Coeff res[4][4];
	int qp = 40;
	quant(c, res, qp, 8, 4, 4);
	deQuant(res, c, qp, 8, 4, 4);

	system("PAUSE");
*/
	return 0;
}