/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: InputManager.cpp
Purpose: Defines the FrameRateController class methods
Language: C++, MS C++ Compiler
Platform: MS Visual Studio 2017, Windows 10
Project: CS529_shashwatpandey_milestone1
Author: Shashwat Pandey, shashwat.pandey, ID: 60003718
Creation date: 18 October 2018
- End Header --------------------------------------------------------*/

#include "InputManager.h"
#include "GLFW/glfw3.h"

extern GLFWwindow* gpWindow;

InputManager::~InputManager()
{
}

void InputManager::Update()
{
	UpdateStatesForKeyboardKeys();
}

void InputManager::UpdateStatesForKeyboardKeys() {
	int state = 0;
	for (int i = 0; i < NUM; ++i) {
		state = glfwGetKey(gpWindow, keyCodes[i]);

		if (state == GLFW_PRESS) {
			// check for triggered
			if (keyStateReleased[static_cast<KEYBOARD_KEYS>(i)]) {
				keyStateTriggered[static_cast<KEYBOARD_KEYS>(i)] = true;
			}
			else {
				keyStateTriggered[static_cast<KEYBOARD_KEYS>(i)] = false;
			}

			keyStatePressed[static_cast<KEYBOARD_KEYS>(i)] = true;
			keyStateReleased[static_cast<KEYBOARD_KEYS>(i)] = false;

		}
		else if (state == GLFW_RELEASE) {
			keyStatePressed[static_cast<KEYBOARD_KEYS>(i)] = false;
			keyStateTriggered[static_cast<KEYBOARD_KEYS>(i)] = false;
			keyStateReleased[static_cast<KEYBOARD_KEYS>(i)] = true;
		}
	}
}

bool InputManager::isPressed(unsigned int KeyScanCode) {
	return keyStatePressed[static_cast<KEYBOARD_KEYS>(KeyScanCode)];
}


bool InputManager::isTriggered(unsigned int KeyScanCode) {
	return keyStateTriggered[static_cast<KEYBOARD_KEYS>(KeyScanCode)];
}

bool InputManager::isReleased(unsigned int KeyScanCode) {
	return keyStateReleased[static_cast<KEYBOARD_KEYS>(KeyScanCode)];
}

InputManager::InputManager()
{
	// set all keycodes for keys used in game
	keyCodes[0] = GLFW_KEY_UP;
	keyCodes[1] = GLFW_KEY_DOWN;
	keyCodes[2] = GLFW_KEY_LEFT;
	keyCodes[3] = GLFW_KEY_RIGHT;
	keyCodes[4] = GLFW_KEY_SPACE;
	keyCodes[5] = GLFW_KEY_Z;
	keyCodes[6] = GLFW_KEY_X;
	keyCodes[7] = GLFW_KEY_N;
	keyCodes[8] = GLFW_KEY_D;
	keyCodes[9] = GLFW_KEY_ESCAPE;

	for (int i = 0; i < NUM; ++i) {
		keyStatePressed[static_cast<KEYBOARD_KEYS>(i)] = false;
		keyStateTriggered[static_cast<KEYBOARD_KEYS>(i)] = false;
		keyStateReleased[static_cast<KEYBOARD_KEYS>(i)] = true;
	}
}