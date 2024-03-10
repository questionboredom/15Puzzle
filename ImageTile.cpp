#include "ImageTile.h"
void ImageTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectShape, states);
}
const sf::Vector2f& ImageTile::getPosition() const { return rectShape.getPosition(); }

void ImageTile::setPosition(const sf::Vector2f& position)
{
	rectShape.setPosition(position);
}

sf::RectangleShape initRectShape(unsigned int n, const sf::Texture* texture, 
	const sf::IntRect& textureRect, unsigned int cellSideLength ,const sf::Vector2f& boxPosition)
{
	sf::RectangleShape rectShape{sf::Vector2f{static_cast<float>(cellSideLength), static_cast<float>(cellSideLength)}};
	if (n)
	{
		rectShape.setTexture(texture);
		rectShape.setTextureRect(textureRect);
	}
	else
	{
		rectShape.setTexture(nullptr);
	}
	rectShape.setPosition(boxPosition);
	return rectShape;
}
