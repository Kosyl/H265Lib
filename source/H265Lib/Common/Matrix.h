#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <cassert>
#include <algorithm>

namespace H265Lib
{
	template<typename T>
	class Matrix
	{
	private:
		struct MatrixInfo
		{
			size_t Rows = 0, Columns = 0, Stride = 0, OffsetX = 0, OffsetY = 0;
		};

		std::shared_ptr<std::vector<T>> _data;
		MatrixInfo _matrixInfo;

	public:
		Matrix() :
			_data(std::make_shared<std::vector<T>>()),
			_matrixInfo()
		{
		}

		Matrix(size_t columns, size_t rows) :
			_data(std::make_shared<std::vector<T>>()),
			_matrixInfo()
		{
			resize(columns, rows);
		}

		Matrix(const Matrix& other)
			: _data(std::make_shared<std::vector<T>>(*(other._data))),
			_matrixInfo(other._matrixInfo)
		{
		}

		Matrix(Matrix&& other)
			: _data(std::move(other._data)),
			_matrixInfo(std::move(other._matrixInfo))
		{
		}

		Matrix& operator=(const Matrix& other)
		{
			if (this == &other)
				return *this;
			_data = std::make_shared<std::vector<T>>(*(other._data));
			_matrixInfo = other._matrixInfo;
			return *this;
		}

		Matrix& operator=(Matrix&& other)
		{
			if (this == &other)
				return *this;
			_data = std::move(other._data);
			_matrixInfo = std::move(other._matrixInfo);
			return *this;
		}

		std::shared_ptr<Matrix> submatrix(size_t offsetX, size_t offsetY, size_t width, size_t height)
		{
			auto result = std::make_shared<Matrix>(*this);
			result->_matrixInfo.Rows = height;
			result->_matrixInfo.Columns = width;
			result->_matrixInfo.OffsetX = offsetX;
			result->_matrixInfo.OffsetY = offsetY;
			result->_matrixInfo.Stride = _matrixInfo.Stride;

			return result;
		}

		void resize(size_t columns, size_t rows)
		{
			_matrixInfo.Rows = rows;
			_matrixInfo.Columns = columns;

			initWithDefaults();

			_matrixInfo.Stride = columns;
		}

		size_t width()
		{
			return _matrixInfo.Columns;
		}

		size_t height()
		{
			return _matrixInfo.Rows;
		}

		typename std::vector<T>::const_reference at(size_t column, size_t row) const
		{
			return (*_data)[(_matrixInfo.OffsetY + row)*_matrixInfo.Stride + column + _matrixInfo.OffsetX];
		}

		typename std::vector<T>::reference at(size_t column, size_t row)
		{
			return (*_data)[(_matrixInfo.OffsetY + row)*_matrixInfo.Stride + column + _matrixInfo.OffsetX];
		}

		typename std::vector<T>::const_reference operator()(size_t column, size_t row) const
		{
			return (*_data)[(_matrixInfo.OffsetY + row)*_matrixInfo.Stride + column + _matrixInfo.OffsetX];
		}

		typename std::vector<T>::reference operator()(size_t column, size_t row)
		{
			return (*_data)[(_matrixInfo.OffsetY + row)*_matrixInfo.Stride + column + _matrixInfo.OffsetX];
		}

		void initWithDefaults()
		{
			auto total = width()*height();
			(*_data).resize(total);
			(*_data).clear();
			for (size_t i = 0; i < total; ++i)
			{
				(*_data).push_back(T());
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
				_idx = (_sourceMatrix._matrixInfo.OffsetY + _row)*_sourceMatrix._matrixInfo.Stride + _column + _sourceMatrix._matrixInfo.OffsetX;
			}

			void incrementIdx()
			{
				++_column;
				if (_column == _sourceMatrix._matrixInfo.Columns)
				{
					_column = 0;
					++_row;
				}
				refreshIdx();
			}

			ForwardIterator& operator++ () // Pre-increment
			{
				incrementIdx();
				return *this;
			}

			ForwardIterator operator++ (int) // Post-increment
			{
				ForwardIterator tmp(*this);
				incrementIdx();
				return tmp;
			}

			// two-way comparison: v.begin() == v.cbegin() and vice versa
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
				return (*(_sourceMatrix._data))[_idx];
			}

			typename std::vector<UnqualifiedType>::reference operator-> () const
			{
				return (*(_sourceMatrix._data))[_idx];
			}

			// One way conversion: iterator -> const_iterator
			operator ForwardIterator<const Type>() const
			{
				return ForwardIterator<const Type>(_sourceMatrix);
			}
		};

		friend class ForwardIterator < T > ;
		friend class ForwardIterator < const T > ;
		// `iterator` and `const_iterator` used by your class:
		typedef ForwardIterator<T> iterator;
		typedef ForwardIterator<const T> const_iterator;

		iterator begin()
		{
			return iterator(*this, 0, 0);
		}

		iterator end()
		{
			return iterator(*this, _matrixInfo.Rows, 0);
		}

		const_iterator cbegin()
		{
			return const_iterator(*this, 0, 0);
		}

		const_iterator cend()
		{
			return const_iterator(*this, _matrixInfo.Rows, 0);
		}

#pragma endregion
	};
};