#include "../Headers/Cell.h"

Cell::Cell(float x, float y)
{
	this->cell.setPosition(Vector2f(x, y));
	this->cell.setSize(Vector2f(10 ,10));
	this->color = 0;
}
void Cell::draw(RenderWindow& window) 
{
	if (color > 1) {
		color = 1;
	}
	else if (color < 0) {
		color = 0;
	}

	if (color == 1) 
	{
		this->cell.setFillColor(Color::Green);
	}
	else if (color == 0) 
	{
		this->cell.setFillColor(Color::Color(128, 128, 128));
	}
	else
	{
		this->cell.setFillColor(Color::Color(0, std::clamp((int)(255 * color), 0, 255), 0));
	}
	window.draw(this->cell);
}
void Cell::setColor(float color)
{
	this->color = color;
}
float Cell::getColor() 
{
	return this->color;
}