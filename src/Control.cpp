#include "Control.hpp"

#include <iostream>

/*
 * Setup:
 *  - current item
 *  - circles
 *  - triangles
 *  - spheres
 *  - max number limited?
 * 
 * Generations:
 *  - paused
 *  - save image
 *  - display mode
 *  - saving?
 * 
 * StepThrough:
 *  - which step am I at? -> enums
 */

Fitness* Control::fitness;
Generation* Control::generation;

float Control::barMargin = 4;


Control::Control(float winW, float winH, std::string name) :
	screenSetup(this)
{
	this->window.create(sf::VideoMode(winW, winH), name);
	
// 	this->targetSprite.setTexture(Control::fitness->target.getTexture());
// 	this->chromosomeSprite.setTexture(Control::fitness->chromosome.getTexture());
	
// 	this->currentScreen = &this->screenSetup;
// 	this->nextScreen = this->currentScreen;
	
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
	this->currentScreen = nullptr;
	this->switchScreen(this->screenSetup);
	
	std::cout << "Starting interactive loop" << std::endl;
	sf::Clock clock;
	while (this->window.isOpen()) {
		if (this->currentScreen != this->nextScreen) {
			if (this->currentScreen != nullptr) this->currentScreen->exit();
			if (this->nextScreen == nullptr) {
				std::cout << "Switched to null screen; window closed" << std::endl;
				window.close();
				continue;
			}
			this->currentScreen = this->nextScreen;
			this->currentScreen->enter();
		}
		
		// handle window events
		sf::Event event;
		while (this->window.pollEvent(event)) {
			this->currentScreen->event(event);
		}
		
		// update screen
		this->currentScreen->update(clock.restart());
		
		// draw screen
		this->window.clear(sf::Color::White);
		this->window.draw(*this->currentScreen);
		this->window.display();
	}
	
	std::cout << "Interactive loop stopped; window closed" << std::endl;
}


void Control::switchScreen(Screen& newScreen)
{
	std::cout << "Switched screens" << std::endl;
	this->nextScreen = &newScreen;
}


void Control::close()
{
	this->nextScreen = nullptr;
}


void Control::setFont(const sf::Font& font)
{
	this->leftText.setFont(font);
	this->rightText.setFont(font);
	
	this->screenSetup.setFont(font);
	
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


void Control::setText(const std::string& left, const std::string& right)
{
	this->leftText.setString(left);
	this->rightText.setString(right);
	
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


void Control::drawBar(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->bar, states);
	target.draw(this->rightText, states);
	target.draw(this->leftText, states);
}


/*
void Control::modeGenerations()
{
	// handle window events
	sf::Event event;
	while (this->window.pollEvent(event)) {
		// window closed
		if (event.type == sf::Event::Closed) {
			window.close();
			return;
		}
		
		// key pressed
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Q) {
				window.close();
				return;
			}
		}
	}
	
	this->window.clear();
	this->window.draw(this->bar);
	this->window.draw(this->rightText);
	this->window.draw(this->leftText);
	this->window.display();
}
*/
