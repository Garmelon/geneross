#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Control;



class Screen : public sf::Drawable
{
public:
	Screen(Control* controlptr);
	virtual ~Screen();
	
	virtual void enter() =0;
	virtual void event(sf::Event& event) =0;
	virtual void update(sf::Time dt) =0;
// 	virtual void draw() const =0;
	virtual void exit() =0;
	
protected:
	Control* control;
};



class ScreenSetup : public Screen
{
public:
	ScreenSetup(Control* controlptr);
	
	virtual void enter();
	virtual void event(sf::Event& event);
	virtual void update(sf::Time dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void exit();
	
	void setFont(const sf::Font& font);
	
private:
	sf::Vector2f offset;
	float distance;
	int items;
	
	int selection;
	mutable sf::Text text;
	sf::RectangleShape rect;
	
	sf::Time warnTimer;
	
	void keyPressed(sf::Event& event);
	void selectItem(int position);
	void executeItem(int position);
	void drawMenu(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawMenuItem(sf::RenderTarget& target, sf::RenderStates states, sf::Vector2f position,
		std::string name, std::string choice="") const;
};



class ScreenGenerations : public Screen
{
};



class ScreenStepThrough : public Screen
{
};
