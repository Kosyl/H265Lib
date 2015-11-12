#include "Matrix.h"

namespace H265Lib
{

	template <typename T>
	void Matrix<T>::

	template <typename T>
	std::unique_ptr<Matrix<T>> Matrix<T>::createEmpty(size_t columns, size_t rows)
	{
		auto result = std::make_unique<Matrix<T>>(rows, columns);
		result->clear();
		return result;
	}
}