#pragma once

#include "Chromosome.hpp"
#include <SFML/Graphics.hpp>



class Fitness
{
public:
	static sf::Color backgroundColor;
	
	Fitness();
	bool loadShader(std::string filename);
	bool loadTarget(std::string filename);
	
	void render(const Chromosome& chr);
	unsigned long long compute();
	
	unsigned long long of(const Chromosome& chr);  // uses render and compute
	
	sf::RenderTexture target;      // contains image to compare against
	sf::RenderTexture chromosome;  // contains the Chromosome to be evaluated
	
	sf::RenderTexture comp;  // smaller RenderWindow which is downloaded as Image
	
	bool horizontal;
	
private:
	bool shaderLoaded = false;
	bool targetLoaded = false;
	
	sf::Shader compshdr;  // shader to perform pixel-wise image diff with
	sf::VertexArray dummy;  // something to render so that the shader is run on every pixel
};
