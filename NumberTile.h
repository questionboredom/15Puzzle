#ifndef NUMBER_TILE_8_51_AM_6_1_2024_H
#define NUMBER_TILE_8_51_AM_6_1_2024_H
#include "BaseTile.h"
#include "Puzzle.h"
#include "SFML/Graphics.hpp"
class NumberTile : public sf::Drawable
{
public:
	sf::Text text;
	sf::RectangleShape rectShape;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& position);
	void highlight();
	void unhighlight();
	unsigned int getSideLength() const { return static_cast<unsigned int>(rectShape.getSize().x); }
};
sf::Text getMaxCharacterSizeText(unsigned int n, const sf::Font& font, unsigned int cellSideLength);
sf::Text initText(unsigned int n, const sf::Font& font, unsigned int cellSideLength, const sf::Vector2f& boxPosition);
sf::RectangleShape initRectShape(unsigned int cellSideLength, const sf::Vector2f& boxPosition);
template <unsigned int row_count, unsigned int column_count>
std::array<NumberTile, row_count* column_count> initTiles(const sf::Vector2f& position,
	 unsigned int cellSideLength, const sf::Font& font, const Puzzle<row_count, column_count>& puzzle)
{
	std::array<NumberTile, row_count* column_count> tiles;
	//set positions of texts
	unsigned int i{ 1 };
	for (int r{ 1 }; r <= row_count; ++r)
	{
		for (int c{ 1 }; c <= column_count; ++c, ++i)
		{
			unsigned int n{ puzzle.at(r,c) };
			NumberTile& tile{ tiles.at(n) };
			sf::Vector2f boxPosition{getBoxPosition(r, c, position, cellSideLength)};
			tile.text = initText(n, font, cellSideLength, boxPosition);
			tile.rectShape = initRectShape(cellSideLength, boxPosition);
			if (n == row_count * column_count) continue;
			if (n == i)
				tile.highlight();
			else
				tile.unhighlight();
		}
	}
	return tiles;
}
#endif // !NUMBER_TILE_8_51_AM_6_1_2024_H