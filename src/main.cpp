#include <ctime>
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "Genes.hpp"
#include "Chromosome.hpp"
#include "Fitness.hpp"
#include "Generation.hpp"
#include "Control.hpp"

int main(int argc, char** argv)
{
	std::string filename;
	if (argc != 2) {
		std::cout << "u dun goofed" << std::endl;
		return 1;
	} else {
		filename = argv[1];
		std::cout << "Using image: " << filename << std::endl;
	}
	
	std::minstd_rand randomEngine;
	randomEngine.seed(time(nullptr));
	
	Chromosome::re = &randomEngine;
	Gene::re = &randomEngine;
	Generation::re = &randomEngine;
	
// 	Chromosome::allowGeneType(Gene::Circle);
// 	Chromosome::allowGeneType(Gene::Triangle);
	
	Generation::size = 100;
	Generation::living = 10;
	Generation::crossover = 0.0;
	
	Fitness fitn;
// 	fitn.loadTarget("firefox.png");
	fitn.loadTarget(filename);
	fitn.loadShader("compare.glsl");
	
	Gene::size = sf::Vector2f(fitn.target.getSize());
	Generation::fitness = &fitn;
	Generation gen;
	
	Control::fitness = Generation::fitness;
	Control::generation = &gen;
	Control con(800, 600, "gross");  // window size
	
	sf::Font font;
	font.loadFromFile("FreeMonoBold.ttf");
	con.setFont(font);
	
	con.interactive();
}
