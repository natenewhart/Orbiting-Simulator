#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>

// Local Includes
#include "config.h"
#include "library.h"
#include "orbit.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(SCRW, SCRH), "Orbit Simulator");
	window.setFramerateLimit(FPS);

	GameState gameState = GameState::play; // Current state of game

	DeltaTimeCalculator deltaTimeCalc;

	MassObject a(sf::Vector2f(300, 300));
	MassObject b(sf::Vector2f(SCRW / 2, SCRH / 2));

	a.vel.x = 100;
	a.mass  = 300;
	b.mass  = 10000;

	std::vector<MassObject> objectArray{a, b};

	// Object spawning and deleting variables

	sf::Vector2f mousePos(0, 0);          // Live mouse position
	sf::Vector2f rightClickMousePos(0,0); // Last mouse position when right clicked
	sf::Vector2f leftClickMousePos(0, 0); // Last position of mouse when left clicked
	bool deleteObject = false;   // Check if user has tried to delete object using right click
	bool leftClicking = false;   // Is always true while left clicking

	sf::Vector2f spawnVel(0,0); // Velocity of spawned objects
	float spawnMass = 100;      // Mass of spawned objects

	TextDisplay menuText; 

	std::vector<float> x;
	x.reserve(2000);

	while (window.isOpen())
	{
		deleteObject = false;
		const float dt = deltaTimeCalc.getDeltaTime(); // Get delta time

		mousePos.x = sf::Mouse::getPosition(window).x; // Update mouse pos
		mousePos.y = sf::Mouse::getPosition(window).y;

		// ----- Updates -----

		sf::Event event;
		while (window.pollEvent(event)) // Handle events
		{
			if (event.type == sf::Event::Closed) // Close window button
				window.close();

			if (event.type == sf::Event::LostFocus && gameState != GameState::pause) // When window isn't focused game is idled
				gameState = GameState::idle;

			if (event.type == sf::Event::GainedFocus && gameState == GameState::idle) // Stop idleing after refocusing
				gameState = GameState::play;

			switch (gameState)
			{
			case GameState::play:

				if (event.type == sf::Event::KeyPressed) // Keyboard input
				{
					if (event.text.unicode == sf::Keyboard::Escape) // Pause game
					{
						gameState = GameState::pause;
						break;
					}
					if (event.text.unicode == sf::Keyboard::R) // Reset game
					{
						objectArray.clear(); // Delete all objects
					}
					if (event.text.unicode >= sf::Keyboard::Num0 && event.text.unicode <= sf::Keyboard::Num5) // Number keys give mass of spawn object
					{
						spawnMass = powf(10.f, event.text.unicode - sf::Keyboard::Num0);
					}
				}
				if (event.type == sf::Event::MouseWheelMoved) // Mouse wheel edits mass by 5
				{
					spawnMass += event.mouseWheel.delta * 5;
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
				{
					rightClickMousePos = mousePos;
					deleteObject = true;
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) // Save mouse position
				{
					leftClickMousePos = mousePos;
					leftClicking = true;
				}
				if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) // Spawn object on mouse release
				{
					spawnVel = mousePos - leftClickMousePos; // New velocity of object is depended on the dragged section of screen

					objectArray.push_back(MassObject(leftClickMousePos, spawnVel, spawnMass)); // Create object
					spawnVel = sf::Vector2f(0, 0);
					leftClicking = false;
				}
				break;

			case GameState::pause:

				if (event.type == sf::Event::KeyPressed && event.text.unicode == sf::Keyboard::Escape) // Un-pause game
					gameState = GameState::play;

				break;
			}
		}

		// ----- Physics & Updates -----

		for (int i = 0; i < objectArray.size(); i++)
		{
			if (deleteObject)
			{
				if (checkInCircle(rightClickMousePos, objectArray[i].pos, objectArray[i].radius))
				{
					objectArray.erase(objectArray.begin() + i); // Remove object from list
					deleteObject = false;
					continue;
				}
			}
		}

		if (gameState == GameState::play)
		{
			const int physicsSubStepsPerFrame = static_cast<int>(std::roundl(std::fmax(1.0f, dt / PHYSICS_SUB_STEP_DELTATIME))); // Calculate how many sub steps per current frame
			const float dtPhysics = dt / physicsSubStepsPerFrame;                         // Correct delta time value for sub step updates

			std::cout << physicsSubStepsPerFrame << "\n";

			for (int i = 0; i < physicsSubStepsPerFrame; i++) // Updating all physics that needs to be done sub frame
			{
				for (int i = 0; i < objectArray.size(); i++)
				{
					sf::Vector2f acceleration{0,0}; // Acceleration change on object

					if (deleteObject)
					{
						if (checkInCircle(rightClickMousePos, objectArray[i].pos, objectArray[i].radius))
						{
							objectArray.erase(objectArray.begin() + i); // Remove object from list
							deleteObject = false;
							continue;
						}
					}

					for (int j = 0; j < objectArray.size(); j++)
					{
						if (i != j) // If second object isn't self
						{
							// Accumulate acceleration change
							acceleration += exertGravity(objectArray[i].pos, objectArray[i].mass, objectArray[j].pos, objectArray[j].mass);
						}
					}

					// Apply total acceleration to object i
					//objectArray[i].vel += 0.5f * acceleration * dtPhysics;
					//objectArray[i].update(dtPhysics);
					//objectArray[i].vel += 0.5f * acceleration * dtPhysics;

					objectArray[i].pos += objectArray[i].vel * dtPhysics + 0.5f * acceleration * dtPhysics * dtPhysics;
					objectArray[i].vel += acceleration * dtPhysics;
					objectArray[i].update(dtPhysics);
				}
			}
		}

		// ----- Rendering -----

		window.clear(sf::Color(20, 20, 20)); // Clear screen

		for (const MassObject& obj : objectArray) // Draw all objects
		{

			window.draw(obj.drawObj);
		}

		// ---- Menus ----

		drawSpawnMass(window, menuText, spawnMass); // Display mass on the screen
		drawSpawnMagnitude(window, menuText, leftClickMousePos, mousePos, leftClicking);

		switch (gameState)
		{
		case GameState::play:

			if (leftClicking)
			{
				drawObjectArrow(window, leftClickMousePos, mousePos);
			}
			break;

		case GameState::pause:

			drawScreenOverlay(window);
			break;
		}

		window.display();
	}

	return 0;
}