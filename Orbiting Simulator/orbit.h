#include "SFML/Graphics.hpp"
#pragma once

class MassObject // Object effected by gravity and renderable on screen
{
public:
	MassObject(sf::Vector2f position = sf::Vector2f(0, 0), sf::Vector2f velocity = sf::Vector2f(0, 0), float mass = 100, int radius = 20);

	void update(); // Update position of object

	// ---- Variables -----
	
	sf::Vector2f pos; // Position center of object
	sf::Vector2f vel;
	sf::Vector2f acc; // Acceleration
	
	float mass;
	
	int radius;
	sf::Color color;
	sf::CircleShape drawObj; // Circle object to be drawn
};

// -------- Orbiting Functions ----------

float calcGravFactor(sf::Vector2f p1, sf::Vector2f p2); // Calculate G / r^2. Multiply by any mass to get the gravitational field strength
