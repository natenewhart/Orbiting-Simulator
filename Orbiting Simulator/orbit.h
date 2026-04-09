#include "SFML/Graphics.hpp"
#pragma once

class MassObject // Object effected by gravity and renderable on screen
{
public:
	MassObject(const sf::Vector2f position = sf::Vector2f(0, 0), const sf::Vector2f velocity = sf::Vector2f(0, 0), const float mass = 100, const int radius = 20);

	void update(const float dt); // Update position of object

	// ---- Variables -----
	
	sf::Vector2f pos; // Position center of object
	sf::Vector2f vel;
	
	float mass;
	
	int radius;
	sf::Color color;
	sf::CircleShape drawObj; // Circle object to be drawn
};

// -------- Orbiting Functions ----------

float gravitationalForce(const sf::Vector2f p1, const float m1, const sf::Vector2f p2, const float m2); // Calculate and return gravitational force

sf::Vector2f exertGravity(const sf::Vector2f p1, const float m1, const sf::Vector2f p2, const float m2); // Return acceleration vector to be applied to object 1 given the mass and pos of object 2

