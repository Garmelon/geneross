#pragma once

#include <random>
#include <vector>
#include "Chromosome.hpp"
#include "Fitness.hpp"



class Generation
{
public:
	struct Individual
	{
		Chromosome chromosome;
		unsigned long long fitness = 0;
		bool fitnessEvaluated = false;
		bool alive = true;
	};
	
	static size_t size;      // total size of generation
	static size_t living;    // amount of chromosomes to stay alive between generations
	static float crossover;  // probability for a new chromosome to have two parents
	static Fitness* fitness;
	static std::minstd_rand* re;
	
	Generation();
	
	// sorts by
	//  1. alive ones first
	//  2. ones with fitness first
	//  3. ones with lowest fitness first
	void sort();
	
	void advance(unsigned int generations=1);
	void updateFitness();
	void cull();
	void reproduce();
	
	std::vector<Individual> individuals;
};
