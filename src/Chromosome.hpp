#pragma once

#include <memory>
#include <unordered_set>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Genes.hpp"



class Chromosome : public sf::Drawable
{
public:
	static std::minstd_rand* re;
	
	static void allowGeneType(Gene::GeneType gt, bool allowed=true);
	static void toggleGeneType(Gene::GeneType gt);
	static bool isGeneTypeAllowed(Gene::GeneType gt);
	static bool isAnyGeneTypeAllowed();
	
	Chromosome();  // create empty chromosome
	Chromosome(const Chromosome& father, const Chromosome& mother);  // crossover
	
	Chromosome(const Chromosome& other);  // copy constructor
	Chromosome& operator=(const Chromosome& other);  // copy assignment operator
	Chromosome(Chromosome&& other);  // copy constructor
	Chromosome& operator=(Chromosome&& other);  // copy assignment operator
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	void mutate();  // randomly mutate chromosome's genes
	
	size_t length() const;
	
private:
	static std::unordered_set<Gene::GeneType> allowedGeneTypes;
	
	std::vector<std::unique_ptr<Gene>> genes;
	
	std::vector<std::unique_ptr<Gene>>::iterator selectGene();
	std::vector<std::unique_ptr<Gene>>::const_iterator selectConstGene() const;
	
	void addGene();
	void removeGene();
	void swapGenes();
	void mutateGene();
};
