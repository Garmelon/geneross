#include "Chromosome.hpp"

#include <algorithm>
#if __GNUC__ < 7 // gcc 7 will support clamp
namespace std {
	template<typename T>
	T clamp(T v, T lo, T hi)
	{
		return std::min(hi, std::max(lo, v));
	}
}
#endif

#include <iostream>
#include <iterator>



// #include <cmath>
sf::Vector2f Chromosome::size(0, 0);
float Chromosome::stddev_position = .1;
float Chromosome::stddev_radius = .1;
float Chromosome::stddev_color = 20;
std::mt19937_64* Chromosome::re;


Chromosome::Chromosome()
{
	// this->genes is already empty
	this->circle.setPointCount(100);
}


Chromosome::Chromosome(Chromosome& father, Chromosome& mother) :
	Chromosome()
{
	std::uniform_int_distribution<> booldist(0, 1);
	
	auto fpair = this->selectSegment(father.genes);
	auto mpair = this->selectSegment(mother.genes);
	
// 	auto cur_it = this->genes.begin();
// 	std::cout << std::distance(cur_it, father.genes.begin()) << std::endl;
// 	std::cout << std::distance(cur_it, mother.genes.begin()) << std::endl;
// 	cur_it = this->genes.insert(cur_it, fpair.second, father.genes.end());
// 	cur_it = this->genes.insert(cur_it, mpair.first, mpair.second);
// 	cur_it = this->genes.insert(cur_it, father.genes.begin(), fpair.first);
	
	if (booldist(*Chromosome::re)) {
		this->genes.insert(this->genes.begin(), fpair.second, father.genes.end());
		this->genes.insert(this->genes.begin(), mpair.first, mpair.second);
		this->genes.insert(this->genes.begin(), father.genes.begin(), fpair.first);
	} else {
		this->genes.insert(this->genes.begin(), mpair.second, mother.genes.end());
		this->genes.insert(this->genes.begin(), fpair.first, fpair.second);
		this->genes.insert(this->genes.begin(), mother.genes.begin(), mpair.first);
	}
}


void Chromosome::mutate()
{
	std::uniform_int_distribution<> booldist(0, 1);
	std::uniform_int_distribution<> choicedist(0, 12);
	while (booldist(*Chromosome::re)) {
		int choice = choicedist(*Chromosome::re);
		
		if (choice < 1) {  // add
// 			std::cout << "Added circle" << std::endl;
			this->genes.push_back(this->randomGene());
		
		} else if (choice < 2) {  // remove
// 			std::cout << "Removed circle" << std::endl;
			auto it = this->selectGene(this->genes);
			if (it != this->genes.end()) {
				this->genes.erase(it);
			}
		
		} else if (choice < 4) {  // swap
// 			std::cout << "Swapped circles" << std::endl;
			auto it_one = this->selectGene(this->genes);
			auto it_two = this->selectGene(this->genes);
			if (it_one != this->genes.end() && it_two != this->genes.end() && it_one != it_two) {
				auto tmp = *it_one;
				*it_one = *it_two;
				*it_two = tmp;
			}
		
		} else {  // mutate
// 			std::cout << "Mutated circle" << std::endl;
			auto it = this->selectGene(this->genes);
			if (it != this->genes.end()) {
				this->mutateGene(*it);
			}
		}
	}
}


void Chromosome::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	this->circle.setPosition(0, 0);
	this->circle.setRadius(Chromosome::size.x + Chromosome::size.y);
	this->circle.setOrigin(Chromosome::size);
	this->circle.setFillColor(sf::Color::White);
	target.draw(this->circle, states);
	for (auto gene : this->genes) {
		this->circle.setPosition(gene.position);
		this->circle.setRadius(gene.radius);
		this->circle.setOrigin(sf::Vector2f(gene.radius, gene.radius));
		this->circle.setFillColor(gene.color);
		target.draw(this->circle, states);
	}
}


size_t Chromosome::length()
{
	return this->genes.size();
}


float Chromosome::maxRadius()
{
	return std::min(Chromosome::size.x, Chromosome::size.y)/2;
}


Chromosome::Gene Chromosome::randomGene()
{
	float max_radius = this->maxRadius();
	std::uniform_real_distribution<> xdist(0, Chromosome::size.x);
	std::uniform_real_distribution<> ydist(0, Chromosome::size.y);
	std::uniform_real_distribution<> rdist(0, sqrt(max_radius));
	std::uniform_int_distribution<> colordist(0, 255);
	
	sf::Vector2f position(xdist(*Chromosome::re), ydist(*Chromosome::re));
	float radius = (pow(rdist(*Chromosome::re), 2));
	sf::Color color(
		colordist(*Chromosome::re),
		colordist(*Chromosome::re),
		colordist(*Chromosome::re),
		150
	);
	
	Chromosome::Gene gene;
	gene.position = position;
	gene.radius = radius;
	gene.color = color;
	
	return gene;
}


void Chromosome::mutateGene(Gene& gene)
{
	std::uniform_int_distribution<> booldist(0, 1);
	float max_radius = this->maxRadius();
	
	if (booldist(*Chromosome::re)) {  // position
		std::normal_distribution<> posdist(0, Chromosome::stddev_position);
		gene.position.x = std::clamp<float>(
			gene.position.x + posdist(*Chromosome::re)*max_radius,
			0,
			Chromosome::size.x
		);
		gene.position.y = std::clamp<float>(
			gene.position.y + posdist(*Chromosome::re)*max_radius,
			0,
			Chromosome::size.y
		);
	}
	
	if (booldist(*Chromosome::re)) {  // radius
		std::normal_distribution<> raddist(0, Chromosome::stddev_radius);
		gene.radius = std::clamp<float>(
			gene.radius + pow(raddist(*Chromosome::re)*sqrt(max_radius), 2),
			0,
			max_radius
		);
	}
	
	if (booldist(*Chromosome::re)) {  // color (all three values at the same time)
		std::normal_distribution<> coldist(0, Chromosome::stddev_color);
		gene.color.r = std::clamp<unsigned int>(gene.color.r + coldist(*Chromosome::re), 0, 255);
		gene.color.g = std::clamp<unsigned int>(gene.color.g + coldist(*Chromosome::re), 0, 255);
		gene.color.b = std::clamp<unsigned int>(gene.color.b + coldist(*Chromosome::re), 0, 255);
		gene.color.a = std::clamp<unsigned int>(gene.color.a + coldist(*Chromosome::re), 0, 255);
	}
}


std::pair<std::vector<Chromosome::Gene>::iterator, std::vector<Chromosome::Gene>::iterator>
Chromosome::selectSegment(std::vector<Chromosome::Gene>& genes)
{
	std::uniform_int_distribution<> randdist(0, genes.size()-1);
	auto first = genes.begin() + randdist(*Chromosome::re);
	auto second = genes.begin() + randdist(*Chromosome::re);
	
	if (first > second) {
		std::swap(first, second);
	}
	
	return std::pair<std::vector<Chromosome::Gene>::iterator,
	                 std::vector<Chromosome::Gene>::iterator>(first, second);
}


std::vector<Chromosome::Gene>::iterator
Chromosome::selectGene(std::vector<Chromosome::Gene>& genes)
{
	if (genes.empty()) {
		return genes.end();
	} else {
		std::uniform_int_distribution<> posdist(0, genes.size()-1);
		return genes.begin() + posdist(*Chromosome::re);
	}
}
