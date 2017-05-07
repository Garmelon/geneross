#include "Chromosome.hpp"



std::minstd_rand* Chromosome::re;
std::unordered_set<Chromosome::GeneType> Chromosome::allowedGeneTypes;


void Chromosome::allowGeneType(GeneType gt, bool allowed)
{
	if (allowed) {
		Chromosome::allowedGeneTypes.insert(gt);
	} else {
		Chromosome::allowedGeneTypes.erase(gt);
	}
}


bool Chromosome::isGeneTypeAllowed(GeneType gt)
{
	return Chromosome::allowedGeneTypes.find(gt) != Chromosome::allowedGeneTypes.end();
}


Chromosome::Chromosome()
{
	// empty so you can choose between crossover and creating a new chromosome
}


Chromosome::Chromosome(Chromosome& father, Chromosome& mother)
{
	auto split_father = father.selectGene();
	auto split_mother = mother.selectGene();
	
	// reserve to father's size/capacity, and shrink_to_fit afterwards?
	
	Gene* geneptr;
	for (auto it=father.genes.begin(); it!=split_father; ++it) {
		*geneptr = **it;
		this->genes.push_back(std::unique_ptr<Gene>(geneptr));
	}
	
	for (auto it=split_mother; it!=mother.genes.end(); ++it) {
		*geneptr = **it;
		this->genes.push_back(std::unique_ptr<Gene>(geneptr));
	}
}


void Chromosome::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& geneptr : this->genes) {
		target.draw(*geneptr, states);
	}
}


void Chromosome::mutate()
{
	std::uniform_int_distribution<> mutatedist(0, 1);  // 1/2 of the time, it mutates everytime!
	std::uniform_int_distribution<> choicedist(0, 11);
	
	while (mutatedist(*Chromosome::re)) {
		int choice = choicedist(*Chromosome::re);
		
		if      (choice < 1) this->addGene();
		else if (choice < 2) this->removeGene();
		else if (choice < 4) this->swapGenes();
		else                 this->mutateGene();
		
// 			std::cout << "Swapped circles" << std::endl;
//			std::cout << "Mutated circle" << std::endl;
// 		}
	}
}


size_t Chromosome::length() const
{
	return this->genes.size();
}


std::vector<std::unique_ptr<Gene>>::iterator Chromosome::selectGene()
{
	if (genes.empty()) {
		return this->genes.end();
	} else {
		std::uniform_int_distribution<> posdist(0, this->genes.size()-1);
		return genes.begin() + posdist(*Chromosome::re);
	}
}


void Chromosome::addGene()
{
}


void Chromosome::removeGene()
{
	auto it = this->selectGene();
	if (it != this->genes.end()) {
		this->genes.erase(it);
	}
}


void Chromosome::swapGenes()
{
	auto it_one = this->selectGene();
	auto it_two = this->selectGene();
	if (it_one != this->genes.end() && it_two != this->genes.end() && it_one != it_two) {
		it_one->swap(*it_two);
// 		auto tmp = *it_one;
// 		*it_one = *it_two;
// 		*it_two = tmp;
	}
}


void Chromosome::mutateGene()
{
	auto it = this->selectGene();
	if (it != this->genes.end()) {
		(*it)->mutate();
	}
}
