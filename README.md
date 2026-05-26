# TEngine
A custom game engine built from scratch in C++ with OpenGL
(hopefully some Vulkan soon!) Being made to power my game "Turbulent"
as a way to learn graphics and physics programming from the ground up.

## Last Updated
**May 2026** - Crude terrain generation with chunk system and height-based color. Major refactoring incoming...
<img width="771" height="471" alt="image" src="https://github.com/user-attachments/assets/d01d3115-1cc1-4d15-8ea9-9c0c05edfb3c" />

## About Turbulent
Turbulent is planned to be an ode to some of my favorite games I played
throughout high school and college, Rust and Escape From Tarkov. Although I
love these games they're both time consuming due to the nature of the game
so I wish to set out and create something more accessible with busier schedules
and with features and ideas that I love from these games as well as ones I 
wish were implemented.

## Current State
VERY early WIP. The base of this project is the custom renderer I made
for my "Physical Modeling and Animation" at Clemson. Currently working
on scene abstraction and some core boilerplate and clean-up based on issues
I faced while using my renderer during class.

## How I'm Learning
Honestly, my structure is a bit scrambled, but I originally started learning
through learnopengl.com, but got too ambitious for reading through the textbook.
Currently I am reading through Tomas Möller's "Real-Time Rendering", and just
getting a basic mental roadmap of various topics and implementing them into my
engine as I see fit when I want a new feature for Turbulent. I also do lots of
research by watching YouTube videos or reading articles breaking down specific
effects I find fascinating or chatting with more experienced developers online
such as those in the "Graphics Programming" Discord.

## Goals
`[-]` = In Progress | `[x]` = Done | `[ ]` = Planned

- [x] Scene Management
- [x] Debug Grid
- [x] Basic Heightmap Terrain
- [ ] Basic Blinn-phong lighting
- [ ] LOD and Tessellation shaders
- [ ] more to come...

## Monster Refactors
`[-]` = In Progress | `[x]` = Done | `[ ]` = Planned

- [-] MR-01 - Basic Cleanup from class projects, ~~basic scene abstraction~~
- [ ] MR-02 - Proper Input handler cleanup
- [ ] MR-03 - Safety Checks and exception handling
- [ ] more to come...

## Built With
- C++
- OpenGL
- GLFW
- GLM
- GLAD
- CMake

## Known Issues / Limitations
Because my current set up was built for a physical modeling class, most renderer
and vertex actions are handled via the CPU and the renderer has very limited power
and expandability. Lots of work will need to be done on the renderer to handle new
rendering methods, so it's a big work in progress.

## Find Me
[YouTube - Dev Logs & Demos](https://www.youtube.com/@Luvayy)

---
*"When every action has a purpose, every action has a result" - Gregg Plitt*
