#ifndef OBJECT_H
#define OBJECT_H

#include<vector>

class Component;
class Event;

enum GameObjectID {
	BALL,
	PADDLE,
	BRICKS,
	POWERUP,
	POWERUP2,
	ENEMY,
	ENEMY2,
	IDNUM
};

class GameObject {
	public:
	GameObject();
	~GameObject();
	
	void HandleEvent(Event *pEvent);

	void Update();
	Component* AddComponent(unsigned int Type);
	Component* GetComponent(unsigned int Type);
	
	public:
	//Sprite *mpSprite;
	//Transform* mpTransform;
	//Controller* mpController;
	GameObjectID ID;
	std::vector<Component*> mComponents;

	std::string jsonString;
};

#endif