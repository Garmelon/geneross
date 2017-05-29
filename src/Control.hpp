#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Chromosome.hpp"
#include "Fitness.hpp"
#include "Generation.hpp"


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
	
	Control(float winW, float winH, std::string name);  // creates a window
	~Control();
	
	void interactive();
	
	void setFont(const sf::Font& font);
	void setLeftText(const std::string& text);
	void setRightText(const std::string& text);
	
private:
	sf::RenderWindow window;
	
	sf::Sprite targetSprite;
	sf::Sprite chromosomeSprite;
	
	sf::RectangleShape bar;
	sf::Text leftText;
	sf::Text rightText;
	
	void updateBar();
};
