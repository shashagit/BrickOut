#pragma once

#include "Component.h"
class GameObject;

class Controller : public Component{
public:
	Controller();
	~Controller();
	Controller* Create();
	void Update();
};