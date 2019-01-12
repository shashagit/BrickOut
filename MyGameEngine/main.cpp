#include <iostream>

#include "stdio.h"
#include "Managers/RenderManager.h"
#include "Managers/InputManager.h"
#include "Managers/FrameRateController.h"
#include "Managers/ResourceManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/CollisionManager.h"
#include "Managers/EventManager.h"
#include "Managers/LevelManager.h"
#include "windows.h"


#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "GameObject/ObjectFactory.h"

#include "Components/Component.h"
#include "Components/Controller.h"

#include <GLFW/glfw3.h> // should be after glew

#define MAXFRAMERATE 60 // 0 is unlocked
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

// declaring global variables
RenderManager* gpRenderManager = nullptr;
InputManager* gpInputManager = nullptr;
GLFWwindow* gpWindow = nullptr;
ResourceManager* gpResourceManager = nullptr;
GameObjectManager* gpGoManager = nullptr;
ObjectFactory* gpObjectFactory = nullptr;
PhysicsManager* gpPhysicsManager = nullptr;
CollisionManager* gpCollisionManager = nullptr;
FrameRateController* gpFrameRateController = nullptr;
EventManager* gpEventManager = nullptr;
LevelManager* gpLevelManager = nullptr;

#pragma comment(lib, "legacy_stdio_definitions.lib")

int main() {
	// Debug Console
	if (AllocConsole())
	{
		FILE* file;

		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);

		SetConsoleTitle("DEBUG");
	}

	//----------------------------------------------
	// OpenGL Boilerplate
	//----------------------------------------------
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	gpWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ARK4NOID", nullptr, nullptr);
	glfwMakeContextCurrent(gpWindow);


	//----------------------------------------------
	// Managers
	//----------------------------------------------
	gpRenderManager = new RenderManager();
	gpResourceManager = new ResourceManager();
	gpInputManager = new InputManager();
	gpFrameRateController = new FrameRateController(MAXFRAMERATE);
	gpGoManager = new GameObjectManager();
	gpObjectFactory = new ObjectFactory();
	gpPhysicsManager = new PhysicsManager();
	gpCollisionManager = new CollisionManager();
	gpEventManager = new EventManager();
	gpLevelManager = new LevelManager();

	bool appIsRunning = true;
	bool hasStarted = false;

	// Load Level
	gpLevelManager->LoadLevel();
	GameObject* firstScreen = gpObjectFactory->LoadObject("firstScreen");

	//----------------------------------------------
	// Game Loop
	//----------------------------------------------
	while (appIsRunning) {
		if (glfwWindowShouldClose(gpWindow)) {
			appIsRunning = false;
		}

		gpFrameRateController->FrameStart();
		//printf("FrameTime: %lf\n", gpFrameRateController->GetFrameTime());

		// Track all events to process this cycle
		glfwPollEvents();

		// Update Input Manager with new keystrokes
		gpInputManager->Update();

		// Update all timed events
		gpEventManager->Update(gpFrameRateController->GetFrameTime());

		// Update Physics (10 times each frame for smoother physics)
		//for (int i = 0; i < 10; ++i) {
		gpPhysicsManager->Update(gpFrameRateController->GetFrameTime());// 10.0f);
		//}

		// Update all game Objects
		gpGoManager->Update();

		// Draw Everything
		gpRenderManager->Draw();

		// call once all drawing is done
		glfwSwapBuffers(gpWindow);

		// Debug Mode
		if (gpInputManager->isTriggered(KEYBOARD_KEYS::D)) {
			gpRenderManager->isDebugMode = !gpRenderManager->isDebugMode;
		}

		// press space to start game
		if (gpInputManager->isPressed(KEYBOARD_KEYS::JUMP) && !hasStarted) {
			gpGoManager->DeleteGameObject(firstScreen);
			gpGoManager->ShootBall();
			hasStarted = true;
		}

		if (gpInputManager->isPressed(KEYBOARD_KEYS::ESC)) {
			appIsRunning = false;
		}
		gpFrameRateController->FrameEnd();
	}

	// free the managers
	delete gpInputManager;
	delete gpFrameRateController;
	delete gpResourceManager;
	delete gpGoManager;
	delete gpEventManager;
	delete gpLevelManager;
	delete gpRenderManager;
	delete gpCollisionManager;
	delete gpPhysicsManager;
	delete gpObjectFactory;

	// exit gracefully
	glfwTerminate();
}
