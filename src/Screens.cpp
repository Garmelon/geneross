#include "Screens.hpp"

#include "Control.hpp"

#include <iostream>

// Screen base class
Screen::Screen(Control* controlptr)
{
	this->control = controlptr;
}


Screen::~Screen()
{
}



// ScreenSetup setup screen
// const sf::Vector2f ScreenSetup::offset(20, 20);
// const float ScreenSetup::distance = 50;
// const int ScreenSetup::items = 3;


ScreenSetup::ScreenSetup(Control* controlptr) :
	Screen(controlptr),
	offset(20, 20),
	distance(30),
	items(4)
{
	this->selection = 0;
	
	this->text.setCharacterSize(20);
	this->text.setFillColor(sf::Color::Black);
	
	this->rect.setFillColor(sf::Color::Yellow);
}


void ScreenSetup::enter()
{
	std::cout << "ENTERED!" << std::endl;
	this->rect.setSize(
		sf::Vector2f(
			this->control->window.getSize().x,
			30
		)
	);
	
	this->selectItem(0);
}


void ScreenSetup::event(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		this->keyPressed(event);
	}
}


void ScreenSetup::update(sf::Time dt)
{
	if (this->warnTimer > sf::Time::Zero) {
		this->warnTimer -= dt;
	}
}


void ScreenSetup::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	this->drawMenu(target, states);
	
	if (this->warnTimer > sf::Time::Zero) {
		this->control->drawBar(target, states);
	}
}


void ScreenSetup::exit()
{
}


void ScreenSetup::setFont(const sf::Font& font)
{
	this->text.setFont(font);
}


void ScreenSetup::keyPressed(sf::Event& event)
{
	switch (event.key.code) {
		// close window, stop program
		case sf::Keyboard::Q:
			this->control->close();
			return;
			
		// move selection up in menu
		case sf::Keyboard::Up:
			if (event.key.shift) this->selectItem(0);
			else this->selectItem(this->selection - 1);
			break;
			
		// move selection down in menu
		case sf::Keyboard::Down:
			if (event.key.shift) this->selectItem(this->items - 1);
			else this->selectItem(this->selection + 1);
			break;
			
		// select current item and perform action
		case sf::Keyboard::Return:
			this->executeItem(this->selection);
			
		default: break;
	}
}


void ScreenSetup::selectItem(int position)
{
	// kinda ugly, just to get the same % behaviour as in python
	this->selection = ((position%this->items) + this->items)%this->items;
	this->rect.setPosition(
		sf::Vector2f(
			0,
			this->offset.y + this->distance*this->selection
		)
	);
}


void ScreenSetup::executeItem(int position)
{
	switch (position) {
		case 0: Chromosome::toggleGeneType(Gene::Circle); break;
		case 1: Chromosome::toggleGeneType(Gene::Triangle); break;
		case 2: break; // spacing placeholder
		case 3:
			if (Chromosome::isAnyGeneTypeAllowed()) {
// 				this->control->switchScreen(Control::screenGenerations);
			} else {
				this->control->setText("Allow at least one gene type to continue.");
				this->warnTimer = sf::seconds(3);
			}
			break;
	}
}


void ScreenSetup::drawMenu(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->rect);
	
	sf::Vector2f position = this->offset;
	
	this->drawMenuItem(
		target, states, position, "Circle genes:  ",
		Chromosome::isGeneTypeAllowed(Gene::Circle)?"x":" "
	);
	position.y += this->distance;
	
	this->drawMenuItem(
		target, states, position, "Triangle genes:",
		Chromosome::isGeneTypeAllowed(Gene::Triangle)?"x":" "
	);
	position.y += 2*this->distance;
	
	this->drawMenuItem(
		target, states, position, "Continue"
	);
}


void ScreenSetup::drawMenuItem(sf::RenderTarget& target, sf::RenderStates states,
	sf::Vector2f position, std::string name, std::string choice) const
{
	if (choice != "") {
		name += " [";
		name += choice;
		name += "]";
	}
	
	this->text.setString(name);
	this->text.setPosition(position);
	target.draw(text, states);
}
