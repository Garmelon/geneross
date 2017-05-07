#pragma once

#include <memory>
#include <unordered_set>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Genes.hpp"



class Chromosome : public sf::Drawable
{
public:
	enum GeneType
	{
		Circle
	};
	
	static std::minstd_rand* re;
	
	static void allowGeneType(GeneType gt, bool allowed=true);
	static bool isGeneTypeAllowed(GeneType gt);
	
	Chromosome();  // create empty chromosome
	Chromosome(Chromosome& father, Chromosome& mother);  // crossover
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	void mutate();  // randomly mutate chromosome's genes
	
	size_t length() const;
	
private:
	static std::unordered_set<GeneType> allowedGeneTypes;
	
	std::vector<std::unique_ptr<Gene>> genes;
	
	std::vector<std::unique_ptr<Gene>>::iterator selectGene();
	
	void addGene();
	void removeGene();
	void swapGenes();
	void mutateGene();
};
