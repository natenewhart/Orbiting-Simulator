#include <chrono>

#pragma once

// ----- Classes ------

class DeltaTimeCalculator
{
public:
	DeltaTimeCalculator();

	float getDeltaTime(); // Return seconds between last frame and current

private:
	std::chrono::high_resolution_clock::time_point prevTime; // The last time measurement
};

class TextDisplay // Simple text rendering class
{
public:
	TextDisplay();

	void draw(sf::RenderWindow& window, const std::string& text, const sf::Vector2f pos, const int fontSize = 24);

private:
    sf::Font font;
    sf::Text text;
};

// ---- Normal Functions ----

bool aproxEqual(const float a, const float b, const float epsilon = 1e-5f);
sf::Vector2f normalizeVector2f(const sf::Vector2f vect);
bool checkInCircle(const sf::Vector2f pos, const sf::Vector2f circlePos, const int radius); // Check if coordinate point falls inside a circle

// ---- Render Functions ----

void drawScreenOverlay(sf::RenderWindow& window); // Render black semi-transparent overlay

void drawObjectArrow(sf::RenderWindow& window, const sf::Vector2f start, const sf::Vector2f end); // Draw representation of velocity on newly spawned objects

void drawSpawnMass(sf::RenderWindow& window, TextDisplay& textDisplay, const float spawnMass);

void drawSpawnMagnitude(sf::RenderWindow& window, TextDisplay& textDisplay, const sf::Vector2f p1, const sf::Vector2f p2, const bool leftClicking);

//void updatePositionAndVelocityConstAcceleration(sf::Vector2f pos, sf::Vector2f vel, sf::Vector);