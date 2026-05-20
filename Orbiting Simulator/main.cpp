#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <print>
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

	// Initial objects and conditions
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

	// Game time variables for physics sub stepping
	DeltaTimeCalculator deltaTimeCalc;
	float elapsedGameTime = 0;
	float elapsedPhysicsTime = 0;

	while (window.isOpen())
	{
		deleteObject = false;
		const float dt = std::min(MAX_DELTA_TIME, deltaTimeCalc.getDeltaTime()); // Get delta time

		mousePos.x = sf::Mouse::getPosition(window).x; // Update mouse pos
		mousePos.y = sf::Mouse::getPosition(window).y;

		// ----- Even Updates -----

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

		// ----- Physics & Game Updates -----

		if (gameState == GameState::play)
		{
			if (deleteObject) // Deletion algorithm this deletes all objects touching mouse 
			{
				for (auto it = objectArray.begin(); it != objectArray.end();)
				{
					if (checkInCircle(rightClickMousePos, it->pos, it->radius))
					{
						it = objectArray.erase(it); // erase returns next valid iterator
						continue;
					}
					++it; // Iterate only if no deletion occured
				}
				deleteObject = false;
			}

			// Physics Loop
			elapsedGameTime += dt; // Accumulate total elapsed game time
			while (elapsedGameTime - elapsedPhysicsTime >= PHYSICS_SUB_STEP_DELTATIME) // Updating all physics that needs to be done sub frame
			{
				float dtPhysics = PHYSICS_SUB_STEP_DELTATIME; // Correct delta time value for sub step updates
				// Accumilate all accelerations per physics sub frame
				for (MassObject& currObj : objectArray)
				{
					sf::Vector2f currFrameAcceleration(0, 0);
					for (MassObject& otherObj : objectArray) // Acceleration accumilation loop
					{
						if (&otherObj != &currObj) // Skip exerting gravity on self
						{
							currFrameAcceleration += exertGravity(currObj.pos, currObj.mass, otherObj.pos, otherObj.mass);
						}
					}
					currObj.acc = currFrameAcceleration;
				}

				// Update all objects based on current frames acceleration
				for (MassObject& currObj : objectArray)
				{
					// Update each objects position and velocity
					currObj.pos += currObj.vel * dtPhysics + 0.5f * currObj.acc * dtPhysics * dtPhysics;
					currObj.vel += currObj.acc * dtPhysics;
				}

				elapsedPhysicsTime += dtPhysics;
			}

			// Final Mass Object Position Updates
			for (MassObject& obj : objectArray) // Draw all objects
			{
				obj.update(); // Update position rect sfml
			}
		}

		// ----- Rendering -----

		window.clear(sf::Color(20, 20, 20)); // Clear screen

		for (MassObject& obj : objectArray) // Draw all objects
		{
			window.draw(obj.drawObj);
		}

		// ---- Game State Menus ----

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