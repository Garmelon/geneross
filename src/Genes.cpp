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


void Gene::randomizeColor(sf::Color& color)
{
	std::uniform_int_distribution<> colordist(0, 255);
	color.r = colordist(*Gene::re);
	color.g = colordist(*Gene::re);
	color.b = colordist(*Gene::re);
	color.a = colordist(*Gene::re);
}


void Gene::mutateColor(sf::Color& color, float stddev)
{
	std::normal_distribution<> coldist(0, stddev);
	
	color.r = std::clamp<unsigned int>(color.r + coldist(*Gene::re), 0, 255);
	color.g = std::clamp<unsigned int>(color.g + coldist(*Gene::re), 0, 255);
	color.b = std::clamp<unsigned int>(color.b + coldist(*Gene::re), 0, 255);
	color.a = std::clamp<unsigned int>(color.a + coldist(*Gene::re), 0, 255);
}


void Gene::randomizePosition(sf::Vector2f& position)
{
	std::uniform_real_distribution<> floatdist(0, 1);
	position.x = floatdist(*Gene::re)*Gene::size.x;
	position.y = floatdist(*Gene::re)*Gene::size.y;
}


void Gene::mutatePosition(sf::Vector2f& position, float stddev)
{
	std::normal_distribution<> posdist(0, stddev);
	float min_side = std::min(Gene::size.x, Gene::size.y);
	
	position.x = std::clamp<float>(
		position.x + posdist(*Gene::re)*min_side,
		0,
		Gene::size.x
	);
	
	position.y = std::clamp<float>(
		position.y + posdist(*Gene::re)*min_side,
		0,
		Gene::size.y
	);
}


Gene::~Gene()
{
}



// GeneCircle
sf::CircleShape GeneCircle::circle(0, 36);
float GeneCircle::stddev_position = .1;  // in percent of min side length
float GeneCircle::stddev_radius = .1;    // in percent of max radius
float GeneCircle::stddev_color = 20;     // absolute value


GeneCircle::GeneCircle()
{
	this->type = Gene::Circle;
}


GeneCircle::~GeneCircle()
{
}


void GeneCircle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	GeneCircle::circle.setPosition(this->position);
	GeneCircle::circle.setRadius(this->radius);
	GeneCircle::circle.setOrigin(this->radius, this->radius);
	GeneCircle::circle.setFillColor(this->color);
	target.draw(GeneCircle::circle, states);
}


void GeneCircle::randomize()
{
	Gene::randomizeColor(this->color);
	Gene::randomizePosition(this->position);
	
	std::uniform_real_distribution<> floatdist(0, 1);
	this->radius = floatdist(*Gene::re)*this->maxRadius();
}


void GeneCircle::mutate()
{
	std::uniform_int_distribution<> choicedist(0, 2);
	
	switch (choicedist(*Gene::re)) {
		case 0:
			Gene::mutatePosition(this->position, GeneCircle::stddev_position);
			break;
		case 1:
			this->mutateRadius();
			break;
		case 2:
			Gene::mutateColor(this->color, GeneCircle::stddev_color);
			break;
	}
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


float GeneCircle::maxRadius()
{
// 	return std::min(Gene::size.x, Gene::size.y);
	return std::min(Gene::size.x, Gene::size.y)/2;
}



// GeneTriangle
float GeneTriangle::stddev_position = .1;
float GeneTriangle::stddev_color = 20;
sf::VertexArray GeneTriangle::vertices(sf::Triangles, 3);


GeneTriangle::GeneTriangle()
{
	this->type = Gene::Triangle;
}


GeneTriangle::~GeneTriangle()
{
}


void GeneTriangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	GeneTriangle::vertices[0].position = this->pos1;
	GeneTriangle::vertices[1].position = this->pos2;
	GeneTriangle::vertices[2].position = this->pos3;
	
	GeneTriangle::vertices[0].color = this->color;
	GeneTriangle::vertices[1].color = this->color;
	GeneTriangle::vertices[2].color = this->color;
	
	target.draw(GeneTriangle::vertices, states);
}


void GeneTriangle::randomize()
{
	Gene::randomizePosition(this->pos1);
// 	Gene::randomizePosition(this->pos2);
// 	Gene::randomizePosition(this->pos3);
	this->pos2 = pos1;
	this->pos3 = pos1;
	Gene::mutatePosition(this->pos2, GeneTriangle::stddev_position);
	Gene::mutatePosition(this->pos3, GeneTriangle::stddev_position);
	Gene::randomizeColor(this->color);
}


void GeneTriangle::mutate()
{
	std::uniform_int_distribution<> choicedist(0, 3);
	
	switch (choicedist(*Gene::re)) {
		case 0:
			Gene::mutatePosition(this->pos1, this->stddev_position);
			break;
		case 1:
			Gene::mutatePosition(this->pos2, this->stddev_position);
			break;
		case 2:
			Gene::mutatePosition(this->pos3, this->stddev_position);
			break;
		case 3:
			Gene::mutateColor(this->color, this->stddev_color);
			break;
	}
}
