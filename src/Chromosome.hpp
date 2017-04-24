#pragma once

#include <SFML/Graphics.hpp>



class Chromosome : public sf::Drawable
{
public:
	Chromosome();  // create empty chromosome
	Chromosome(Chromosome& father, Chromosome& mother);  // cross over two chromosomes
	
	void mutate();  // randomly mutate chromosome's genes
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
private:
	// TODO: random numbers
	// pass as reference to Chromosome and Mutate?
	// pass as reference on object creation?
	
	struct Gene
	{
		sf::Vector2f position;
		float size;
		sf::Color color;
	};
	
	sf::CircleShape circle;  // drawing the chromosome, one draw call at a time
	
	size_t genesize = 0;
	std::forward_list<Gene> genes;
};
