#pragma once

#include "Chromosome.hpp"
#include <SFML/Graphics.hpp>



class Fitness
{
public:
	Fitness(sf::Texture target);
	bool loadShader(std::string filename);
	
	unsigned long long of(const Chromosome& chr);
	
	sf::Texture target;      // base image to compare against
	sf::RenderTexture tex;   // big RenderWindow containg the Chromosome to be evaluated
	sf::RenderTexture comp;  // smaller RenderWindow which is downloaded as Image
	
	sf::VertexArray dummy;
	bool horizontal;
	
private:
	sf::Shader compshdr;  // shader to perform pixel-wise image diff with
};
