# TEngine
A custom game engine built from scratch in C++ with OpenGL
(hopefully some Vulkan soon!) Being made to power my game "Turbulent"
as a way to learn graphics and physics programming from the ground up.

## Last Updated
**May 2026**
Blinn-Phong Lighting added, up to 8 lights!

<img width="1417" height="840" alt="image" src="https://github.com/user-attachments/assets/f8cc5eb6-bfcb-44f3-99a7-bbb5eecbb320" />

## About Turbulent
Turbulent is planned to be an ode to some of my favorite games I played
throughout high school and college, Rust and Escape From Tarkov. Although I
love these games they're both time consuming due to the nature of the game
so I wish to set out and create something more accessible with busier schedules
and with features and ideas that I love from these games as well as ones I 
wish were implemented.

## Current State
TEngine features a forward renderer with a structured render queue, separated CPU/GPU mesh management, a modular resource system, and chunk-based terrain generation.

## How I'm Learning
I originally started learning
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
- [x] Basic Blinn-phong lighting
- [ ] LOD and Tessellation shaders
- [ ] more to come...

## Monster Refactors
`[-]` = In Progress | `[x]` = Done | `[ ]` = Planned

- [x] MR-01 - Basic Cleanup from class projects, ~~basic scene abstraction~~
- [x] MR-02 - Proper Input handler cleanup
- [x] MR-03 - Renderer abstraction and renderObjects
- [x] MR-04 - Re-do Resource Managers for cleanliness
- [ ] MR-05 - Clean-up and ImGui integration
- [ ] MR-06 - Safety Checks and exception handling
- [ ] more to come...

## Built With
- C++
- OpenGL
- GLFW
- GLM
- GLAD
- CMake

## Find Me
[YouTube - Dev Logs & Demos](https://www.youtube.com/@Luvayy)

---
*"When every action has a purpose, every action has a result" - Gregg Plitt*
