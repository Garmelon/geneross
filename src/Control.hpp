#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Chromosome.hpp"
#include "Fitness.hpp"
#include "Generation.hpp"
#include "Screens.hpp"


/*
 * TODO:
 * display modes etc.
 *  - overlap (transparency)
 *  - side-by-side
 * 
 * What I want:
 *  - generation screen
 *   - side-by-side/overlap view
 *   - generation-specific bar
 * 
 *  - stepping-through screen
 *   - show whole generation?
 *   - evaluate fitness one-by-one
 *    - side-by-side/overlap view
 *    - chromosome-specific bar
 */


class Control {
public:
	static Fitness* fitness;
	static Generation* generation;
	
	static float barMargin;
	
	sf::RenderWindow window;
	sf::Vector2f getSizeWithBar();
	
	ScreenSetup screenSetup;
	ScreenGenerations screenGenerations;
	
	Control(float winW, float winH, std::string name);  // creates a window
	~Control();
	
	void interactive();
	
	void switchScreen(Screen& newScreen);
	void close();
	
	void setFont(const sf::Font& font);
	void setLeftText(const std::string& text);
	void setRightText(const std::string& text);
	void setText(const std::string& leftText, const std::string& rightText="");
	
	void drawBar(sf::RenderTarget& target, sf::RenderStates states) const;
	
private:
	
	Screen* currentScreen;
	Screen* nextScreen;
	
	sf::RectangleShape bar;
	sf::Text leftText;
	sf::Text rightText;
	
	void updateBar();
};
