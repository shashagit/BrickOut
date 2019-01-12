#pragma once
#include "Component.h"

class Brick : public Component
{
public:
	Brick();
	~Brick();
	void Update();
	Brick* Create();
	void HandleEvent(Event *pEvent);
	void RandomDropGenerate();
	void SetMaxHitCount(int);
	void TakeAHit();
private:
	int maxHitCount;
};

