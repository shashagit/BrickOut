#include "Enemy.h"
#include "Body.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../Managers/FrameRateController.h"
#include "../Managers/EventManager.h"

extern GameObjectManager* gpGoManager;

extern FrameRateController *gpFrameRateController;
extern EventManager* gpEventManager;

Enemy::Enemy() : Component(ENEMY)
{
	mGoingUp = true;
	mTimer = 0;
	mTimerLimit = 2.0f;
	pBody = nullptr;
	pPad = nullptr;
}
Enemy::~Enemy()
{
}

Enemy* Enemy::Create() {
	return new Enemy();
}

void Enemy::Update()
{
	mTimer += gpFrameRateController->GetFrameTime();
	
	if(mTimer > mTimerLimit)
	{
		mGoingUp = !mGoingUp;
		mTimer = 0;
	}
	
	if(pBody == nullptr)
		pBody = static_cast<Body*>(mpOwner->GetComponent(BODY));
	
	if (mpOwner->ID == ENEMY) {
		if (mGoingUp == true)
			Vector2DSet(&pBody->mVel, -200.0f, 0.0f);
		else
			Vector2DSet(&pBody->mVel, 200.0f, 0.0f);
	}
	else if (mpOwner->ID == ENEMY2) {
		if(pPad == nullptr)
		    pPad = static_cast<Body*>(gpGoManager->paddles[0]->GetComponent(BODY));
		
		Vector2D dirVec;
		Vector2DSub(&dirVec, &pPad->mPos, &pBody->mPos);
		Vector2DNormalize(&dirVec, &dirVec);

		Vector2DSetV(&pBody->mVel, &dirVec);
		Vector2DScale(&pBody->mVel, &pBody->mVel, 25.0f);
	}
}

void Enemy::Serialize(const char * str)
{
}

void Enemy::HandleEvent(Event * pEvent)
{
	if (COLLIDE == pEvent->mType) {
		gpGoManager->DeleteGameObject(mpOwner);
	}
}
