#pragma once
#include <list>
#include <unordered_map>
#include "Event.h"

class GameObject;

class EventManager
{
public:
	EventManager();
	~EventManager();

	void BroadcastEvent(Event *pEvent);
	void BroadcastEventToSubscribers(Event *pEvent);
	void AddTimedEvent(Event *pEvent);

	void Update(float frameTime);

	void Subscribe(EventType Type, GameObject * pGameObjectNewSubscriber);
	void Unsubscribe(EventType Type, GameObject* pGameObjectOldSubscriber);
	std::list<Event*> mTimedEvents;
	std::unordered_map < EventType, std::list<GameObject*>> mSubscriptions;
};

