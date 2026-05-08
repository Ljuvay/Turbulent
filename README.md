# Clemson CPSC 4190 Project 5 (Extra Credit)
By Dylan Kauffman

# Description
A real time rigid body dynamics simulation of a mesh suspended by four springs.
The simulation applies linear/angular velocity and handles forces between
rigid bodies and springs. The collision response does NOT work right. I ran out of time

# Building
mkdir build
cd build
cmake ..
make
./app

# ---Simulation Controls---
E       - Summon rigid body box (one at a time | reset to try again)
c		- Cut a random spring!
m/M     - Decrease/Increase Particle Mass (default 0.01)
g/G     - Decrease/Increase Gravity (default -5.0)
s/S     - Decrease/Increase Spring Constant (default 5.0)
d/D     - Decrease/Increase Linear Spring Damping (default 0.3)
a/A     - Decrease/Increase Angular Spring Damping (default 8.0)
q/Q     - Decrease/Increase Collision Restitution (default 0.5)
x/X     - Decrease/Increase Collision Stickiness (default 0.9)
R       - Reset Simulation
P       - Pause Simulation
Esc     - Exit

# ---Camera Controls---
Arrow Keys  - Move Camera Forward/Backward/Left/Right
Space/Ctrl  - Move Camera Up/Down
T           - Reset Camera Rotation
Tab         - Toggle Wireframe Mode

# What I learned

This project helped me to learn more about rigid bodies, and ATTEMPT
to gain more understand of their collisions which I STILL cannot get right
(oops). Overall was fun to be creative though, may try and redo collisions over
the summer, would be fun when not under such a deadline. These weeks leading to
finals have been crazy, so I didn't have much time. 

# Resources Used

Rigid Body Slides
https://jtessen.people.clemson.edu/cpsc6190/html/lectures.html

More Rigid Body
https://ocw.mit.edu/courses/8-09-classical-mechanics-iii-fall-2014/6fe39e8d5ce4ce746ca256dfea665eda_MIT8_09F14_Chapter_2.pdf

Quaternion
https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
