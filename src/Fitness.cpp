#include "Fitness.hpp"

#include <SFML/System.hpp>



Fitness::Fitness(sf::Texture target, float scale)
{
	this->target = target;
	
	sf::Vector2u targetSize = this->target.getSize();
	this->tex.create(targetSize.x, targetSize.y);
	this->sprite.setTexture(this->tex.getTexture());
	this->comp.create(targetSize.x*scale, targetSize.y*scale);
// 	this->comp.setSmooth(true);
	
	this->view.reset(sf::FloatRect(0, 0, targetSize.x, targetSize.y));
	this->view.setViewport(sf::FloatRect(0, 0, 1, 1));
}


bool Fitness::loadShader(std::string filename)
{
	this->compshdr.loadFromFile(filename, sf::Shader::Fragment);
	if (this->compshdr.isAvailable()) {
		this->compshdr.setUniform("base", this->target);
		this->compshdr.setUniform("curr", this->tex.getTexture());
		this->compshdr.setUniform("size", sf::Vector2f(this->comp.getSize()));
		return true;
	} else {
		return false;
	}
}


unsigned long long Fitness::of(Chromosome chr)
{
	// first, render a reduced, shaderized version to RenderTextures
	this->tex.clear();
	this->tex.draw(chr);
	this->tex.display();
	
	this->comp.clear(sf::Color::Blue);
	this->comp.setView(this->view);
	this->comp.draw(this->sprite, &this->compshdr);
// 	this->comp.draw(this->sprite);
	this->comp.display();
	
	// then, download the result as an image and add the pixels
	sf::Image image = this->comp.getTexture().copyToImage();
	sf::Vector2u size = image.getSize();
	unsigned long long fitness = 0;
	for (unsigned int x=0; x<size.x; ++x) {
		for (unsigned int y=0; y<size.y; ++y) {
			sf::Color color = image.getPixel(x, y);
			fitness += color.r + color.g + color.b;
		}
	}
	return fitness;
}
