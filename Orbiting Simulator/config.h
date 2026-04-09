#pragma once

// Window

constexpr float SCRW = 1000;
constexpr float SCRH = 900;

// Game

constexpr int FPS = 60;
constexpr float PHYSICS_SUB_STEP_DELTATIME = 1.f / 200.f; // Delta time for physics sup stepping

enum GameState
{
	play  = 0, // Normal state of game
	pause = 1, // Game is rendering but no updates are happening
	idle  = 2  // Idleing, window not in focus
};

// Physics Constants

constexpr float GRAVITATIONAL_MULTIPLIER = 300.f;
constexpr float MAXIMUM_GRAVITATIONAL_FORCE = 10e+6f;