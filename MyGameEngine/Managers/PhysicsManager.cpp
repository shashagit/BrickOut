#include "PhysicsManager.h"
#include "..\Components\Component.h"
#include "..\GameObject\GameObjectManager.h"
#include "..\Components\Body.h"
#include "CollisionManager.h"
#include "..\GameObject\GameObject.h"
#include "Event.h"
#include "EventManager.h"

extern int const SCREEN_WIDTH;
extern int const SCREEN_HEIGHT;

extern EventManager* gpEventManager;

extern GameObjectManager *gpGoManager;
extern CollisionManager* gpCollisionManager;


PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Update(float frametime)
{
	// integrate all objects
	for (auto go : gpGoManager->mGameObjects)
	{
		Body *pBody = static_cast<Body*>(go->GetComponent(BODY));
		if (nullptr != pBody) {
			pBody->Integrate(-100.0f, frametime);

			// keep in bounds
			Vector2D extents = pBody->mpShape->GetExtents();
			if (pBody->mPos.x - extents.x < -SCREEN_WIDTH / 2 ||
				pBody->mPos.x + extents.x > SCREEN_WIDTH / 2) {
				// for ball
				if (abs(pBody->mVel.x) > 0) { // if velocity is non-zero we need to invert it
					pBody->mVel.x = -pBody->mVel.x; // invert velocity in x direction
				}
				else // for paddle 
				{ // just clamp position
					pBody->mPos.x = pBody->mPos.x < 0 ?
						-SCREEN_WIDTH / 2 + extents.x:
						SCREEN_WIDTH / 2 - extents.x;
				}
			} // anything that falls outside the screen raises an event
			else if (pBody->mPos.y - extents.y < -SCREEN_HEIGHT / 2 ||
				pBody->mPos.y + extents.y > SCREEN_HEIGHT / 2) {
				OutOfBoundsEvent obe(pBody->mpOwner->ID);
				gpEventManager->BroadcastEventToSubscribers(&obe);
			}
			
		}
	}

	// bound checks
	// send out of bounds event (only ball can go out of bounds)
	/*Body* pBall = static_cast<Body*>(gpGoManager->ball->GetComponent(BODY));
	if (pBall->mPos.y + pBall->mpShape->GetExtents().y > SCREEN_HEIGHT / 2 ||
		pBall->mPos.y - pBall->mpShape->GetExtents().y < -SCREEN_HEIGHT / 2) {
		OutOfBoundsEvent obe;
		gpEventManager->BroadcastEventToSubscribers(&obe);
	}*/


	gpCollisionManager->Reset();

	auto pObj1 = gpGoManager->mGameObjects.begin();
	auto pObjLast = gpGoManager->mGameObjects.end();

	for (; pObj1 != pObjLast; ++pObj1) {
		Body *pBody1 = static_cast<Body*>((*pObj1)->GetComponent(BODY));

		if (nullptr == pBody1)
			continue;

		for (auto pObj2 = pObj1 + 1; pObj2 != pObjLast; ++pObj2) {
			Body *pBody2 = static_cast<Body*>((*pObj2)->GetComponent(BODY));

			if (nullptr == pBody2)
				continue;

			// if both are bricks, continue
			if (pBody1->mpOwner->ID == BRICKS &&
				pBody2->mpOwner->ID == BRICKS) {
				continue;
			}
			else if ((pBody1->mpOwner->ID == ENEMY || pBody1->mpOwner->ID == ENEMY2) &&
				(pBody2->mpOwner->ID == ENEMY || pBody2->mpOwner->ID == ENEMY2)) {
				continue;
			}
			else if (pBody1->mpOwner->ID == BRICKS &&
				(pBody2->mpOwner->ID == POWERUP2 || pBody2->mpOwner->ID == POWERUP)) {
				continue;
			} else if ((pBody1->mpOwner->ID == POWERUP2 || pBody1->mpOwner->ID == POWERUP) &&
				pBody2->mpOwner->ID == BRICKS) {
				continue;
			} else if ((pBody1->mpOwner->ID == POWERUP2 || pBody1->mpOwner->ID == POWERUP) &&
				pBody2->mpOwner->ID == BALL) {
				continue;
			} else if (pBody1->mpOwner->ID == BALL &&
				(pBody2->mpOwner->ID == POWERUP2 || pBody2->mpOwner->ID == POWERUP)) {
				continue;
			}
			else if ((pBody1->mpOwner->ID == POWERUP2 || pBody1->mpOwner->ID == POWERUP) &&
				(pBody2->mpOwner->ID == ENEMY || pBody2->mpOwner->ID == ENEMY2)) {
				continue;
			}
			else if ((pBody2->mpOwner->ID == POWERUP2 || pBody2->mpOwner->ID == POWERUP) &&
				(pBody1->mpOwner->ID == ENEMY || pBody1->mpOwner->ID == ENEMY2)) {
				continue;
			}
			else {
				gpCollisionManager->CheckCollisionAndGenerateContact(pBody1->mpShape, pBody1->mPos,
					pBody2->mpShape, pBody2->mPos);
			}
		}
	}
	

	// super advanced physics here
	for (auto c : gpCollisionManager->mContacts) {
		if (c->colDir != NONE) { // means ball collides with something AABB
			for (int i = 0; i < 2; ++i) {

				// body is ball
				if (c->mpBodies[i]->mpOwner->ID == BALL) { 
					if (c->colDir == EAST || c->colDir == WEST) {
						c->mpBodies[i]->mVel.x = -c->mpBodies[i]->mVel.x; // invert velocity in x
						float offset = c->mpBodies[i]->mpShape->GetExtents().x - abs(c->differenceVector.x);
						if (c->colDir == EAST)
							c->mpBodies[i]->mPos.x -= offset;
						else
							c->mpBodies[i]->mPos.x += offset;

					} else if (c->colDir == NORTH || c->colDir == SOUTH) {
						c->mpBodies[i]->mVel.y = -c->mpBodies[i]->mVel.y; // invert velocity in y
						float offset = c->mpBodies[i]->mpShape->GetExtents().y - abs(c->differenceVector.y);
						if (c->colDir == NORTH)
							c->mpBodies[i]->mPos.y -= offset;
						else
							c->mpBodies[i]->mPos.y += offset;
					}
				}

				// body is paddle
				if (c->mpBodies[i]->mpOwner->ID == PADDLE) {
					Body* pBall = static_cast<Body*>(gpGoManager->ball->GetComponent(BODY));
					float hitDistance = c->mpBodies[i]->mPos.x - pBall->mPos.x;
					float fractionPaddleLength = hitDistance / c->mpBodies[i]->mpShape->GetExtents().x;

					Vector2D hitVelocity = pBall->mVel;
					pBall->mVel.x -= fractionPaddleLength * 100.0f;
					Vector2DNormalize(&pBall->mVel, &pBall->mVel);
					Vector2DScale(&pBall->mVel, &pBall->mVel, Vector2DLength(&hitVelocity));

				}
			}
		}
	}

	// call relevant collision events
	// only looking at the first collision for now
	for (auto c : gpCollisionManager->mContacts) {
		CollideEvent ce;
		c->mpBodies[0]->mpOwner->HandleEvent(&ce);
		c->mpBodies[1]->mpOwner->HandleEvent(&ce);
		break;
	}
}