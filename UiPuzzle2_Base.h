#ifndef UI_PUZZLE2_BASE_10_39_AM_6_1_2024_H
#define UI_PUZZLE2_BASE_10_39_AM_6_1_2024_H

#include <algorithm>
#include "Puzzle.h"
#include "SFML/Graphics.hpp"
template <unsigned int row_count, unsigned int column_count, class Tile>
class UiPuzzle2_Base : public Puzzle<row_count, column_count>, public sf::Drawable
{
public:
	std::array<Tile, row_count* column_count> m_tiles;
	sf::Vector2f m_topLeftPosition;
public:
	UiPuzzle2_Base(const Puzzle<row_count, column_count>& puzzle, 
		const std::array<Tile, row_count* column_count>& tiles,
		const sf::Vector2f& topLeftPosition)
		:Puzzle<row_count, column_count>{ puzzle }, m_tiles{tiles}, m_topLeftPosition{topLeftPosition}
	{}
	const sf::Vector2f& getTopLeftPosition() const { return m_topLeftPosition; }
	unsigned int getCellSideLength() const { return m_tiles.front().getSideLength(); }
private:
	bool move(unsigned int r, unsigned int c)
	{
		unsigned int nR{ Puzzle<row_count, column_count>::getZeroPosition().at(0) };
		unsigned int nC{ Puzzle<row_count, column_count>::getZeroPosition().at(1) };
		unsigned int n{ Puzzle<row_count, column_count>::at(r, c) };
		if (!Puzzle<row_count, column_count>::move(n)) return false;
		sf::Vector2f zeroPosition{m_tiles.at(n).getPosition()};
		sf::Vector2f valuePosition{m_tiles.at(0).getPosition()};
		m_tiles.at(0).setPosition(zeroPosition);
		m_tiles.at(n).setPosition(valuePosition);
		Tile& tile{ m_tiles.at(n) };
		if (n == ((nR - 1) * column_count + nC))
			tile.highlight();
		else
			tile.unhighlight();
		return true;
	}
public:
	bool move(unsigned int value)
	{
		if (!Puzzle<row_count, column_count>::move(value)) return false;
		sf::Vector2f zeroPosition{m_tiles.at(value).getPosition()};
		sf::Vector2f valuePosition{m_tiles.at(0).getPosition()};
		m_tiles.at(0).setPosition(zeroPosition);
		m_tiles.at(value).setPosition(valuePosition);
		return true;
	}
	void update(const sf::Event& event, sf::RenderWindow& window, float deltaTime)
	{
		movedEvent(event, window);
		event; window; deltaTime;
	}
	bool movedEvent(const sf::Event& event, sf::RenderWindow& window)
	{
		window;
		if (!(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left))
			return false;
		unsigned int cellSideLength{ getCellSideLength() };
		sf::Vector2f mousePosition{
			static_cast<float>(event.mouseButton.x),
				static_cast<float>(event.mouseButton.y)
		};
		sf::Vector2i coordinates {
			static_cast<sf::Vector2i>(mousePosition - m_topLeftPosition) / static_cast<int>(cellSideLength) + sf::Vector2i{1, 1}
		};
		if (!(coordinates.x >= 1 && coordinates.x <= column_count && coordinates.y >= 1 && coordinates.y <= row_count)) return false;
		return move(coordinates.y, coordinates.x);
		//return move (Puzzle<row_count, column_count>::at(coordinates.y, coordinates.x));
	}
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		std::for_each(m_tiles.begin(), m_tiles.end(),
			[&target, &states](auto& tile) mutable
			{
				target.draw(tile, states);
			});
		// for (auto& tile : m_tiles)
		//	target.draw(tile, states);
	}
};


#endif // !UI_PUZZLE2_BASE_10_39_AM_6_1_2024_H

