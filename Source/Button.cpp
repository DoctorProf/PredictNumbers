#include "../Headers/Button.h"

Button::Button(float x, float y, std::wstring textButton, Font& font) 
{
	this->button.setPosition(Vector2f(x, y));
	this->button.setSize(Vector2f(100, 30));
	this->button.setFillColor(Color::Color(128, 128, 128));
	this->textButton.setString(textButton);
	this->textButton.setFont(font);
	this->textButton.setFillColor(Color::White);
	this->textButton.setCharacterSize(12);
	this->textButton.setPosition(Vector2f(x + 15, y + 7));
}
void Button::draw(RenderWindow& window) 
{
	window.draw(this->button);
	window.draw(this->textButton);
}
bool Button::buttonClick(float mouseX, float mouseY)
{
	Vector2f position = button.getPosition();
	Vector2f size = button.getSize();
	return (mouseX >= position.x && mouseX <= size.x + position.x) && (mouseY >= position.y && mouseY <= size.y + position.y);
}