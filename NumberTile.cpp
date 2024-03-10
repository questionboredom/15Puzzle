#include "NumberTile.h"
void NumberTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectShape,states); target.draw(text, states);
}
const sf::Vector2f& NumberTile::getPosition() const { return text.getPosition(); }
void NumberTile::setPosition(const sf::Vector2f& position)
{
	text.setPosition(position); rectShape.setPosition(position);
}
void NumberTile::highlight() 
{
	rectShape.setFillColor(sf::Color::White);
	rectShape.setOutlineColor(sf::Color::Black);
	text.setFillColor(sf::Color::Black);
}
void NumberTile::unhighlight() {
	rectShape.setFillColor(sf::Color::Black);
	rectShape.setOutlineColor(sf::Color::White);
	text.setFillColor(sf::Color::White);
}
sf::Text getMaxCharacterSizeText(unsigned int n, const sf::Font& font, unsigned int cellSideLength)
{
	// set max character size for each number
	unsigned int characterSize{ 500 };
	sf::Text text{ std::to_string(n), font, characterSize };
	float width{ text.getLocalBounds().width };
	float maxLength{ width > (static_cast<float>(characterSize)) ? width : static_cast<float>(characterSize) };
	characterSize = cellSideLength * characterSize / static_cast<unsigned int>(maxLength);
	text.setCharacterSize(characterSize);
	sf::FloatRect bounds{text.getLocalBounds()};
	text.setOrigin((bounds.width - cellSideLength) * 0.5f + bounds.left, (bounds.height - cellSideLength) * 0.5f + bounds.top);
	return text;
}
sf::Text initText(unsigned int n, const sf::Font& font, unsigned int cellSideLength, const sf::Vector2f& boxPosition)
{
	sf::Text text;
	if (n)
		text = sf::Text{ getMaxCharacterSizeText(n, font, cellSideLength) };
	else
		text = sf::Text("", font);
	text.setPosition(boxPosition);
	return text;
}
sf::RectangleShape initRectShape(unsigned int cellSideLength, const sf::Vector2f& boxPosition)
{
	sf::RectangleShape rectShape{sf::Vector2f{
		static_cast<float>(cellSideLength), static_cast<float>(cellSideLength)
	} };
	rectShape.setFillColor(sf::Color::Black);
	rectShape.setOutlineColor(sf::Color::White);
	rectShape.setOutlineThickness(-1);
	rectShape.setPosition(boxPosition);
	return rectShape;
}