#include "EventManager.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/GameObject.h"

extern GameObjectManager* gpGoManager;

EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}

void EventManager::BroadcastEvent(Event * pEvent)
{
	for (auto goc : gpGoManager->mGameObjects) {
		goc->HandleEvent(pEvent);
	}
}

void EventManager::BroadcastEventToSubscribers(Event * pEvent)
{
	auto subscriberList = mSubscriptions[pEvent->mType];
	for (auto pGo : subscriberList) {
		pGo->HandleEvent(pEvent);
	}
}

void EventManager::AddTimedEvent(Event * pEvent)
{
	mTimedEvents.push_back(pEvent);
}

void EventManager::Update(float frameTime) {
	// loop through all timed events
	// decrease timer by frametime
	// <0 ? fire event

	std::list<Event*>::iterator it;
	it = mTimedEvents.begin();

	while (it != mTimedEvents.end()) {
		Event *pEvent = *it;
		pEvent->mTimer -= frameTime;

		if (pEvent->mTimer < 0.0f) {
			BroadcastEventToSubscribers(pEvent);
			delete pEvent;

			it = mTimedEvents.erase(it);
		}
		else {
			++it;
		}
	}
}

void EventManager::Subscribe(EventType Type, GameObject * pGameObjectNewSubscriber)
{
	auto &subscriberList = mSubscriptions[Type];
	for (auto pGo : subscriberList) {
		if (pGo == pGameObjectNewSubscriber) {
			return;
		}
	}

	subscriberList.push_back(pGameObjectNewSubscriber);

}

void EventManager::Unsubscribe(EventType Type, GameObject* pGameObjectOldSubscriber) {
	auto &subscriberList = mSubscriptions[Type];
	auto it = std::find(subscriberList.begin(), subscriberList.end(), pGameObjectOldSubscriber);

	if (it == subscriberList.end())
		return;

	subscriberList.erase(it);
}
