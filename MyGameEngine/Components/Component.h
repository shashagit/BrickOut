#pragma once
#include "stringbuffer.h"
#include "document.h"
#include "writer.h"
#include "../Managers/Event.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace rapidjson;

enum COMPONENT_TYPE
{
	TRANSFORM,
	SPRITE,
	CONTROLLER,
	BRICK,
	BODY,
	LEFTRIGHT
};

class GameObject;
class Event;

class Component {
	public:
	Component(unsigned int Type);
	virtual ~Component();
	
	virtual void Update() = 0; // abstract function
	unsigned int GetType() {return mType;}
	
	virtual Component* Create() = 0;
	virtual	void HandleEvent(Event *pEvent) {}
	virtual void Serialize(const char* d) {}

	public:
	GameObject *mpOwner;
	std::string jsonString;
	
	private:
	unsigned int mType;
};