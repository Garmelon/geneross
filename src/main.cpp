#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "Chromosome.hpp"

/*
 * UI Modes:
 * 
 * 1) generation screen
 *    - show all chromosomes at once
 *    - show selection
 *     - reorder things
 *     - kill off things
 *    - ... and breeding
 *     - fill up things
 *    - show fitness for individuals?
 *    - show stats for generation?
 * 
 * 1.5) mating screen
 *      - 
 * 
 * 2) fitness screen
 *    - show original image
 *    - show currently evaluated image
 *    - allows for pausing/singlestepping
 *    - show stats at the bottom
 * 
 * 3) fast-forward screen
 *    - show original image
 *    - show best of previous generation
 *    - show stats at the bottom
 * 
 * logical states:
 * 1) selection
 * 2) crossover
 * 3) mutation
 * 
 * program states:
 * 1) evaluating
 *    - when going slow: fitness screen
 *    - when fast-forwarding: fast-forward screen
 * 2) selecting
 * 3) crossing over
 * 4) mutating
 * Step 2-4
 *  - in generation screen
 *  - invisible?
 */

/*
 * Project structure plan:
 * - Chromosome
 *  - static (class) size variables
 *   - prevents situation where two differently-sized chromosomes mate
 *  - can mate
 *  - can mutate
 *   - add circle
 *   - remove circle
 *   - change circle (stays at same layer)
 *   - maybe change circle color, size, position separately, or all at once
 *   - move circle (between layers)
 *  - can be rendered to texture
 * - Comparator (fitness tester) (maybe just function in Generations)
 *  - can render progress/info to window (simple renderable)
 *   - c.compare() sets variables in instance, draw directly after compare()-ing
 *   - screen update logic in Generations
 *  - initialized with size (or static size variables?)
 *  - compares two textures
 * - Generations
 *  - keeps track of Chromosomes
 *  - applies fitness function to Chromosomes
 *  - applies selection to Chromosomes
 *  - makes Chromosomes mate and mutate
 *   - mutate only freshly mated chromosomes
 *  - displays various states of genetic algorithm
 * - Graph
 *  - displays nice graph for use with Generations
 * - Button?
 */

/*
 * Generations states:
 * - display generation stats (resting) (all other visual states optional)
 * - run modes: step-by-step, visual, fast, off?
 * - toggles for visibility of steps?
 * - display evaluating fitness process
 *  - step-through: both images, image diff, chromosome stats (fitness etc.), generation stats
 *                  autorun mode
 *  - fast: only generation stats, updated every half second or so
 *  - off: nothing rendered at all
 * - display selection process
 *  - show all chromosomes, in tiny RenderTextures?
 *  - show big version on hover-over, including original image?
 *  - sort and move to respective position
 *  - blend out deleted ones (set opacity to 0)
 * - display mating process
 *  - show parents
 *  - show result
 *  - show chromosomes? (string of circles maybe?)
 *  - show parent fitness?
 * 
 * We've come full circle!
 * 
 * Further notes:
 * - controls for stepping <-> runnning with delay
 */

int main() {
	const float winW = 480;
	const float winH = 480;
	std::mt19937_64 randomEngine;
	randomEngine.seed(1);
	
	sf::RenderWindow window(sf::VideoMode(winW, winH), "gross");
	window.setMouseCursorVisible(false); // hide the cursor
	
	Chromosome::size = sf::Vector2f(winW/2, winH/2);
	Chromosome::re = &randomEngine;
	Chromosome father, mother, child, monster;
	sf::View vfather(sf::FloatRect(0, 0, winW/2, winH/2));
	sf::View vmother(sf::FloatRect(0, 0, winW/2, winH/2));
	sf::View vchild(sf::FloatRect(0, 0, winW/2, winH/2));
	sf::View vmonster(sf::FloatRect(0, 0, winW/2, winH/2));
	vfather.setViewport(sf::FloatRect(0, 0, .5, .5));
	vmother.setViewport(sf::FloatRect(.5, 0, .5, .5));
	vchild.setViewport(sf::FloatRect(0, .5, .5, .5));
	vmonster.setViewport(sf::FloatRect(.5, .5, .5, .5));
	
	/*
	// load images
	sf::Texture base;
	sf::Texture comp;
	base.loadFromFile("tom-face.png");
	comp.loadFromFile("tom-face-christmas.png");
	sf::Sprite sprbase(base);
	sf::Sprite sprcomp(comp);
	
	sf::Shader compshdr;
	compshdr.loadFromFile("compare.glsl", sf::Shader::Fragment);
	
	if (!compshdr.isAvailable()) {
		std::cout << "The shader is not available\n";
		return 1;
	}
	
	compshdr.setUniform("base", base);
	compshdr.setUniform("curr", comp);
	compshdr.setUniform("size", sf::Vector2f(240, 240));
	compshdr.setUniform("offs", sf::Vector2f(0, 0));
	
	sf::Shader medcompshdr;
	medcompshdr.loadFromFile("compare_med.glsl", sf::Shader::Fragment);
	if (!medcompshdr.isAvailable()) {
		std::cout << "The medshader is not available\n";
		return 1;
	}
	medcompshdr.setUniform("base", base);
	medcompshdr.setUniform("curr", comp);
	medcompshdr.setUniform("size", sf::Vector2f(240, 240));
	medcompshdr.setUniform("offs", sf::Vector2f(240, 0));
	
	// Set the resolution parameter (the resoltion is divided to make the fire smaller)
// 	shader.setParameter("resolution", sf::Vector2f(winW / 2, winH / 2));
	
	// Use a timer to obtain the time elapsed
// 	sf::Clock clk;
// 	clk.restart(); // start the timer
	*/
	
	
	
	while (window.isOpen()) {
		// Event handling
		sf::Event event;
		
		while (window.pollEvent(event)) {
			// Exit the app when a key is pressed
			if (event.type == sf::Event::KeyPressed) {
				father = Chromosome();
// 				mother = Chromosome();
				for (int i=0; i<1000; ++i) {
					father.mutate();
// 					mother.mutate();
				}
				mother = father;
				for (int i=0; i<20; ++i) {
					mother.mutate();
				}
				child = Chromosome(father, mother);
				monster = child;
				monster.mutate();
				
				std::cout << "----------SIZES----------" << std::endl;
				std::cout << "father size:  " << father.length() << std::endl;
				std::cout << "mother size:  " << mother.length() << std::endl;
				std::cout << "child size:   " << child.length() << std::endl;
				std::cout << "monster size: " << monster.length() << std::endl;
			}
		}
		
		// Set the others parameters who need to be updated every frames
// 		shader.setParameter("time", clk.getElapsedTime().asSeconds());
		
// 		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
// 		shader.setParameter("mouse", sf::Vector2f(mousePos.x, mousePos.y - winH));
		
		// Draw the sprites
		window.clear();
		window.setView(vfather); window.draw(father);
		window.setView(vmother); window.draw(mother);
		window.setView(vchild); window.draw(child);
		window.setView(vmonster); window.draw(monster);
		window.display();
		
		/*
		window.draw(sprbase);
		
		sprcomp.setPosition(240, 0);
		window.draw(sprcomp);
		
		sprcomp.setPosition(0, 240);
		window.draw(sprcomp, &compshdr);
		
		sprcomp.setPosition(240, 240);
		window.draw(sprcomp, &medcompshdr);
		*/
		
	}
	
	return 0;
}
