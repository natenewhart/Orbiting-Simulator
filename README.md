# Orbiting Simulator

This is a 2D gravity simulation built from scratch using C++ and the SFML library, where point masses attract each other and you can spawn new bodies to watch them orbit.

## Features

- Real-time N-body gravity simulation between point masses
- Physics runs on a fixed sub-step timer, independent of render framerate
- Spawn many masses with any initial velocity and mass

## Getting Started

### Prerequisites

Make sure you have Visual Studio 2022 or later installed, along with the C++ packages.

### Installation / Building

1. Clone the project into Visual Studio and build to compile, the project file is already set up with all links and dependencies.

> Note: The SFML library comes pre-installed in the `\Dependencies` folder.

2. Make sure the `.ttf` font files exist relative to the working directory.
3. Build and run (`main.cpp` is the entry point).

## Controls

- **Left click and hold**, then drag and release to spawn a new mass. The direction and length of the drag sets its initial velocity
- **Right click** an object to delete it
- **Number keys 0-5** to set the mass of the next spawned object (powers of 10)
- **Mouse wheel** to fine-tune the spawn mass by increments of 5
- **R** to clear all objects and reset the simulation
- **Esc** to pause and unpause the game

## Gameplay
<img width="1002" height="932" alt="Screenshot 2026-08-23 193233" src="https://github.com/user-attachments/assets/58655572-75f7-4841-bf55-5c620566caad" />
<img width="1002" height="932" alt="Screenshot 2026-08-23 193226" src="https://github.com/user-attachments/assets/7b08d81b-327b-45c3-b55d-a7a535eae9dc" />
<img width="1002" height="932" alt="Screenshot 2026-08-23 193246" src="https://github.com/user-attachments/assets/bf7a3069-a489-41f8-9ced-77993434c11d" />


## License

COMING SOON...
