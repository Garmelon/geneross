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
	compshdr.setUniform("size", 240, 240);
	
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
		
		// Draw the sprites, one with the shader on it
		window.clear();
		
		window.draw(sprbase);
		
		sprcomp.setPosition(240, 0);
		window.draw(sprcomp);
		
		sprcomp.setPosition(0, 240);
		window.draw(sprcomp, &compshdr);
		
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
