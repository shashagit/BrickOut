#pragma once

#include <list>
#include "..\Components\Body.h"
#include "Shape.h"

enum CollisionDirection {
	NONE = -1,
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class Contact
{
public:
	Contact();
	Body* mpBodies[2];
	CollisionDirection colDir;
	Vector2D differenceVector;
};

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();
	
	void Reset();
	bool CheckCollisionAndGenerateContact(Shape *pShape1, Vector2D Pos1,
		Shape *pShape2, Vector2D Pos2);
public:
	std::list<Contact*> mContacts;

private:
	bool (*CollisionFunctions[ShapeType::SHAPESNUM][ShapeType::SHAPESNUM])(Shape *pShape1, Vector2D Pos1,
		Shape *pShape2, Vector2D Pos2, std::list<Contact*> &mContacts);
};


