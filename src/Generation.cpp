#include "Generation.hpp"



size_t Generation::size = 500;
size_t Generation::living = Generation::size*0.25;
float Generation::crossover = 0.25;
Fitness* Generation::fitness;
std::minstd_rand* Generation::re;


Generation::Generation()
{
	this->individuals.resize(Generation::size, {Chromosome(), 0, false, false});
}


void Generation::sort()
{
	std::sort(
		this->individuals.begin(),
		this->individuals.end(),
		[](Generation::Individual& a, Generation::Individual& b) -> bool {
			if (a.alive && b.alive) {
				if (a.fitnessEvaluated && b.fitnessEvaluated) {
					return a.fitness < b.fitness;
				} else {
					return a.fitnessEvaluated;
				}
			} else {
				return a.alive;
			}
		}
	);
}


void Generation::advance(unsigned int generations)
{
	for (unsigned int i=0; i<generations; ++i) {
// #define debugcount {int alive=0,fitness=0;for(auto&i:individuals){i.alive?++alive:1;i.fitnessEvaluated?++fitness:1;}std::cout<<" -> alive: "<<alive<<", fitness: "<<fitness<<std::endl;}
// 		std::cout << "-- updating fitness" << std::endl;
// 		debugcount
		this->updateFitness();
// 		std::cout << "-- sorting" << std::endl;
// 		debugcount
		this->sort();
// 		std::cout << "-- culling" << std::endl;
// 		debugcount
		this->cull();
// 		std::cout << "-- sorting" << std::endl;
// 		debugcount
		this->sort();
// 		std::cout << "-- reproducing" << std::endl;
// 		debugcount
		this->reproduce();
// #undef debugcount
	}
}


void Generation::updateFitness()
{
	for (auto& individual : this->individuals) {
		if (!individual.fitnessEvaluated) {
			individual.fitness = Generation::fitness->of(individual.chromosome);
			individual.fitnessEvaluated = true;
		}
	}
}


void Generation::cull()
{
	auto it = this->individuals.begin() + Generation::living;
	for (; it!=this->individuals.end(); ++it) {
		it->alive = false;
	}
}


void Generation::reproduce()
{
	// reproduce according to parameter(s)
	std::uniform_int_distribution<> alivedist(0, Generation::living-1);
	std::uniform_real_distribution<> crossoverdist(0, 1);
	
	for (auto& individual : this->individuals) {
		if (individual.alive) continue;
		
		auto father = this->individuals.begin() + alivedist(*Generation::re);
		
		if (crossoverdist(*Generation::re) < crossover) {
			auto mother = this->individuals.begin() + alivedist(*Generation::re);
			individual.chromosome = Chromosome(father->chromosome, mother->chromosome);
		} else {
			individual.chromosome = father->chromosome;
		}
		
		individual.alive = true;
		individual.fitness = 0;  // technically unnecessary, but who cares
		individual.fitnessEvaluated = false;
		
		individual.chromosome.mutate();
	}
}


/*
Generation::Generation()
{
	this->individuals.resize(Generation::size, {Chromosome(), 0, false, true});
}


void Generation::updateFitness()
{
	for (auto& individual : this->individuals) {
		if (!individual.fitnessEvaluated) {
			individual.fitness = Generation::fitness->of(individual.chromosome);
			individual.fitnessEvaluated = true;
		}
	}
}


void Generation::cull(CullType algorithm)
{
	// sort by fitness
	std::sort(
		this->individuals.begin(),
		this->individuals.end(),
		[](Generation::Individual& a, Generation::Individual& b) -> bool {
			return a.fitness < b.fitness;
		}
	);
	
	// apply correct culling algorithm
	switch (algorithm) {
		case Generation::CUTOFF: this->cullCutoff(); break;
		case Generation::ROULETTE: this->cullRoulette(); break;
	}
}


void Generation::reproduce(float crossover)
{
	// sort by fitness/available
	std::sort(
		this->individuals.begin(),
		this->individuals.end(),
		[](Generation::Individual& a, Generation::Individual& b) -> bool {
			if (a.alive && b.alive) {
				return a.fitness < b.fitness;
			} else {
				return a.alive;
			}
		}
	);
	
	// reproduce according to parameter(s)
	std::uniform_int_distribution<> alivedist(0, Generation::living-1);
	std::uniform_real_distribution<> crossoverdist(0, 1);
	auto it = this->individuals.begin() + Generation::living;
	for (; it!=this->individuals.end(); ++it) {
		auto father = this->individuals.begin() + alivedist(*Generation::re);
		
		if (crossoverdist(*Generation::re) < crossover) {
			auto mother = this->individuals.begin() + alivedist(*Generation::re);
			it->chromosome = Chromosome(father->chromosome, mother->chromosome);
		} else {
			it->chromosome = father->chromosome;
		}
		
		it->alive = true;
		it->fitness = 0;  // technically unnecessary, but who cares
		it->fitnessEvaluated = false;
		
		it->chromosome.mutate();
	}
}


void Generation::cullCutoff()
{
	auto it = this->individuals.begin() + Generation::living;
	for (; it!=this->individuals.end(); ++it) {
		it->alive = false;
	}
}


void Generation::cullRoulette()
{
	// TODO: implement this
}
*/
