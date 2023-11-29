#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

using namespace sf;

class Cell 
{
private:
	RectangleShape cell;
	float color;
public:
	Cell(float x = 0, float y = 0);
	void draw(RenderWindow& window);
	void setColor(float color);
	float getColor();
};