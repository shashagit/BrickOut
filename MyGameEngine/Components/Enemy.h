#pragma once
#include "Component.h"
#include "Body.h"

class Enemy : public Component {
public:
	Enemy();
	~Enemy();

	void Update();
	Enemy* Create();
	void Serialize(const char* str);
	void HandleEvent(Event * pEvent);

private:
	bool mGoingUp;
	float mTimer;
	float mTimerLimit;
	Body* pBody, *pPad;
};