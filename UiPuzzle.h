#ifndef UI_PUZZLE_4_11_28_12_2023_H
#define UI_PUZZLE_4_11_28_12_2023_H

#include <algorithm>
#include "Puzzle.h"
#include "SFML/Graphics.hpp"
// to do: template specialisation for numbers, text and image
// https://www.learncpp.com/cpp-tutorial/class-template-specialization/
template <unsigned int row_count, unsigned int column_count>
class UiPuzzle: public Puzzle<row_count,column_count>, public sf::Drawable
{
public:
	struct TextRectShape: public sf::Drawable
	{
		sf::Text text;
		sf::RectangleShape rectShape;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{ target.draw(rectShape); target.draw(text, states);}
		const sf::Vector2f& getPosition() const { return text.getPosition(); }
		void setPosition(const sf::Vector2f& position)
		{text.setPosition(position); rectShape.setPosition(position);}
		void highlight() {
			rectShape.setFillColor(sf::Color::White);
			rectShape.setOutlineColor(sf::Color::Black);
			text.setFillColor(sf::Color::Black);
		}
		void unhighlight() {
			rectShape.setFillColor(sf::Color::Black);
			rectShape.setOutlineColor(sf::Color::White);
			text.setFillColor(sf::Color::White);
		}
	};
private:
	std::array<TextRectShape, row_count* column_count> m_textboxes;
	sf::Vector2f m_topLeftPosition;
public:
	UiPuzzle(const sf::Vector2f& position, unsigned int cellSideLength, const sf::Font& font, 
		unsigned int numberOfRandomMoves) 
		:Puzzle<row_count,column_count>{numberOfRandomMoves}, m_textboxes{ initTextBoxes(position,font,cellSideLength, *this)},
		m_topLeftPosition{position}
	{
	}
	unsigned int getCellSideLength() const { return static_cast<unsigned int>(m_textboxes.front().rectShape.getSize().x); }
private:
	sf::Text getMaxCharacterSizeText(unsigned int n, const sf::Font& font, unsigned int cellSideLength) const
	{
		// set max character size for each number
		unsigned int characterSize{ 500 };
		sf::Text text{ std::to_string(n),font,characterSize };
		float width{ text.getLocalBounds().width };
		float maxLength{ width > (static_cast<float>(characterSize)) ? width : static_cast<float>(characterSize) };
		characterSize = cellSideLength * characterSize / static_cast<unsigned int>(maxLength);
		text.setCharacterSize(characterSize);
		sf::FloatRect bounds{text.getLocalBounds()};
		text.setOrigin((bounds.width - cellSideLength) * 0.5f + bounds.left, (bounds.height - cellSideLength) * 0.5f + bounds.top);
		return text;
	}
	std::array<sf::Text, row_count* column_count> getMaxCharacterSizeTexts(
		const sf::Font& font, unsigned int cellSideLength) const
	{
		std::array<sf::Text, row_count* column_count> texts{};
		for (unsigned int n{ 0 }; n < row_count * column_count; ++n)
			texts.at(n) = getMaxCharacterSizeText(n, font, cellSideLength);
		return texts;
	}
	sf::Vector2f getBoxPosition(unsigned int r, unsigned int c, const sf::Vector2f& position, unsigned int cellSideLength) const
	{
		return sf::Vector2f {position.x + (c - 1) * cellSideLength, position.y + (r - 1) * cellSideLength};
	}
	sf::Text initText(unsigned int n, const sf::Font& font, unsigned int cellSideLength, const sf::Vector2f& boxPosition) const
	{
		sf::Text text;
		if (n)
			text = sf::Text{ getMaxCharacterSizeText(n, font, cellSideLength) };
		else
			text = sf::Text("", font);
		text.setPosition(boxPosition);
		return text;
	}
	sf::RectangleShape initRectShape(unsigned int cellSideLength, const sf::Vector2f& boxPosition) const
	{
		sf::RectangleShape rectShape{sf::Vector2f{
			static_cast<float>(cellSideLength),static_cast<float>(cellSideLength)
		} };
		rectShape.setFillColor(sf::Color::Black);
		rectShape.setOutlineColor(sf::Color::White);
		rectShape.setOutlineThickness(-1);
		rectShape.setPosition(boxPosition);
		return rectShape;
	}
	std::array<TextRectShape, row_count* column_count> initTextBoxes(const sf::Vector2f& position,
		const sf::Font& font, unsigned int cellSideLength, const Puzzle<row_count, column_count>& puzzle) const
	{
		std::array<TextRectShape, row_count* column_count> textBoxes;
		//set positions of texts
		unsigned int i{ 1 };
		for (int r{ 1 }; r <= row_count; ++r)
		{
			for (int c{ 1 }; c <= column_count; ++c, ++i)
			{
				unsigned int n{ puzzle.at(r,c) };
				TextRectShape& textBox{ textBoxes.at(n) };
				sf::Vector2f boxPosition{getBoxPosition(r, c, position, cellSideLength)};
				textBox.text = initText(n, font, cellSideLength, boxPosition);
				textBox.rectShape = initRectShape(cellSideLength, boxPosition);
				if (n == row_count * column_count) continue;
				if (n == i) 
					textBox.highlight(); 
				else 
					textBox.unhighlight();
			}
		}
		return textBoxes;
	}
	bool move(unsigned int r, unsigned int c)
	{
		unsigned int nR {Puzzle<row_count, column_count>::getZeroPosition().at(0)};
		unsigned int nC{ Puzzle<row_count, column_count>::getZeroPosition().at(1) };
		unsigned int n{ Puzzle<row_count, column_count>::at(r, c) };
		if (!Puzzle<row_count, column_count>::move(n)) return false;
		sf::Vector2f zeroPosition{m_textboxes.at(n).getPosition()};
		sf::Vector2f valuePosition{m_textboxes.at(0).getPosition()};
		m_textboxes.at(0).setPosition(zeroPosition);
		m_textboxes.at(n).setPosition(valuePosition);
		TextRectShape& textbox{ m_textboxes.at(n) };
		if (n == ((nR - 1) * column_count + nC))
			textbox.highlight();
		else
			textbox.unhighlight();
		return true;
	}
public:
	bool move(unsigned int value)
	{
		if (!Puzzle<row_count, column_count>::move(value)) return false;
		sf::Vector2f zeroPosition{m_textboxes.at(value).getPosition()};
		sf::Vector2f valuePosition{m_textboxes.at(0).getPosition()};
		m_textboxes.at(0).setPosition(zeroPosition);
		m_textboxes.at(value).setPosition(valuePosition);
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
			static_cast<sf::Vector2i>(mousePosition - m_topLeftPosition) / static_cast<int>(cellSideLength) + sf::Vector2i{1,1}
		};
		if (!(coordinates.x >= 1 && coordinates.x <= column_count && coordinates.y >= 1 && coordinates.y <= row_count)) return false;
		return move(coordinates.y, coordinates.x);
		//return move (Puzzle<row_count, column_count>::at(coordinates.y, coordinates.x));
	}
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//for (auto& textBox : m_textboxes)
		//	target.draw(textBox, states);
		std::for_each(m_textboxes.begin(), m_textboxes.end(),
			[&target, states] (auto & textbox) mutable
		{
			target.draw(textbox, states);
		});
	}
};

#endif // !UI_PUZZLE_4_11_28_12_2023_H
