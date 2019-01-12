#include "Controller.h"
#include "../Managers/InputManager.h"
#include "../GameObject/GameObject.h"
#include "Body.h"
#include "Transform.h"
#include "GLFW/glfw3.h"

extern InputManager* gpInputManager;


Controller::Controller() : Component(CONTROLLER) {
	
}

Controller::~Controller() {}

Controller * Controller::Create()
{
	return new Controller();
}

void Controller::Update() {
	if(mpOwner != nullptr && gpInputManager!= nullptr)
	{
		Body* pBr = static_cast<Body*>(mpOwner->GetComponent(BODY));
		if(pBr != nullptr) {
			/*if (gpInputManager->isPressed(KEYBOARD_KEYS::UP)) {
				pBr->mPos.y += 2;
			}
			if (gpInputManager->isPressed(KEYBOARD_KEYS::DOWN)) {
				pBr->mPos.y -= 2;
			}*/
			if (gpInputManager->isPressed(KEYBOARD_KEYS::LEFT)) {
				pBr->mPos.x -= 5;
			}
			if (gpInputManager->isPressed(KEYBOARD_KEYS::RIGHT)) {
				pBr->mPos.x += 5;
			}
			/*if (gpInputManager->isPressed(KEYBOARD_KEYS::Z)) {
				Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
				pTr->mAngle += 1;
			}
			if (gpInputManager->isPressed(KEYBOARD_KEYS::X)) {
				Transform* pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
				pTr->mAngle -= 1;
			}*/
		}
	}
}