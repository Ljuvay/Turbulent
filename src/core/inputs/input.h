#ifndef INPUT_H
#define INPUT_H

namespace Input {
	void init(void* glfwWindow);
	void Update();
	void clearKeyStates();
	bool KeyPressed(unsigned int keyCode);
	bool KeyDown(unsigned int keyCode);
}


#endif // !INPUT_H
