#include <ctime>
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "Genes.hpp"
#include "Chromosome.hpp"
#include "Fitness.hpp"
#include "Generation.hpp"
#include "Control.hpp"

int main()
{
	std::minstd_rand randomEngine;
	randomEngine.seed(time(nullptr));
	
	Chromosome::re = &randomEngine;
	Gene::re = &randomEngine;
	Generation::re = &randomEngine;
	
// 	Chromosome::allowGeneType(Gene::Circle);
	Chromosome::allowGeneType(Gene::Triangle);
	
	Generation::size = 100;
	Generation::living = 10;
	Generation::crossover = 0.0;
	
	Fitness fitn;
	fitn.loadTarget("firefox.png");
// 	fitn.loadTarget("tom-face.png");
// 	fitn.loadTarget("goldman_sachs.jpg");
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

/*	
// 	Chromosome a;
// 	for (int i=0; i<1000; ++i) a.mutate();
// 	Chromosome b(a);
// 	for (int i=0; i<10; ++i) b.mutate();
// 	Chromosome c(a);
// 	Chromosome d(a, b);
	
// 	std::cout << a.length() << " | " << b.length() << " | " << c.length() << " | " << d.length() << std::endl;
	
	sf::Sprite ts(fitn.target.getTexture());
	sf::Sprite cs(fitn.chromosome.getTexture());
	
	sf::RenderWindow window(sf::VideoMode(winW, winH), "gross");
// 	window.setMouseCursorVisible(false);
	
	int gencnt = 0;
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
// 			if (event.type == sf::Event::KeyPressed) {
// 				std::cout << "Trying to advance generation" << std::endl;
// 				gen.advance();
// 				std::cout << "Advanced generation" << std::endl;
// 			}
		}
		
		
		gen.advance();
		std::cout << "Generation: " << ++gencnt << " (length: " << gen.individuals[0].chromosome.length() << ")" << std::endl;
		
		// display results
		window.clear();
		ts.setPosition(  0,   0); window.draw(ts);
		fitn.render(gen.individuals[0].chromosome); cs.setPosition(240,   0); window.draw(cs);
		fitn.render(gen.individuals[9].chromosome); cs.setPosition(240, 240); window.draw(cs);
// 		fitn.render(a); cs.setPosition(  0,   0); window.draw(cs);
// 		fitn.render(b); cs.setPosition(240,   0); window.draw(cs);
// 		fitn.render(c); cs.setPosition(  0, 240); window.draw(cs);
// 		fitn.render(d); cs.setPosition(240, 240); window.draw(cs);
		window.display();
		
// 		std::cout << "Generation finished: " << ++gencnt << " (winner's length: "
// 		std::cout << genr.individuals[0].chromosome.length() << ")" << std::endl;
	}
}
*/
