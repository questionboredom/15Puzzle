#ifndef BASE_TILE_10_11_AM_6_1_2024_H
#define BASE_TILE_10_11_AM_6_1_2024_H
#include "SFML/Graphics.hpp"
inline sf::Vector2f getBoxPosition(unsigned int r, unsigned int c, const sf::Vector2f& position, unsigned int cellSideLength)
{
	return sf::Vector2f {position.x + (c - 1) * cellSideLength, position.y + (r - 1) * cellSideLength};
}
#endif // !BASE_TILE_10_11_AM_6_1_2024_H
