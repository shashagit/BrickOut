#include <cstring>
#include "../Managers/CollisionManager.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "Transform.h"
#include "../Managers/PhysicsManager.h"
#include "../Managers/Shape.h"
#include "../Managers/Event.h"


extern GameObjectManager* gpGoManager;

Body::Body() : Component(BODY)
{
	Vector2DZero(&mPos);
	Vector2DZero(&mPrevPos);
	isOutOfBounds = false;
}


Body::~Body()
{
}

void Body::Update()
{
}

Body* Body::Create() {
	return new Body();
}

void Body::Serialize(const char* v)
{
	Document d;
	d.Parse(v);

	mMass = d["Mass"].GetFloat();

	if (mMass != 0.0f) {
		mInvMass = 1.0f / mMass;
	}
	else {
		mInvMass = 0.0f;
	}

	if (d.HasMember("Velocity")) {
		if (mpOwner->ID == BALL) {
			Vector2DSet(&gpGoManager->ballStartVelocity,
				d["Velocity"]["x"].GetFloat(),
				d["Velocity"]["y"].GetFloat());
		}
		else {
			mVel.x = d["Velocity"]["x"].GetFloat();
			mVel.y = d["Velocity"]["y"].GetFloat();
		}
	}

	const char* shapeType = d["Shape"]["Name"].GetString();

	if (strcmp(shapeType, "CIRCLE") == 0) {
		float rad = d["Shape"]["Radius"].GetFloat();

		mpShape = new ShapeCircle(rad);
		mpShape->mpOwnerBody = this;

	}
	else if (strcmp(shapeType, "AABB") == 0) {
		float t = d["Shape"]["Top"].GetFloat();
		float b = d["Shape"]["Bottom"].GetFloat();
		float l = d["Shape"]["Left"].GetFloat();
		float r = d["Shape"]["Right"].GetFloat();

		mpShape = new ShapeAABB(t, b, l, r);
		mpShape->mpOwnerBody = this;
	}
}

void Body::Initialize()
{
	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

	if (pTr != nullptr) {
		Vector2DSetV(&mPrevPos, &pTr->mPos);
		Vector2DSetV(&mPos, &pTr->mPos);
	}
}

void Body::Integrate(float gravity, float deltaTime)
{
	// save current position
	Vector2DSetV(&mPrevPos ,&mPos);

	// compute acceleration
	mTotalForce.y += gravity;
	Vector2DScale(&mTotalForce, &mTotalForce, mInvMass);
	Vector2DSetV(&mAcc, &mTotalForce);

	// integrate the velocity
	Vector2DScaleAdd(&mVel, &mAcc, &mVel, deltaTime);

	// integrate the position
	Vector2DScaleAdd(&mPos, &mVel, &mPos, deltaTime);

	Vector2DZero(&mTotalForce);

	Transform *pTr = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));

	if (pTr != nullptr) {
		Vector2DSetV(&pTr->mPos, &mPos);
	}
}

void Body::HandleEvent(Event *pEvent) {
	if (pEvent->mType == COLLIDE) {
		if (mpOwner->ID == BALL) {
			// increse velocity
			Vector2DScale(&mVel, &mVel, 1.02f);
			Vector2DScale(&mVel, &mVel, 0.99f);
			//printf("Vel : %f %f ", mVel.x, mVel.y);
		}
	}

	if(pEvent->mType == OUT_OF_BOUNDS) { 
		OutOfBoundsEvent* obe = static_cast<OutOfBoundsEvent*>(pEvent);
		if (obe->id == BALL) {
			if (!isOutOfBounds) {
				isOutOfBounds = true;
				Vector2DZero(&mVel); // stop the ball
				gpGoManager->BallOutOfBounds();

				// make ball vanish slowly
				if (mPos.y > 0) {
					Vector2DSet(&mVel, 0.0f, 10.0f);
				}
				else {
					Vector2DSet(&mVel, 0.0f, -10.0f);
				}
			}
		}
	}
}