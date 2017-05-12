#include <ctime>
#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "Chromosome.hpp"
#include "Fitness.hpp"

int main()
{
	const float winW = 480;
	const float winH = 480;
	std::minstd_rand randomEngine;
	randomEngine.seed(time(nullptr));
	
	Chromosome::allowGeneType(Gene::Circle);
	
	Chromosome::re = &randomEngine;
	Gene::re = &randomEngine;
	
	Fitness fitn;
	fitn.loadTarget("tom-face.png");
	fitn.loadShader("compare.glsl");
	
	Gene::size = sf::Vector2f(fitn.target.getSize());
	
	Chromosome a;
	for (int i=0; i<1000; ++i) a.mutate();
	Chromosome b(a);
	for (int i=0; i<10; ++i) b.mutate();
	Chromosome c(a);
	Chromosome d(a, b);
	
	std::cout << a.length() << " | " << b.length() << " | " << c.length() << " | " << d.length() << std::endl;
	
	sf::Sprite ts(fitn.target.getTexture());
	sf::Sprite cs(fitn.chromosome.getTexture());
	
	sf::RenderWindow window(sf::VideoMode(winW, winH), "gross");
// 	window.setMouseCursorVisible(false);
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
			}
		}
		
		// display results
		window.clear();
		fitn.render(a); cs.setPosition(  0,   0); window.draw(cs);
		fitn.render(b); cs.setPosition(240,   0); window.draw(cs);
		fitn.render(c); cs.setPosition(  0, 240); window.draw(cs);
		fitn.render(d); cs.setPosition(240, 240); window.draw(cs);
		window.display();
		
// 		std::cout << "Generation finished: " << ++gencnt << " (winner's length: "
// 		std::cout << genr.individuals[0].chromosome.length() << ")" << std::endl;
	}
}
