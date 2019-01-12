#include "Brick.h"
#include "Sprite.h"
#include "Body.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/GameObjectManager.h"
#include "../Managers/Event.h"
#include "../Managers/EventManager.h"

#include "../GameObject/ObjectFactory.h"


extern EventManager* gpEventManager;

extern GameObjectManager* gpGoManager;


// Brick should be renamed to Destroyable component
Brick::Brick() : Component(BRICK), maxHitCount(0)
{
	

}


Brick::~Brick()
{
}

void Brick::Update()
{
}

Brick * Brick::Create()
{
	return new Brick();
}

void Brick::SetMaxHitCount(int num)
{
	maxHitCount = num;

	if (mpOwner->ID == POWERUP || mpOwner->ID == POWERUP2) {
		gpEventManager->Subscribe(OUT_OF_BOUNDS, mpOwner);
	}
	else if (mpOwner->ID == BRICKS) {
		gpEventManager->Subscribe(I_GOT_DESTROYED, mpOwner);
	}
}

static int hitCount = 0;



void Brick::TakeAHit()
{
	--maxHitCount;
	if (maxHitCount == 0) {
		//printf(" destroy brick \n");
		DestroyedEvent dest;
		HandleEvent(&dest);
		return;
	}
	
	Sprite* pSp = static_cast<Sprite*>(mpOwner->GetComponent(SPRITE));
	std::string texName = "brick" + std::to_string(maxHitCount);
	pSp->OverwriteTexFileName(texName.c_str());

}

void Brick::HandleEvent(Event *pEvent) {
	if (pEvent->mType == COLLIDE) {
		if(maxHitCount <= 2) // only destroyable bricks take a Hit
			TakeAHit();
	}

	if (pEvent->mType == I_GOT_DESTROYED) {

		if (mpOwner->ID == BRICKS)
			gpGoManager->SpawnDrops(mpOwner);

		else if (mpOwner->ID == POWERUP)
			gpGoManager->ApplyPowerUp(POWERUP);

		else if (mpOwner->ID == POWERUP2)
			gpGoManager->ApplyPowerUp(POWERUP2);

		gpGoManager->DeleteGameObject(mpOwner); // delete mpOwner;

	}

	if (pEvent->mType == OUT_OF_BOUNDS) { // is only called for PowerUps
		OutOfBoundsEvent* obe = static_cast<OutOfBoundsEvent*>(pEvent);
		if (obe->id == POWERUP || obe->id == POWERUP2) {
			//std::cout << "Drop destroyed" << std::endl;
			gpGoManager->DeleteGameObject(mpOwner); // delete mpOwner;
		}
	}
}
