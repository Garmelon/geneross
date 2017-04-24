#include "Chromosome.hpp"



/*
class Chromosome : public sf::Drawable
{
public:
	Chromosome();  // create empty chromosome
	Chromosome(Chromosome& father, Chromosome& mother);  // cross over two chromosomes
	
	void mutate();  // randomly mutate chromosome's genes
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
private:
	struct Gene
	{
		sf::Vector2f position;
		float size;
		sf::Color color;
	};
	
	sf::CircleShape circle;  // drawing the chromosome, one draw call at a time
	std::vector<Gene> genes;
};
*/

Chromosome::Chromosome()
{
	// this->genes is already empty
};

Chromosome::Chromosome(Chromosome& father, Chromosome& mother) {
	// randomly swap father and mother
	if (/*TODO: random bool*/ false) {
		Chromosome& tmp = father;
		father = mother;
		mother = tmp;
	}
	
	// replace random segment of mother with random segment of father
	
	// TODO: in seperate function:
	// finding random segment:
	// randomly find starting position, then length
	// find starting iterator, then end iterator
	
	// using function from above:
	// find father segment: f_start, f_stop (iterators)
	// find mother segment: m_start, m_stop (iterators)
	
	// RIGHT:
	// append mother until m_start
	// append father from f_start to f_end
	// append mother from m_end
}
