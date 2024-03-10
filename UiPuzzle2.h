#ifndef UI_PUZZLE_2_7_43_AM_6_1_2024_H
#define UI_PUZZLE_2_7_43_AM_6_1_2024_H

#include "UiPuzzle2_Base.h"
#include "NumberTile.h"
#include "ImageTile.h"
// to do: template specialisation for numbers, text and image
// https://www.learncpp.com/cpp-tutorial/class-template-specialization/

template <unsigned int row_count, unsigned int column_count, class Tile>
class UiPuzzle2 : public UiPuzzle2_Base<row_count,column_count,Tile>
{
public:
};

template<unsigned int row_count, unsigned int column_count>
class UiPuzzle2<row_count, column_count, NumberTile> :public UiPuzzle2_Base<row_count, column_count, NumberTile>
{
	UiPuzzle2_Base<row_count, column_count, NumberTile> init(const sf::Vector2f& position,
		unsigned int cellSideLength, const sf::Font& font, unsigned int numberOfRandomMoves) const
	{
		Puzzle<row_count, column_count> puzzle{ numberOfRandomMoves };
		std::array<NumberTile, row_count* column_count> tiles{initTiles(position, cellSideLength, font, puzzle)};
		return UiPuzzle2_Base<row_count, column_count, NumberTile>{puzzle, tiles, position};
	}
public:
	const sf::Font* getFontPtr() const 
	{ 
		return UiPuzzle2_Base<row_count, column_count, NumberTile>::m_tiles.front().text.getFont(); 
	}
	UiPuzzle2<row_count, column_count, NumberTile>(const sf::Vector2f& position,
			unsigned int cellSideLength, const sf::Font& font, unsigned int numberOfRandomMoves)
		:UiPuzzle2_Base<row_count, column_count, NumberTile>{init(position,cellSideLength,font,numberOfRandomMoves)}
	{
	}
	void randomisePuzzle(unsigned int numberOfRandomMoves)
	{
		assert(getFontPtr());
		*(static_cast<UiPuzzle2_Base<row_count, column_count, NumberTile>*>(this)) 
			= init(UiPuzzle2_Base<row_count, column_count, NumberTile>::getTopLeftPosition(),
			UiPuzzle2_Base<row_count, column_count, NumberTile>::getCellSideLength(), 
			*(getFontPtr()), numberOfRandomMoves);
	}
};

template<unsigned int row_count, unsigned int column_count>
class UiPuzzle2<row_count, column_count, ImageTile> :public UiPuzzle2_Base<row_count, column_count, ImageTile>
{
	UiPuzzle2_Base<row_count, column_count, ImageTile> init(const sf::Vector2f& position,
		unsigned int cellSideLength,  const sf::Texture* texture, 
		unsigned int numberOfRandomMoves) const
	{
		Puzzle<row_count, column_count> puzzle{ numberOfRandomMoves };
		std::array<ImageTile, row_count* column_count> tiles{
			initTiles(position, cellSideLength, texture, puzzle)
		};
		return UiPuzzle2_Base<row_count, column_count, ImageTile>{puzzle, tiles, position};
	}
public:
	const sf::Texture* getTexture() const 
	{
		return UiPuzzle2_Base<row_count, column_count, ImageTile>::m_tiles.back().rectShape.getTexture();
	}
	unsigned int getCellSideLength() const
	{
		return UiPuzzle2_Base<row_count, column_count, ImageTile>::m_tiles.front().rectShape.getSize().x;
	}
	UiPuzzle2<row_count, column_count, ImageTile>(const sf::Vector2f& position,
		unsigned int cellSideLength, const sf::Texture* texture, 
		unsigned int numberOfRandomMoves)
		:UiPuzzle2_Base<row_count, column_count, ImageTile>{ 
			init(position,cellSideLength,texture,numberOfRandomMoves) 
		}
	{

	}
	void randomisePuzzle(unsigned int numberOfRandomMoves)
	{
		assert(getTexture());
		*(static_cast<UiPuzzle2_Base<row_count, column_count, ImageTile>*>(this)) 
			= init(UiPuzzle2_Base<row_count, column_count, ImageTile>::getTopLeftPosition(),
			UiPuzzle2_Base<row_count, column_count, ImageTile>::getCellSideLength(), 
			getTexture(), numberOfRandomMoves);
	}
};

#endif // !UI_PUZZLE_2_7_43_AM_6_1_2024_H