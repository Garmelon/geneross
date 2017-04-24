#include <SFML/Graphics.hpp>
#include <iostream>

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
	
	sf::RenderWindow window(sf::VideoMode(winW, winH), "gross");
	window.setMouseCursorVisible(false); // hide the cursor
	
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
	medcompshdr.setUniform("base", comp);
	medcompshdr.setUniform("curr", base);
	medcompshdr.setUniform("size", sf::Vector2f(240, 240));
	medcompshdr.setUniform("offs", sf::Vector2f(240, 0));
	
	// Set the resolution parameter (the resoltion is divided to make the fire smaller)
// 	shader.setParameter("resolution", sf::Vector2f(winW / 2, winH / 2));
	
	// Use a timer to obtain the time elapsed
// 	sf::Clock clk;
// 	clk.restart(); // start the timer
	
	while (window.isOpen()) {
		// Event handling
		sf::Event event;
		
		while (window.pollEvent(event)) {
			// Exit the app when a key is pressed
			if (event.type == sf::Event::KeyPressed)
				window.close();
		}
		
		// Set the others parameters who need to be updated every frames
// 		shader.setParameter("time", clk.getElapsedTime().asSeconds());
		
// 		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
// 		shader.setParameter("mouse", sf::Vector2f(mousePos.x, mousePos.y - winH));
		
		// Draw the sprites
		window.clear();
		
		window.draw(sprbase);
		
		sprcomp.setPosition(240, 0);
		window.draw(sprcomp);
		
		sprcomp.setPosition(0, 240);
		window.draw(sprcomp, &compshdr);
		
		sprcomp.setPosition(240, 240);
		window.draw(sprcomp, &medcompshdr);
		
		window.display();
	}
	
	return 0;
}

// old main - keep for testing purposes
/*
int main() {
	const float winW = 800;
	const float winH = 600;
	
	sf::RenderWindow window(sf::VideoMode(winW, winH), "gross");
	window.setMouseCursorVisible(false); // hide the cursor
	
	// Create a texture and a sprite for the shader
	sf::Texture tex;
	tex.create(winW, winH);
	sf::Sprite spr(tex);
	
	sf::Shader shader;
	shader.loadFromFile("fire.glsl", sf::Shader::Fragment); // load the shader
	
	if (!shader.isAvailable()) {
		std::cout << "The shader is not available\n";
	}
	
	// Set the resolution parameter (the resoltion is divided to make the fire smaller)
	shader.setParameter("resolution", sf::Vector2f(winW / 2, winH / 2));
	
	// Use a timer to obtain the time elapsed
	sf::Clock clk;
	clk.restart(); // start the timer
	
	while (window.isOpen()) {
		// Event handling
		sf::Event event;
		
		while (window.pollEvent(event)) {
			// Exit the app when a key is pressed
			if (event.type == sf::Event::KeyPressed) 
				window.close();
		}
		
		// Set the others parameters who need to be updated every frames
		shader.setParameter("time", clk.getElapsedTime().asSeconds());
		
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		shader.setParameter("mouse", sf::Vector2f(mousePos.x, mousePos.y - winH));
		
		// Draw the sprite with the shader on it
		window.clear();
		window.draw(spr, &shader);
		window.display();
	}
	
	return 0;
}
*/
