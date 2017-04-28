#include "Fitness.hpp"

#include <SFML/System.hpp>



Fitness::Fitness(sf::Texture target, float scale) :
	dummy(sf::TriangleFan, 4)
{
	this->target = target;
	
	sf::Vector2u targetSize = this->target.getSize();
	
	this->tex.create(targetSize.x, targetSize.y);
	
// 	this->view.reset(sf::FloatRect(0, 0, targetSize.x, targetSize.y));
// 	this->view.setViewport(sf::FloatRect(0, 0, 1, 1));
	
	this->horizontal = targetSize.x > targetSize.y;
	if (this->horizontal) {
		targetSize.x = 1;
	} else {
		targetSize.y = 1;
// 		this->comp.create(targetSize.x*scale, 2);
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
		this->compshdr.setUniform("size", sf::Vector2f(this->comp.getSize()));
		this->compshdr.setUniform("horizontal", this->horizontal);
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
// 	this->comp.setView(this->view);
	this->comp.draw(this->dummy, &this->compshdr);
// 	this->comp.draw(this->sprite, &this->compshdr);
// 	this->comp.draw(this->sprite);
	this->comp.display();
	
	// then, download the result as an image and add the pixels
	sf::Image image = this->comp.getTexture().copyToImage();
	sf::Vector2u size = image.getSize();
	unsigned long long fitness = 0;
// 	for (unsigned int x=0; x<size.x; ++x) {
// 		for (unsigned int y=0; y<size.y; ++y) {
// 			sf::Color color = image.getPixel(x, y);
// 			fitness += color.r + color.g + color.b;
// 		}
// 	}
	
	if (this->horizontal) {
		for (unsigned int y=0; y<size.y; ++y) {
			sf::Color col = image.getPixel(0, y);
			fitness += col.r;                    // 2^(8*0)
			fitness += col.g*256;                // 2^(8*1)
			fitness += col.b*65536;              // 2^(8*2)
			fitness += col.a*16777216;           // 2^(8*3)
// 			col = image.getPixel(1, y);
// 			fitness += col.r*4294967296;         // 2^(8*4)
// 			fitness += col.g*1099511627776;      // 2^(8*5)
// 			fitness += col.b*281474976710656;    // 2^(8*6)
// 			fitness += col.a*72057594037927936;  // 2^(8*7)
		}
	} else {
		for (unsigned int x=0; x<size.x; ++x) {
			sf::Color col = image.getPixel(x, 0);
			fitness += col.r;                    // 2^(8*0)
			fitness += col.g*256;                // 2^(8*1)
			fitness += col.b*65536;              // 2^(8*2)
			fitness += col.a*16777216;           // 2^(8*3)
// 			col = image.getPixel(x, 1);
// 			fitness += col.r*4294967296;         // 2^(8*4)
// 			fitness += col.g*1099511627776;      // 2^(8*5)
// 			fitness += col.b*281474976710656;    // 2^(8*6)
// 			fitness += col.a*72057594037927936;  // 2^(8*7)
		}
	}
	
	return fitness;
}
