#pragma once

#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>



class Gene : public sf::Drawable
{
public:
	static std::minstd_rand* re;
	static sf::Vector2f size;
	
	enum GeneType
	{
		Circle,
		Triangle
	};
	
	static Gene* create(GeneType type);
	static Gene* copy(Gene* gene);
	
	
	GeneType type;
	
	virtual ~Gene();
	
	virtual void randomize() =0;
	virtual void mutate() =0;
};



class GeneCircle : public Gene
{
public:
	static float stddev_position;
	static float stddev_radius;
	static float stddev_color;
	
	GeneType type = Gene::Circle;
	
	virtual ~GeneCircle();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void randomize();
	virtual void mutate();
	
private:
	static sf::CircleShape circle;
	
	sf::Vector2f position;
	float radius;
	sf::Color color;
	
	void mutatePosition();
	void mutateRadius();
	void mutateColor();
	
	float maxRadius();
};



class GeneTriangle : public Gene
{
public:
	GeneType type = Gene::Triangle;
	
	virtual ~GeneTriangle();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void randomize();
	virtual void mutate();
	
private:
	// TODO: vertices
};
