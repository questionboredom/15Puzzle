#ifndef PUZZLE_1_43_PM_28_12_2023_H
#define PUZZLE_1_43_PM_28_12_2023_H
#include "Grid.h"
#include "Random.h"
#include <vector>
#include <algorithm>
template <unsigned int row_count, unsigned int column_count>
class Puzzle
{
	Grid<unsigned int, row_count, column_count > m_values{getSolvedStateForGrid()};
	std::array<unsigned int,2> m_zeroPosition{row_count,column_count};
	Grid<unsigned int, row_count, column_count> getSolvedStateForGrid()
	{
		Grid<unsigned int, row_count, column_count> grid{};
		unsigned int i{ 1 };
		for (int r{ 1 }; r <= row_count; ++r)
		{
			for (int c{ 1 }; c <= column_count; ++c)
			{
				grid.at(r, c) = i;
				++i;
				i %= (row_count * column_count);
			}
		}
		return grid;
	}
public:
	Puzzle() = default;
	Puzzle(const Puzzle<row_count, column_count>& puzzle) { *this = puzzle; }
	Puzzle(unsigned int numberOfRandomMoves)
	{
		*this = generateRandomly(numberOfRandomMoves);
	}
	const std::array<unsigned int, 2>& getZeroPosition()const { return m_zeroPosition; }
	//TO DO: generate solvable problems only
	// https://www.geeksforgeeks.org/check-instance-15-puzzle-solvable/
	static constexpr Puzzle<row_count, column_count> generateRandomly(unsigned int randomMovesCount)
	{
		Puzzle<row_count, column_count> puzzle;
		for (unsigned int moveNumber{ 0 }; moveNumber < randomMovesCount; ++moveNumber)
		{
			std::vector<unsigned int> possibleMoves{puzzle.getPossibleMoves()};
			puzzle.move(possibleMoves.at(Random::get(0, possibleMoves.size() - 1)));
		}
		return puzzle;
	}
	unsigned int at(unsigned int r, unsigned int c)const { return m_values.at(r, c); }
	static constexpr bool isSolved(const Grid<unsigned int, row_count, column_count>& puzzleValues)
	{
		unsigned int i{ 1 };
		for (int r{ 1 }; r <= row_count; ++r)
		{
			for (int c{ 1 }; c <= column_count; ++c)
			{
				if (puzzleValues.at(r, c) != i) return false;
				++i;
				i %= (row_count*column_count);
			}
		}
		return true;
	}
	bool isSolved() const{ return isSolved(m_values);}
	std::vector<unsigned int> getPossibleMoves() const
	{
		std::vector<unsigned int> possibleMoves{};
		possibleMoves.reserve(4);
		unsigned int r{ m_zeroPosition.at(0) };
		unsigned int c{ m_zeroPosition.at(1) };
		if (r - 1 >= 1) possibleMoves.push_back(m_values.at(r - 1, c));
		if (r + 1 <= row_count) possibleMoves.push_back(m_values.at(r + 1, c));
		if (c - 1 >= 1) possibleMoves.push_back(m_values.at(r, c - 1));
		if (c + 1 <= column_count) possibleMoves.push_back(m_values.at(r, c + 1));
		return possibleMoves;
	}
	bool move(unsigned int value)
	{
		unsigned int& r{ m_zeroPosition.at(0) };
		unsigned int& c{ m_zeroPosition.at(1) };
		if (r - 1 >= 1 && m_values.at(r - 1, c) == value)
		{
			m_values.at(r, c) = value;
			--r;
			m_values.at(r, c) = 0;
			return true;
		}
		if (r + 1 <= row_count && m_values.at(r + 1, c) == value)
		{
			m_values.at(r, c) = value;
			++r;
			m_values.at(r, c) = 0;
			return true;
		}
		if (c - 1 >= 1 && m_values.at(r, c-1) == value)
		{
			m_values.at(r, c) = value;
			--c;
			m_values.at(r, c) = 0;
			return true;
		}
		if (c + 1 <= column_count && m_values.at(r, c + 1) == value)
		{
			m_values.at(r, c) = value;
			++c;
			m_values.at(r, c) = 0;
			return true;
		}
		return false;
	}
};

#endif // !PUZZLE_1_43_PM_28_12_2023_H
