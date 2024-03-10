#ifndef GRID_9_58_AM_21_10_2023_H
#define GRID_9_58_AM_21_10_2023_H
#include <cassert>
#include <array>
#include <vector>
//bitset manipulation
//https://www.learncpp.com/cpp-tutorial/bit-flags-and-bit-manipulation-via-stdbitset/
//partial template specialisation
//https://www.learncpp.com/cpp-tutorial/partial-template-specialization/
template <typename T, unsigned int row_count, unsigned int column_count >
class Grid
{
	std::array<T, row_count* column_count> m_array{};
public:
	Grid(){}
	Grid(const std::array<T, row_count* column_count>& array):m_array{array}{}
	Grid(std::initializer_list<T> values)
	{
		assert(values.size()==m_array.size());
		unsigned int i{};
		for (T value : values)
		{
			m_array.at(i) = value;
			++i;
		}
			
	}
	virtual void setData(const std::array<T, row_count* column_count>& array)
	{
		m_array = array;
	}
	virtual void setData(std::initializer_list<T> values)
	{
		*this = Grid<T, row_count, column_count>{values};
	}
	const std::array<T, row_count* column_count>& getData() const{return m_array;}
	std::array<T, row_count* column_count>& getData(){return m_array;}
	virtual const T& at(unsigned int r, unsigned int c) const
	{
		assert(r >= 1 && r <= row_count && c >= 1 && c <= column_count);
		return m_array.at((r - 1) * column_count + c - 1);
	}
	virtual const T& const_at(unsigned int r, unsigned int c) const
	{
		assert(r >= 1 && r <= row_count && c >= 1 && c <= column_count);
		return m_array.at((r - 1) * column_count + c - 1);
	}
	virtual T& at(unsigned int r, unsigned int c)
	{
		assert(r >= 1 && r <= row_count && c >= 1 && c <= column_count);
		return m_array.at((r - 1) * column_count + c - 1);
	}
	virtual T& nonconst_at(unsigned int r, unsigned int c)
	{
		assert(r >= 1 && r <= row_count && c >= 1 && c <= column_count);
		return m_array.at((r - 1) * column_count + c - 1);
	}
};
#endif // !GRID_9_58_AM_21_10_2023_H
