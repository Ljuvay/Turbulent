# TEngine
A custom game engine built from scratch in C++ and OpenGL to explore real-time rendering, engine architecture, and graphics programming.

## Last Updated
**May 2026**

### Recent Changes
- Reworked Resource Manager into a modular system
- Added structured Forward Rendering pipeline
- Implemented Blinn-Phong lighting (up to 8 dynamic lights)
- Added ImGui integration for runtime debugging and scene controls
- Improved renderer abstraction and render queue organization

<img width="1850" height="952" alt="image" src="https://github.com/user-attachments/assets/73a38f8e-5318-4930-a939-4055e74ac62a" />

## Features
- Forward Rendering Pipeline
- Scene Management System
- Resource Management System
- Render Queue Architecture
- CPU/GPU Mesh Separation
- ImGui Debug Tools
- Blinn-Phong Lighting (up to 8 dynamic lights)
- Chunk-Based Terrain Generation
- Input Abstraction

## About Turbulent
Turbulent is planned to be an ode to some of my favorite games I played
throughout high school and college, Rust and Escape From Tarkov. Although I
love these games they're both time consuming due to the nature of the game
so I wish to set out and create something more accessible with busier schedules
and with features and ideas that I love from these games as well as ones I 
wish were implemented.

## Engine Architecture

The current rendering pipeline follows a forward rendering approach.

Scene objects are submitted to a render queue before being processed by the renderer, and Meshes are split into CPU and GPU representations

Current goals:
- Clear separation of engine systems
- Maintainable resource ownership
- Extensible rendering pipeline
- Easy graphics experimentation

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
- [x] Basic Blinn-Phong Lighting
- [ ] Material System
- [ ] Shadow Mapping
- [ ] Frustum Culling
- [ ] Terrain LOD
- [ ] Deferred Rendering
- [ ] Vulkan Renderer
- [ ] more to come...

## Major Refactors
`[-]` = In Progress | `[x]` = Done | `[ ]` = Planned

- [x] MR-01 - Basic Cleanup from class projects, ~~basic scene abstraction~~
- [x] MR-02 - Proper Input handler cleanup
- [x] MR-03 - Renderer abstraction and renderObjects
- [x] MR-04 - Re-do Resource Managers for cleanliness
- [x] MR-05 - ImGui Integration
- [-] MR-06 - Engine Cleanup and Editor Tooling
- [ ] MR-07 - Safety Checks and exception handling
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
