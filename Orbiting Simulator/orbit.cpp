#include "orbit.h"
#include "library.h"
#include "config.h"

MassObject::MassObject(sf::Vector2f position, sf::Vector2f velocity, float mass, int radius)
	: pos(position), vel(velocity), mass(mass), radius(radius), color(255, 255, 255), drawObj(radius)
{
	drawObj.setFillColor(color);
}

void MassObject::update()
{
	drawObj.setPosition(sf::Vector2f(pos.x - radius, pos.y - radius)); // Center circle
}

// -------- Orbiting Functions ----------

float gravitationalForce(const sf::Vector2f p1, const float m1, const sf::Vector2f p2, const float m2)
{
	const float rSquared = (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y); // Distance between objects squared

	if (!aproxEqual(rSquared, 0)) // Check for distance being near 0
		return (GRAVITATIONAL_MULTIPLIER * m1 * m2) / rSquared; // The force of gravity
	else return 0.f;
}

sf::Vector2f exertGravity(const sf::Vector2f p1, const float m1, const sf::Vector2f p2, const float m2)
{
	sf::Vector2f forceVect = normalizeVector2f(p2 - p1) * gravitationalForce(p1, m1, p2, m2); // Vector in direction of p2 with magnitude of gravitational force
	return forceVect / m1; // To find acceleration vector divide by mass of the object
}
