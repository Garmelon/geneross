#pragma once

#include <random>
#include <vector>
#include "Chromosome.hpp"
#include "Fitness.hpp"



class Generation
{
public:
	static size_t size;
	static size_t living;
	static Fitness* fitness;
	static std::mt19937_64* re;
	
	Generation();
	
	enum CullType
	{
		CUTOFF,
		ROULETTE
	};
	
	void updateFitness();
	void cull(CullType algorithm=Generation::ROULETTE);
	void reproduce(float crossover=0.5);  // how often a child is the result of two parents,
	                                      // and not simply mutation.
	
	struct Individual
	{
// 		Individual(Chromosome& father, Chromosome& mother) : chromosome(father, mother) {}
		
		Chromosome chromosome;
		unsigned long long fitness = 0;
		bool fitnessEvaluated = false;
		bool alive = true;
	};
	
	std::vector<Individual> individuals;
	
private:
	// all of the culling algorithms can assume:
	//  - sorted array
	//  - alive flag set to true on all individuals
	void cullCutoff();
	void cullRoulette();
	
	// pool of Chromosomes
	// sort pool?
	// min/max/median
	// extract stats
	// calculate fitness
	// cull/selection
	// render full generation? No
};
