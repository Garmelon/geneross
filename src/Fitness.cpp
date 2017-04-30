#include "Fitness.hpp"

#include <SFML/System.hpp>



Fitness::Fitness(sf::Texture target) :
	dummy(sf::TriangleFan, 4)
{
	this->target = target;
	sf::Vector2u targetSize = this->target.getSize();
	this->tex.create(targetSize.x, targetSize.y);
	
	this->horizontal = targetSize.x >= targetSize.y;
	if (this->horizontal) {
		targetSize.x = 1;
	} else {
		targetSize.y = 1;
	}
	
	this->comp.create(targetSize.x, targetSize.y);
// 	this->comp.setSmooth(true);
	
	this->dummy[0] = sf::Vertex(sf::Vector2f(0, 0),            sf::Color::Red   );
	this->dummy[1] = sf::Vertex(sf::Vector2f(targetSize.x, 0), sf::Color::Yellow);
	this->dummy[2] = sf::Vertex(sf::Vector2f(targetSize),      sf::Color::Green );
	this->dummy[3] = sf::Vertex(sf::Vector2f(0, targetSize.y), sf::Color::Blue  );
}


bool Fitness::loadShader(std::string filename)
{
	this->compshdr.loadFromFile(filename, sf::Shader::Fragment);
	if (this->compshdr.isAvailable()) {
		this->compshdr.setUniform("base", this->target);
		this->compshdr.setUniform("curr", this->tex.getTexture());
		this->compshdr.setUniform("size", sf::Vector2f(this->target.getSize()));
		this->compshdr.setUniform("horizontal", this->horizontal);
		return true;
	} else {
		return false;
	}
}


unsigned long long Fitness::of(const Chromosome& chr)
{
	// first, render chr to a texture, so we can just compare two textures
	this->tex.clear();
	this->tex.draw(chr);
	this->tex.display();
	
	// compare the two textures using the SUPER ADVANCED shader
	this->comp.clear(sf::Color::Blue);
	this->comp.draw(this->dummy, &this->compshdr);
	this->comp.display();
	
	// then, download the result as an image and extract the result
	sf::Image image = this->comp.getTexture().copyToImage();
	sf::Vector2u size = image.getSize();
	
	unsigned long long fitness = 0;
	if (this->horizontal) {
		for (unsigned int y=0; y<size.y; ++y) {
			sf::Color col = image.getPixel(0, y);
			fitness += (unsigned long long)col.r;                    // 2^(8*0) - first byte
			fitness += (unsigned long long)col.g*256;                // 2^(8*1) - second byte
			fitness += (unsigned long long)col.b*65536;              // 2^(8*2) - third byte
		}
	} else {
		for (unsigned int x=0; x<size.x; ++x) {
			sf::Color col = image.getPixel(x, 0);
			fitness += (unsigned long long)col.r;                    // 2^(8*0) - first byte
			fitness += (unsigned long long)col.g*256;                // 2^(8*1) - second byte
			fitness += (unsigned long long)col.b*65536;              // 2^(8*2) - third byte
		}
	}
	
	return fitness;
}
