#pragma once
#include <unordered_map>

class GameObject;

class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();

	void LoadLevel(const char* pFileName);
	void LoadBrickPattern(const char* pFileName);
	GameObject* LoadBrick(int index, int i, int j);
	GameObject* CreateObject(const char * str, const char * pFileName);
	GameObject* LoadObject(const char* pFileName);


};

