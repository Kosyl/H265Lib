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
	class Matrix;

	template<typename T>
	using MatrixRef = Matrix<T>;

	template<typename T>
	class Matrix
	{
	private:
		struct MatrixInfo
		{
			size_t rows = 0, columns = 0, stride = 0, offsetX = 0, offsetY = 0;
		};

		std::shared_ptr<std::vector<T>> m_data;
		MatrixInfo matrix_info;

		void initWithDefaults()
		{
			auto total = width()*height();
			(*m_data).resize(total);
			(*m_data).clear();
			for (size_t i = 0; i < total; ++i)
			{
				(*m_data).push_back(T());
			}
		}

		void resize(size_t columns, size_t rows, bool initData)
		{
			matrix_info.rows = rows;
			matrix_info.columns = columns;
			matrix_info.stride = columns;

			if (initData)
				initWithDefaults();
		}

	public:
		Matrix() :
			m_data(nullptr)
		{
			resize(0, 0, false);
		}

		Matrix( size_t size):
			Matrix(size,size )
		{
		}

		Matrix(size_t columns, size_t rows) :
			m_data(std::make_shared<std::vector<T>>())
		{
			resize(columns, rows, true);
		}

		Matrix(std::shared_ptr<std::vector<T>> extData, size_t columns, size_t rows) :
			m_data(extData)
		{
			resize(columns, rows, false);
		}
		
		Matrix(const Matrix& other)
			: m_data(other.m_data),
			matrix_info(other.matrix_info)
		{
		}

		Matrix(Matrix&& other)
			: m_data(std::move(other.m_data)),
			matrix_info(std::move(other.matrix_info))
		{
		}

		Matrix& operator=(const Matrix& other)
		{
			if (this == &other)
				return *this;
			m_data = other.m_data;
			matrix_info = other.matrix_info;
			return *this;
		}

		Matrix& operator=(Matrix&& other)
		{
			if (this == &other)
				return *this;
			m_data = std::move(other.m_data);
			matrix_info = std::move(other.matrix_info);
			return *this;
		}

		MatrixRef<T> submatrix(size_t offsetX, size_t offsetY, size_t width, size_t height)
		{
			MatrixRef<T> result(*this);
			result.matrix_info.rows = height;
			result.matrix_info.columns = width;
			result.matrix_info.offsetX = offsetX;
			result.matrix_info.offsetY = offsetY;
			result.matrix_info.stride = matrix_info.stride;

			return result;
		}

		MatrixRef<T> deepCopy()
		{
			MatrixRef<T> result;
			result.m_data = std::make_shared<std::vector<T>>(height()*width());
			result.matrix_info.rows = height();
			result.matrix_info.columns = width();
			result.matrix_info.offsetX = 0;
			result.matrix_info.offsetY = 0;
			result.matrix_info.stride = width();

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
			return matrix_info.columns;
		}

		size_t height()
		{
			return matrix_info.rows;
		}

		typename std::vector<T>::const_reference at(size_t column, size_t row) const
		{
			return (*m_data)[(matrix_info.offsetY + row)*matrix_info.stride + column + matrix_info.offsetX];
		}

		typename std::vector<T>::reference at(size_t column, size_t row)
		{
			return (*m_data)[(matrix_info.offsetY + row)*matrix_info.stride + column + matrix_info.offsetX];
		}

		typename std::vector<T>::const_reference operator()(size_t column, size_t row) const
		{
			return (*m_data)[(matrix_info.offsetY + row)*matrix_info.stride + column + matrix_info.offsetX];
		}

		typename std::vector<T>::reference operator()(size_t column, size_t row)
		{
			return (*m_data)[(matrix_info.offsetY + row)*matrix_info.stride + column + matrix_info.offsetX];
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
				_idx = (_sourceMatrix.matrix_info.offsetY + _row)*_sourceMatrix.matrix_info.stride + _column + _sourceMatrix.matrix_info.offsetX;
			}

			void incrementIdx()
			{
				++_column;
				if (_column == _sourceMatrix.matrix_info.columns)
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
				return (*(_sourceMatrix.m_data))[_idx];
			}

			typename std::vector<UnqualifiedType>::reference operator-> () const
			{
				return (*(_sourceMatrix.m_data))[_idx];
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
			return iterator(*this, matrix_info.rows, 0);
		}

		const_iterator cbegin()
		{
			return const_iterator(*this, 0, 0);
		}

		const_iterator cend()
		{
			return const_iterator(*this, matrix_info.rows, 0);
		}

#pragma endregion
	};

	template<typename T>
	struct Matrix3D
	{
		Matrix<T> values[3];

		Matrix<T>& operator[](int i) { return values[i]; }
	};

	template<typename T>
	using Matrix3DRef = Matrix3D<T>;
}