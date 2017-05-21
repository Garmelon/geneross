#include "Fitness.hpp"

#include <SFML/System.hpp>



sf::Color Fitness::backgroundColor = sf::Color::White;


Fitness::Fitness() :
	dummy(sf::TriangleFan, 4)
{
}


bool Fitness::loadShader(std::string filename)
{
	this->shaderLoaded = this->compshdr.loadFromFile(filename, sf::Shader::Fragment);
	if (this->shaderLoaded) {
		this->compshdr.setUniform("base", this->target.getTexture());
		this->compshdr.setUniform("curr", this->chromosome.getTexture());
		this->compshdr.setUniform("size", sf::Vector2f(this->target.getSize()));
		this->compshdr.setUniform("horizontal", this->horizontal);
		return true;
	} else {
		return false;
	}
}


bool Fitness::loadTarget(std::string filename)
{
	// so create() isn't called twice on the same RenderTextures
// 	if (this->targetLoaded) {
		// TODO: Test if this is necessary
		
		// DANGER ZONE :P
		// First call destructor of object, then create new instance of object
		// because the RenderTexture doesn't support this->target = RenderTexture();
		
		// Do not separate these lines!
// 		this->target.~RenderTexture();
// 		new (&this->target) sf::RenderTexture();
		
		// Do not separate these lines!
// 		this->comp.~RenderTexture();
// 		new (&this->comp) sf::RenderTexture();
		
		// Do not separate these lines!
// 		this->chromosome.~RenderTexture();
// 		new (&this->chromosome) sf::RenderTexture();
// 	}
	
	this->targetLoaded = false;
	
	// load the image
	sf::Texture tex;
	if (!tex.loadFromFile(filename)) {
		return false;
	}
	
	// create render textures in the needed size
	sf::Vector2u targetSize = tex.getSize();
	if (!this->target.create(targetSize.x, targetSize.y)) {
		return false;
	}
	if (!this->chromosome.create(targetSize.x, targetSize.y)) {
		return false;
	}
	
	// render target image to render texture
	sf::Sprite spr(tex);
	this->target.clear(Fitness::backgroundColor);
	this->target.draw(spr);
	this->target.display();
	
	// do "horizontal" stuff
	this->horizontal = targetSize.x >= targetSize.y;
	if (this->horizontal) {
		targetSize.x = 1;
	} else {
		targetSize.y = 1;
	}
	
	// create render texture needed for computing fitness
	if (!this->comp.create(targetSize.x, targetSize.y)) {
		return false;
	}
	
	// set dummy vertices to comp dimensions
	// the colors don't matter, so why not rainbow 'em?
	this->dummy[0] = sf::Vertex(sf::Vector2f(0, 0),            sf::Color::Red   );
	this->dummy[1] = sf::Vertex(sf::Vector2f(targetSize.x, 0), sf::Color::Yellow);
	this->dummy[2] = sf::Vertex(sf::Vector2f(targetSize),      sf::Color::Green );
	this->dummy[3] = sf::Vertex(sf::Vector2f(0, targetSize.y), sf::Color::Blue  );
	
	// update shader uniforms
	if (this->shaderLoaded) {
		this->compshdr.setUniform("size", sf::Vector2f(this->target.getSize()));
		this->compshdr.setUniform("horizontal", this->horizontal);
	}
	
	// Yay, loading sucessful!
	this->targetLoaded = true;
	return true;
}


void Fitness::render(const Chromosome& chr)
{
	this->chromosome.clear(Fitness::backgroundColor);
	this->chromosome.draw(chr);
	this->chromosome.display();
}


unsigned long long Fitness::compute()
{
	// compare the two textures using the SUPER ADVANCED shader
	this->comp.clear(sf::Color::Blue);
	this->comp.draw(this->dummy, &this->compshdr);
	this->comp.display();
	
	// then, download the result as an image and extract the result
	sf::Image image = this->comp.getTexture().copyToImage();
	sf::Vector2u size = image.getSize();
	
	// TODO: Deal with horizontal-ness properly (not this ↓ ugly)
	unsigned long long fitness = 0;
	if (this->horizontal) {
		for (unsigned int y=0; y<size.y; ++y) {
			sf::Color col = image.getPixel(0, y);
			fitness += (unsigned long long)col.r;        // 2^(8*0) - first byte
			fitness += (unsigned long long)col.g*256;    // 2^(8*1) - second byte
			fitness += (unsigned long long)col.b*65536;  // 2^(8*2) - third byte
		}
	} else {
		for (unsigned int x=0; x<size.x; ++x) {
			sf::Color col = image.getPixel(x, 0);
			fitness += (unsigned long long)col.r;        // 2^(8*0) - first byte
			fitness += (unsigned long long)col.g*256;    // 2^(8*1) - second byte
			fitness += (unsigned long long)col.b*65536;  // 2^(8*2) - third byte
		}
	}
	
	return fitness;
}


unsigned long long Fitness::of(const Chromosome& chr)
{
	this->render(chr);
	return this->compute();
}
