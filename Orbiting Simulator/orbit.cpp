#include "orbit.h"
#include "library.h"
#include "config.h"

MassObject::MassObject(const sf::Vector2f position, const sf::Vector2f velocity, const float mass, const int radius)
	: pos(position), vel(velocity), mass(mass), radius(radius), color(255, 255, 255), drawObj(radius)
{
	drawObj.setFillColor(color);
}

void MassObject::update(const float dt)
{
	//pos += vel * dt;

	drawObj.setPosition(sf::Vector2f(pos.x - radius, pos.y - radius)); // Center circle
}

// -------- Orbiting Functions ----------

float gravitationalForce(const sf::Vector2f p1, const float m1, const sf::Vector2f p2, const float m2)
{
	const float r = std::sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y)); // Distance between objects
	float force = 0.f;

	if (!aproxEqual(r, 0)) // Check for distance being near 0
		force = (GRAVITATIONAL_MULTIPLIER * m1 * m2) / (r * r); // The force of gravity

	return force;
}

sf::Vector2f exertGravity(const sf::Vector2f p1, const float m1, const sf::Vector2f p2, const float m2)
{
	sf::Vector2f forceVect = normalizeVector2f(p2 - p1) * gravitationalForce(p1, m1, p2, m2); // Vector in direction of p2 with magnitude of gravitational force
	return forceVect / m1; // To find acceleration vector divide by mass of the object
}
