#include "Genes.hpp"

#include <algorithm>

#if __GNUC__ < 7 // gcc 7 will support clamp
namespace std {
	template<typename T>
	T clamp(T v, T lo, T hi)
	{
		return std::min(hi, std::max(lo, v));
	}
}
#endif



// Common to all genes
std::minstd_rand* Gene::re;
sf::Vector2f Gene::size;


Gene* Gene::create(GeneType type)
{
	Gene* gene = nullptr;
	
	switch (type) {
		case Gene::Circle:
			gene = new GeneCircle();
			break;
		case Gene::Triangle:
			gene = new GeneTriangle();
			break;
	}
	
	gene->randomize();
	return gene;
}


Gene* Gene::copy(Gene* gene)
{
	switch (gene->type) {
		case Gene::Circle:
			return new GeneCircle(*static_cast<GeneCircle*>(gene));
		case Gene::Triangle:
			return new GeneTriangle(*static_cast<GeneTriangle*>(gene));
	}
	
	return nullptr;
}


Gene::~Gene()
{
}



// GeneCircle
sf::CircleShape GeneCircle::circle(0, 36);
float GeneCircle::stddev_position = .1;  // in percent of min side length
float GeneCircle::stddev_radius = .1;    // in percent of max radius
float GeneCircle::stddev_color = 20;     // absolute value

GeneCircle::~GeneCircle()
{
}


void GeneCircle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	this->circle.setPosition(this->position);
	this->circle.setRadius(this->radius);
	this->circle.setOrigin(this->radius, this->radius);
	this->circle.setFillColor(this->color);
	target.draw(this->circle, states);
}


void GeneCircle::randomize()
{
	std::uniform_int_distribution<> colordist(0, 255);
	this->color.r = colordist(*Gene::re);
	this->color.g = colordist(*Gene::re);
	this->color.b = colordist(*Gene::re);
	this->color.a = colordist(*Gene::re);
	
	std::uniform_real_distribution<> floatdist(0, 1);
	this->position.x = floatdist(*Gene::re)*Gene::size.x;
	this->position.y = floatdist(*Gene::re)*Gene::size.y;
	this->radius = floatdist(*Gene::re)*this->maxRadius();
}


void GeneCircle::mutate()
{
	std::uniform_int_distribution<> choicedist(0, 2);
	
	switch (choicedist(*Gene::re)) {
		case 0:
			this->mutatePosition();
			break;
		case 1:
			this->mutateRadius();
			break;
		case 2:
			this->mutateColor();
			break;
	}
}


void GeneCircle::mutatePosition()
{
	std::normal_distribution<> posdist(0, GeneCircle::stddev_position);
	float min_side = std::min(Gene::size.x, Gene::size.y);
	
	this->position.x = std::clamp<float>(
		this->position.x + posdist(*Gene::re)*min_side,
		0,
		Gene::size.x
	);
	
	this->position.y = std::clamp<float>(
		this->position.y + posdist(*Gene::re)*min_side,
		0,
		Gene::size.y
	);
}


void GeneCircle::mutateRadius()
{
	std::normal_distribution<> raddist(0, GeneCircle::stddev_radius);
	float max_radius = this->maxRadius();
	
	this->radius = std::clamp<float>(
// 		gene.radius + pow(raddist(*Gene::re)*sqrt(max_radius), 2),
		this->radius + raddist(*Gene::re)*max_radius,
		0,
		max_radius
	);
}


void GeneCircle::mutateColor()
{
	std::normal_distribution<> coldist(0, GeneCircle::stddev_color);
	
	this->color.r = std::clamp<unsigned int>(this->color.r + coldist(*Gene::re), 0, 255);
	this->color.g = std::clamp<unsigned int>(this->color.g + coldist(*Gene::re), 0, 255);
	this->color.b = std::clamp<unsigned int>(this->color.b + coldist(*Gene::re), 0, 255);
	this->color.a = std::clamp<unsigned int>(this->color.a + coldist(*Gene::re), 0, 255);
}


float GeneCircle::maxRadius()
{
// 	return std::min(Gene::size.x, Gene::size.y);
	return std::min(Gene::size.x, Gene::size.y)/2;
}



// GeneTriangle
GeneTriangle::~GeneTriangle()
{
}


void GeneTriangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}


void GeneTriangle::randomize()
{
}


void GeneTriangle::mutate()
{
}
