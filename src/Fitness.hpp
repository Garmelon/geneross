#pragma once

#include "Chromosome.hpp"
#include <SFML/Graphics.hpp>



class Fitness
{
public:
	Fitness();
	Fitness(sf::Texture target, float scale=0.5);
	bool loadShader(std::string filename);
	
	double of(Chromosome chr);
	
	sf::Texture target;      // base image to compare against
	sf::RenderTexture tex;   // big RenderWindow containg the Chromosome to be evaluated
	sf::RenderTexture comp;  // smaller RenderWindow which is downloaded as Image
	
private:
	sf::Sprite sprite;    // sprite to render tex to comp
	sf::Shader compshdr;  // shader to perform pixel-wise image diff with
	sf::View view;        // reduces tex to comp size while drawing
};
