#ifndef IMAGE_TILE_9_36_AM_6_1_2024_H
#define IMAGE_TILE_9_36_AM_6_1_2024_H
#include "BaseTile.h"
#include "Puzzle.h"
#include "SFML/Graphics.hpp"
class ImageTile: public sf::Drawable
{
public:
	sf::RectangleShape rectShape;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& position);
	void highlight(){ rectShape.setFillColor(sf::Color::White); }
	void unhighlight() { rectShape.setFillColor(sf::Color{255, 255, 255, 127}); }
	unsigned int getSideLength() const { return static_cast<unsigned int>(rectShape.getSize().x); }
};
sf::RectangleShape initRectShape(unsigned int n, const sf::Texture* texture,
	const sf::IntRect& textureRect, unsigned int cellSideLength ,const sf::Vector2f& boxPosition);
template <unsigned int row_count, unsigned int column_count>
std::array<ImageTile, row_count* column_count> initTiles(const sf::Vector2f& position,
	unsigned int cellSideLength, const sf::Texture* texture, const Puzzle<row_count, column_count>& puzzle)
{
	assert(texture);
	// check texture if square and can be evenly divided
	const unsigned int textureRows{ texture->getSize().y };
	const unsigned int textureColumns{ texture->getSize().x };
	assert(textureColumns == textureRows);
	const unsigned int textureRectRows{ textureRows / row_count };
	assert((textureRectRows * row_count) == textureRows);
	const unsigned int textureRectColumns{ textureColumns / column_count };
	assert((textureRectColumns * column_count) == textureColumns);
	
	std::array<ImageTile, row_count* column_count> tiles;
	//set Textures to sprites
	unsigned int i{ 1 };
	for (int r{ 1 }; r <= row_count; ++r)
	{
		for (int c{ 1 }; c <= column_count; ++c, ++i)
		{
			unsigned int n{ puzzle.at(r,c) };
			ImageTile& tile{ tiles.at(n) };
			sf::Vector2f boxPosition{getBoxPosition(r, c, position, cellSideLength)};
			sf::IntRect textureRect{
				static_cast<int>(((n-1) % column_count )* textureRectColumns),
					static_cast<int>(((n-1) / column_count)*textureRectRows),
					static_cast<int>(textureRectColumns),static_cast<int>(textureRectRows)
			};
			tile.rectShape = initRectShape(n,texture, textureRect, cellSideLength, boxPosition);
			if (n == row_count * column_count) continue;
			if (n == i)
				tile.highlight();
			else
				tile.unhighlight();
		}
	}
	return tiles;
}

#endif // !IMAGE_TILE_9_36_AM_6_1_2024_H