#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Button 
{
private:
	RectangleShape button;
	Text textButton;
public:
	Button(float x, float y, std::wstring textButton, Font& font);
	void draw(RenderWindow& window);
	bool buttonClick(float mouseX, float mouseY);
};