#pragma once
#include "../GameObject/GameObject.h"

enum EventType {
	COLLIDE,
	PLAYER_HIT,
	OUT_OF_BOUNDS,
	I_GOT_DESTROYED,
	NUM_OF_EVENTS
};

class Event {
public:
	Event(EventType Type) : mType(Type), mTimer(0.0f) { }
	virtual ~Event() {}

	EventType mType;
	float mTimer;
};

class CollideEvent : public Event {
public:
	CollideEvent() : Event(COLLIDE) {}
	~CollideEvent() {}
};

class PlayerHitEvent : public Event {
public:
	PlayerHitEvent() : Event(PLAYER_HIT) {}
	~PlayerHitEvent() {}
};

class OutOfBoundsEvent : public Event {
public:
	GameObjectID id;
	OutOfBoundsEvent(GameObjectID _id) : Event(OUT_OF_BOUNDS), id(_id) {}
	~OutOfBoundsEvent() {}
};

class DestroyedEvent : public Event {
public:
	DestroyedEvent() : Event(I_GOT_DESTROYED) {}
	~DestroyedEvent() {}
};