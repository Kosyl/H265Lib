#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <cassert>
#include <algorithm>

namespace HEVC
{
	template<typename T>
	class Matrix
	{
	private:
		struct MatrixInfo
		{
			size_t Rows = 0, Columns = 0, Stride = 0, OffsetX = 0, OffsetY = 0;
		};

		std::shared_ptr<std::vector<T>> data;
		MatrixInfo matrix_info;

		void resize(size_t columns, size_t rows, bool initData)
		{
			matrix_info.Rows = rows;
			matrix_info.Columns = columns;
			matrix_info.Stride = columns;

			if (initData)
				initWithDefaults();
		}

	public:
		Matrix() :
			data(nullptr)
		{
			resize(0, 0, false);
		}

		Matrix(size_t columns, size_t rows) :
			data(std::make_shared<std::vector<T>>())
		{
			resize(columns, rows, true);
		}

		Matrix(std::shared_ptr<std::vector<T>> extData, size_t columns, size_t rows) :
			data(extData)
		{
			resize(columns, rows, false);
		}
		
		Matrix(const Matrix& other)
			: data(other.data),
			matrix_info(other.matrix_info)
		{
		}

		Matrix(Matrix&& other)
			: data(std::move(other.data)),
			matrix_info(std::move(other.matrix_info))
		{
		}

		Matrix& operator=(const Matrix& other)
		{
			if (this == &other)
				return *this;
			data = other.data;
			matrix_info = other.matrix_info;
			return *this;
		}

		Matrix& operator=(Matrix&& other)
		{
			if (this == &other)
				return *this;
			data = std::move(other.data);
			matrix_info = std::move(other.matrix_info);
			return *this;
		}

		Matrix submatrix(size_t offsetX, size_t offsetY, size_t width, size_t height)
		{
			Matrix result(*this);
			result.matrix_info.Rows = height;
			result.matrix_info.Columns = width;
			result.matrix_info.OffsetX = offsetX;
			result.matrix_info.OffsetY = offsetY;
			result.matrix_info.Stride = matrix_info.Stride;

			return result;
		}

		Matrix deepCopy()
		{
			Matrix result;
			result.data = std::make_shared<std::vector<T>>(height()*width());
			result.matrix_info.Rows = height();
			result.matrix_info.Columns = width();
			result.matrix_info.OffsetX = 0;
			result.matrix_info.OffsetY = 0;
			result.matrix_info.Stride = width();

			for(auto& src: *this)
			{
				for(auto& dst: result)
				{
					dst = src;
				}
			}

			return result;
		}

		size_t width()
		{
			return matrix_info.Columns;
		}

		size_t height()
		{
			return matrix_info.Rows;
		}

		typename std::vector<T>::const_reference at(size_t column, size_t row) const
		{
			return (*data)[(matrix_info.OffsetY + row)*matrix_info.Stride + column + matrix_info.OffsetX];
		}

		typename std::vector<T>::reference at(size_t column, size_t row)
		{
			return (*data)[(matrix_info.OffsetY + row)*matrix_info.Stride + column + matrix_info.OffsetX];
		}

		typename std::vector<T>::const_reference operator()(size_t column, size_t row) const
		{
			return (*data)[(matrix_info.OffsetY + row)*matrix_info.Stride + column + matrix_info.OffsetX];
		}

		typename std::vector<T>::reference operator()(size_t column, size_t row)
		{
			return (*data)[(matrix_info.OffsetY + row)*matrix_info.Stride + column + matrix_info.OffsetX];
		}

		void initWithDefaults()
		{
			auto total = width()*height();
			(*data).resize(total);
			(*data).clear();
			for (size_t i = 0; i < total; ++i)
			{
				(*data).push_back(T());
			}
		}

#pragma region Iterator
		template
			<
				class Type,
				class UnqualifiedType = std::remove_cv_t<Type>
			>
		class ForwardIterator
			: public std::iterator < std::forward_iterator_tag,
			UnqualifiedType,
			std::ptrdiff_t,
			Type*,
			Type& >
		{
			Matrix& _sourceMatrix;
			size_t _row, _column, _idx;

		public:

			explicit ForwardIterator(Matrix& matrix, size_t row, size_t column)
				: _sourceMatrix(matrix), _row(row), _column(column)
			{
				refreshIdx();
			}

			void swap(ForwardIterator& other)
			{
				using std::swap;
				swap(_sourceMatrix, other._sourceMatrix);
			}

			void refreshIdx()
			{
				_idx = (_sourceMatrix.matrix_info.OffsetY + _row)*_sourceMatrix.matrix_info.Stride + _column + _sourceMatrix.matrix_info.OffsetX;
			}

			void incrementIdx()
			{
				++_column;
				if (_column == _sourceMatrix.matrix_info.Columns)
				{
					_column = 0;
					++_row;
				}
				refreshIdx();
			}

			ForwardIterator& operator++ ()
			{
				incrementIdx();
				return *this;
			}

			ForwardIterator operator++ (int)
			{
				ForwardIterator tmp(*this);
				incrementIdx();
				return tmp;
			}

			template<class OtherType>
			bool operator == (const ForwardIterator<OtherType>& rhs) const
			{
				return &_sourceMatrix == &(rhs._sourceMatrix) && _idx == rhs._idx;
			}

			template<class OtherType>
			bool operator != (const ForwardIterator<OtherType>& rhs) const
			{
				return &_sourceMatrix != &(rhs._sourceMatrix) || _idx != rhs._idx;
			}

			typename std::vector<UnqualifiedType>::reference operator* () const
			{
				return (*(_sourceMatrix.data))[_idx];
			}

			typename std::vector<UnqualifiedType>::reference operator-> () const
			{
				return (*(_sourceMatrix.data))[_idx];
			}

			operator ForwardIterator<const Type>() const
			{
				return ForwardIterator<const Type>(_sourceMatrix);
			}
		};

		friend class ForwardIterator < T >;
		friend class ForwardIterator < const T >;

		typedef ForwardIterator<T> iterator;
		typedef ForwardIterator<const T> const_iterator;

		iterator begin()
		{
			return iterator(*this, 0, 0);
		}

		iterator end()
		{
			return iterator(*this, matrix_info.Rows, 0);
		}

		const_iterator cbegin()
		{
			return const_iterator(*this, 0, 0);
		}

		const_iterator cend()
		{
			return const_iterator(*this, matrix_info.Rows, 0);
		}

#pragma endregion
	};
};