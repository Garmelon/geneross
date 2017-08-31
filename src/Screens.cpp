#include "Screens.hpp"

#include "Control.hpp"

#include <iostream>

// Screen base class
Screen::Screen(Control* controlptr)
{
	this->control = controlptr;
}


Screen::~Screen()
{
}



// ScreenSetup screen

ScreenSetup::ScreenSetup(Control* controlptr) :
	Screen(controlptr),
	offset(20, 20),
	distance(30),
	items(5)
{
	this->selection = 0;
	
	this->text.setCharacterSize(20);
	this->text.setFillColor(sf::Color::Black);
	
	this->rect.setFillColor(sf::Color::Yellow);
}


void ScreenSetup::enter()
{
	this->rect.setSize(
		sf::Vector2f(
			this->control->window.getSize().x,
			30
		)
	);
	
	this->selectItem(0);
}


void ScreenSetup::event(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		this->keyPressed(event);
	}
}


void ScreenSetup::update(sf::Time dt)
{
	if (this->warnTimer > sf::Time::Zero) {
		this->warnTimer -= dt;
	}
}


void ScreenSetup::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	this->drawMenu(target, states);
	
	if (this->warnTimer > sf::Time::Zero) {
		this->control->drawBar(target, states);
	}
}


void ScreenSetup::exit()
{
}


void ScreenSetup::setFont(const sf::Font& font)
{
	this->text.setFont(font);
}


void ScreenSetup::keyPressed(sf::Event& event)
{
	switch (event.key.code) {
		// close window, stop program
		case sf::Keyboard::Q:
			this->control->close();
			return;
			
		// move selection up in menu
		case sf::Keyboard::Up:
			if (event.key.shift) this->selectItem(0);
			else this->selectItem(this->selection - 1);
			break;
			
		// move selection down in menu
		case sf::Keyboard::Down:
			if (event.key.shift) this->selectItem(this->items - 1);
			else this->selectItem(this->selection + 1);
			break;
			
		// select current item and perform action
		case sf::Keyboard::Return:
			this->executeItem(this->selection);
			
		default: break;
	}
}


void ScreenSetup::selectItem(int position)
{
	// kinda ugly, just to get the same % behaviour as in python
	this->selection = ((position%this->items) + this->items)%this->items;
	this->rect.setPosition(
		sf::Vector2f(
			0,
			this->offset.y + this->distance*this->selection
		)
	);
}


void ScreenSetup::executeItem(int position)
{
	switch (position) {
		case 0: Chromosome::toggleGeneType(Gene::Circle); break;
		case 1: Chromosome::toggleGeneType(Gene::Triangle); break;
		case 2: Chromosome::toggleGeneType(Gene::Rectangle); break;
		case 3: break; // spacing placeholder
		case 4:
			if (Chromosome::isAnyGeneTypeAllowed()) {
				this->control->switchScreen(this->control->screenGenerations);
			} else {
				this->control->setText("Allow at least one gene type to continue.");
				this->warnTimer = sf::seconds(3);
			}
			break;
	}
}


void ScreenSetup::drawMenu(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->rect);
	
	sf::Vector2f position = this->offset;
	
	this->drawMenuItem(
		target, states, position, "Circle genes:   ",
		Chromosome::isGeneTypeAllowed(Gene::Circle)?"x":" "
	);
	position.y += this->distance;
	
	this->drawMenuItem(
		target, states, position, "Triangle genes: ",
		Chromosome::isGeneTypeAllowed(Gene::Triangle)?"x":" "
	);
	position.y += this->distance;
	
	this->drawMenuItem(
		target, states, position, "Rectangle genes:",
		Chromosome::isGeneTypeAllowed(Gene::Rectangle)?"x":" "
	);
	position.y += 2*this->distance;
	
	this->drawMenuItem(
		target, states, position, "Continue"
	);
}


void ScreenSetup::drawMenuItem(sf::RenderTarget& target, sf::RenderStates states,
	sf::Vector2f position, std::string name, std::string choice) const
{
	if (choice != "") {
		name += " [";
		name += choice;
		name += "]";
	}
	
	this->text.setString(name);
	this->text.setPosition(position);
	target.draw(text, states);
}



// ScreenGenerations screen

ScreenGenerations::ScreenGenerations(Control* controlptr) :
	Screen(controlptr)
{
	this->targetSprite.setTexture(Control::fitness->target.getTexture());
	this->chromoSprite.setTexture(Control::fitness->chromosome.getTexture());
	
	this->currentGeneration = 0;
	
	this->snapshotMode = true;  // fullscreen
	this->automaticSnapshots = false;
	this->manualSnapshotCount = 0;
	this->automaticSnapshotCount = 0;
	
	this->lastBestFitness = -1;
	std::cout << std::to_string(lastBestFitness) << std::endl;
}


void ScreenGenerations::enter()
{
	this->paused = true;
	this->updateStatus();
	this->updateSpriteSize();
}


void ScreenGenerations::event(sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		this->keyPressed(event);
	}
}


void ScreenGenerations::update(sf::Time dt)
{
	if (!this->paused) {
		Control::generation->advance();
		++this->currentGeneration;
		this->updateStatus();
		
		if (this->automaticSnapshots && Control::generation->individuals[0].fitnessEvaluated) {
			unsigned long long fitness = Control::generation->individuals[0].fitness;
			if (fitness < this->lastBestFitness) {
				std::cout << "Fitness decreased to " << std::to_string(fitness)
					<< " -> taking snapshot!" << std::endl;
				this->takeSnapshot(false);
				this->lastBestFitness = fitness;
			}
		}
	}
}


void ScreenGenerations::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Control::fitness->render(Control::generation->individuals[0].chromosome);
	target.draw(this->targetSprite, states);
	target.draw(this->chromoSprite, states);
	this->control->drawBar(target, states);
}


void ScreenGenerations::exit()
{
}


void ScreenGenerations::keyPressed(sf::Event& event)
{
	switch (event.key.code) {
		// close window, stop program
		case sf::Keyboard::Q:
			this->control->close();
			return;
			
		case sf::Keyboard::P:
			this->paused = !this->paused;
			this->updateStatus();
			break;
			
		case sf::Keyboard::M:
			this->snapshotMode = !this->snapshotMode;
			this->updateStatus();
			break;
			
		case sf::Keyboard::S:
			this->takeSnapshot(true);
			break;
			
		case sf::Keyboard::A:
			this->automaticSnapshots = !this->automaticSnapshots;
			this->updateStatus();
			break;
			
		default: break;
	}
}


void ScreenGenerations::updateSpriteSize()
{
	sf::Vector2f size = sf::Vector2f(Control::fitness->target.getSize());
	sf::Vector2f windowSize = this->control->getSizeWithBar();
	
	float scale;
	sf::Vector2f spritePos;
	if (2*size.x/size.y > windowSize.x/windowSize.y) {
		std::cout << "wider" << std::endl;
		// the images are wider than the window
		// display is calculated based on width
		scale = windowSize.x/(2*size.x);
		spritePos.x = 0;
		spritePos.y = (windowSize.y - scale*size.y)/2;
	} else {
		std::cout << "higher" << std::endl;
		// the images are higher than the window
		// display is calculated based on height
		scale = windowSize.y/size.y;
		spritePos.x = (windowSize.x - (2*scale*size.x))/2;
		spritePos.y = 0;
	}
	
	this->targetSprite.setScale(sf::Vector2f(scale, scale));
	this->chromoSprite.setScale(sf::Vector2f(scale, scale));

	this->targetSprite.setPosition(spritePos);
	spritePos.x += scale*size.x;
	this->chromoSprite.setPosition(spritePos);
}


void ScreenGenerations::updateStatus()
{
	std::string text = "Generation: ";
	text += std::to_string(this->currentGeneration);
	text += " (Genes: ";
	text += std::to_string(Control::generation->individuals[0].chromosome.length());
	text += ")";
	this->control->setLeftText(text);
	
	text = "[";
	text += (this->paused)?"P":"-";
	text += (this->automaticSnapshots)?"A":"-";
	text += (this->snapshotMode)?"f":"c";
	text += "]";
	this->control->setRightText(text);
}


void ScreenGenerations::takeSnapshot(bool manual)
{
	// name for image file - count
	std::string filename;
	std::string count;
	if (manual) {
		filename = "manual_";
		count = std::to_string(this->manualSnapshotCount);
		++this->manualSnapshotCount;
	} else {
		filename = "automatic_";
		count = std::to_string(this->automaticSnapshotCount);
		++this->automaticSnapshotCount;
	}
	if (count.length() < 5) {
		filename += std::string(5 - count.length(), '0');
	}
	filename += count;
	
	// name for image file - generation and chromosome size
	filename += "_gen_";
	filename += std::to_string(this->currentGeneration);
	filename += "_chr_";
	filename += std::to_string(Control::generation->individuals[0].chromosome.length());
	filename += ".png";
	
	// actually save snapshot
	if (this->snapshotMode) {
		// fullscreen
		sf::Texture tex;
		sf::Vector2u size = this->control->window.getSize();
		tex.create(size.x, size.y);
		tex.update(this->control->window); // TODO: redraw screen before this!?
		tex.copyToImage().saveToFile(filename);
	} else {
		// chromosome only
		Control::fitness->render(Control::generation->individuals[0].chromosome);
		Control::fitness->saveChromosome(filename);
	}
}
