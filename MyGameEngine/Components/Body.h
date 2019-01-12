#pragma once
#pragma once

#include "Component.h"

extern "C" {
#include "../Libs/Vector2D.h"
}

class Shape;

class Body : public Component
{
public:
	Body();
	~Body();

	void Update();
	void Serialize(const char* data);

	void Initialize();
	Body* Create();
	void HandleEvent(Event *pEvent);

	void Integrate(float gravity, float deltaTime);
public:
	Vector2D mPos;
	Vector2D mPrevPos;
	Vector2D mVel;
	Vector2D mAcc;
	Vector2D mTotalForce;

	bool isOutOfBounds;

	float mMass, mInvMass;

	Shape* mpShape;
};

