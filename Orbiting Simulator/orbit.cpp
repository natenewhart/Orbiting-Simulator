#include "orbit.h"
#include "library.h"
#include "config.h"

MassObject::MassObject(sf::Vector2f position, sf::Vector2f velocity, float mass, int radius)
	: pos(position), vel(velocity), acc(0.f, 0.f), mass(mass), radius(radius), color(255, 255, 255), drawObj(radius)
{
	drawObj.setFillColor(color);
}

void MassObject::update()
{
	drawObj.setPosition(sf::Vector2f(pos.x - radius, pos.y - radius)); // Center circle
}

// -------- Orbiting Functions ----------

static inline float distanceSquared(sf::Vector2f p1, sf::Vector2f p2)
{
	return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
}

float calcGravFactor(sf::Vector2f p1, sf::Vector2f p2)
{
	return GRAVITATIONAL_CONSTANT / distanceSquared(p1, p2);
}
