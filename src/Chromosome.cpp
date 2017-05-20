#include "Chromosome.hpp"



std::minstd_rand* Chromosome::re;
std::unordered_set<Gene::GeneType> Chromosome::allowedGeneTypes;


void Chromosome::allowGeneType(Gene::GeneType gt, bool allowed)
{
	if (allowed) {
		Chromosome::allowedGeneTypes.insert(gt);
	} else {
		Chromosome::allowedGeneTypes.erase(gt);
	}
}


bool Chromosome::isGeneTypeAllowed(Gene::GeneType gt)
{
	return Chromosome::allowedGeneTypes.find(gt) != Chromosome::allowedGeneTypes.end();
}


Chromosome::Chromosome()
{
	// empty so you can choose between crossover and creating a new chromosome
}


Chromosome::Chromosome(const Chromosome& father, const Chromosome& mother)
{
	auto split_father = father.selectConstGene();
	auto split_mother = mother.selectConstGene();
	
	// reserve to father's size/capacity, and shrink_to_fit afterwards?
	
	for (auto it=father.genes.begin(); it!=split_father; ++it) {
		this->genes.push_back(std::unique_ptr<Gene>(Gene::copy(it->get())));
	}
	
	for (auto it=split_mother; it!=mother.genes.end(); ++it) {
		this->genes.push_back(std::unique_ptr<Gene>(Gene::copy(it->get())));
	}
}


Chromosome::Chromosome(const Chromosome& other)
{
	// reserve to other's size/capacity?
	
	for (auto& ptr : other.genes) {
		this->genes.push_back(std::unique_ptr<Gene>(Gene::copy(ptr.get())));
	}
}


Chromosome& Chromosome::operator=(const Chromosome& other)
{
	// reserve to other's size/capacity?
	
	this->genes.clear();
	
	if (this != &other) {
		for (auto& ptr : other.genes) {
			this->genes.push_back(std::unique_ptr<Gene>(Gene::copy(ptr.get())));
		}
	}
	
	return *this;
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


std::vector<std::unique_ptr<Gene>>::const_iterator Chromosome::selectConstGene() const
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
	std::uniform_int_distribution<> typedist(0, this->allowedGeneTypes.size()-1);
	
	auto it = this->allowedGeneTypes.begin();
	std::advance(it, typedist(*Gene::re));
	
	const Gene::GeneType type = *it;
	
	Gene* gene = Gene::create(type);
	gene->randomize();
	this->genes.push_back(std::unique_ptr<Gene>(gene));
// 	this->genes.push_back(std::unique_ptr<Gene>(Gene::create(type)));
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
	}
}


void Chromosome::mutateGene()
{
	auto it = this->selectGene();
	if (it != this->genes.end()) {
		(*it)->mutate();
	}
}
