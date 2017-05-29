#include "Control.hpp"

#include <iostream>

Fitness* Control::fitness;
Generation* Control::generation;

float Control::barMargin = 4;


Control::Control(float winW, float winH, std::string name)
{
	this->window.create(sf::VideoMode(winW, winH), name);
	
	this->targetSprite.setTexture(Control::fitness->target.getTexture());
	this->chromosomeSprite.setTexture(Control::fitness->chromosome.getTexture());
	
	this->leftText.setCharacterSize(20);
	this->leftText.setFillColor(sf::Color::Black);
	
	this->rightText.setCharacterSize(20);
	this->rightText.setFillColor(sf::Color::Black);
	
	this->bar.setFillColor(sf::Color(127, 127, 127));
	
	this->updateBar();
}


Control::~Control()
{
}


void Control::interactive()
{
	while (this->window.isOpen()) {
		sf::Event event;
		while (this->window.pollEvent(event)) {
		}
		
		this->window.clear();
		this->window.draw(this->bar);
		this->window.draw(this->rightText);
		this->window.draw(this->leftText);
		this->window.display();
	}
}


void Control::setFont(const sf::Font& font)
{
	this->leftText.setFont(font);
	this->rightText.setFont(font);
	
	this->updateBar();
}


void Control::setLeftText(const std::string& text)
{
	this->leftText.setString(text);
	
	this->updateBar();
}


void Control::setRightText(const std::string& text)
{
	this->rightText.setString(text);
	
	this->updateBar();
}


void Control::updateBar()
{
	// update bar size and position
	this->bar.setSize(
		sf::Vector2f(
			this->window.getSize().x,
			this->leftText.getCharacterSize() + 2*Control::barMargin
		)
	);
	
	this->bar.setPosition(
		sf::Vector2f(
			0,
			this->window.getSize().y - this->bar.getSize().y
		)
	);
	
	// update left text size and position
	sf::FloatRect bounds = this->leftText.getLocalBounds();
	this->leftText.setOrigin(
		sf::Vector2f(
			bounds.left,
			this->rightText.getCharacterSize()/4  // slightly hacky solution to text offset issue
		)
	);
	
	this->leftText.setPosition(
		sf::Vector2f(
			Control::barMargin,
			this->window.getSize().y - this->leftText.getCharacterSize() - Control::barMargin
		)
	);
	
	// update right text size and position
	bounds = this->rightText.getLocalBounds();
	this->rightText.setOrigin(
		sf::Vector2f(
			bounds.left + bounds.width,
			this->rightText.getCharacterSize()/4  // slightly hacky solution to text offset issue
		)
	);
	
	this->rightText.setPosition(
		sf::Vector2f(
			this->window.getSize().x - Control::barMargin,
			this->window.getSize().y - this->rightText.getCharacterSize() - Control::barMargin
		)
	);
}
