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
	
	// common randomizations/mutations
	static void randomizeColor(sf::Color& color);
	static void mutateColor(sf::Color& color, float stddev);
	static void randomizePosition(sf::Vector2f& position);
	static void mutatePosition(sf::Vector2f& position, float stddev);
	
	
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
	
	GeneCircle();
	virtual ~GeneCircle();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void randomize();
	virtual void mutate();
	
private:
	static sf::CircleShape circle;
	
	sf::Vector2f position;
	float radius;
	sf::Color color;
	
	void mutateRadius();
	float maxRadius();
};



class GeneTriangle : public Gene
{
public:
	static float stddev_position;
	static float stddev_color;
	
	GeneTriangle();
	virtual ~GeneTriangle();
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void randomize();
	virtual void mutate();
	
private:
	static sf::VertexArray vertices;
	
	sf::Vector2f pos1;
	sf::Vector2f pos2;
	sf::Vector2f pos3;
	sf::Color color;
};
