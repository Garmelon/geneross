#pragma once

#include <random>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>



class Chromosome : public sf::Drawable
{
public:
	static sf::Vector2f size;
	static float stddev_position;  // percent of max_radius
	static float stddev_radius;    // percent of max_radius
	static float stddev_color;
	static std::mt19937_64* re;
	
	Chromosome();  // create empty chromosome
	Chromosome(Chromosome& father, Chromosome& mother);  // crossover
	
	void mutate();  // randomly mutate chromosome's genes
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	size_t length();
	
protected:
	struct Gene
	{
		sf::Vector2f position;
		float radius;
		sf::Color color;
	};
	
	Gene randomGene();
	void mutateGene(Gene& gene);
	
	std::pair<std::vector<Gene>::iterator, std::vector<Gene>::iterator>
	selectSegment(std::vector<Gene>& genes);
	
	std::vector<Gene>::iterator selectGene(std::vector<Gene>& genes);
	
	mutable sf::CircleShape circle;  // drawing the chromosome, one draw call at a time
	std::vector<Gene> genes;
};
