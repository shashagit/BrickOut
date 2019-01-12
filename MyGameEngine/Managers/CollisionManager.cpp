#include "CollisionManager.h"
#include "Shape.h"

extern "C" {
#include "../Libs/Math2D.h"
}

Contact::Contact() {
	mpBodies[0] = nullptr;
	mpBodies[1] = nullptr;
	Vector2DSet(&differenceVector, 0.0f, 0.0f);
	colDir = CollisionDirection::NONE;
}

bool CheckCollisionCircleCircle(Shape *pShape1, Vector2D Pos1,
	Shape *pShape2, Vector2D Pos2, std::list<Contact*> &Contacts)
{
	ShapeCircle* c1 = static_cast<ShapeCircle*>(pShape1);
	ShapeCircle* c2 = static_cast<ShapeCircle*>(pShape2);

	if (StaticCircleToStaticCircle(&Pos1, c1->mRadius, &Pos2, c2->mRadius)) {
		Contact *pNewContact = new  Contact();

		pNewContact->mpBodies[0] = pShape1->mpOwnerBody;
		pNewContact->mpBodies[1] = pShape2->mpOwnerBody;
		Contacts.push_back(pNewContact);

		return true;
	}

	return false;
};


CollisionDirection CheckDirection(Vector2D diff) {
	Vector2D directions[4];
	Vector2DNormalize(&diff, &diff);
	// set all direction vectors
	Vector2D dir;
	Vector2DSet(&dir, 0.0f, 1.0f);
	directions[NORTH] = dir;
	Vector2DSet(&dir, 0.0f, -1.0f);
	directions[SOUTH] = dir;
	Vector2DSet(&dir, -1.0f, 0.0f);
	directions[WEST] = dir;
	Vector2DSet(&dir, 1.0f, 0.0f);
	directions[EAST] = dir;

	float max = 0.0f;
	int best_match = -1;
	for (int i = 0; i < 4; i++)
	{
		float dot_product = Vector2DDotProduct(&diff, &directions[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}

	return static_cast<CollisionDirection>(best_match);
}

bool CheckCollisionAABBCircle(Shape *pShape1, Vector2D Pos1,
	Shape *pShape2, Vector2D Pos2, std::list<Contact*> &Contacts)
{

	ShapeAABB *r1 = static_cast<ShapeAABB*>(pShape1);
	ShapeCircle* c2 = static_cast<ShapeCircle*>(pShape2);

	if (StaticPointToStaticRect(&Pos2, &Pos1, r1->mRight - r1->mLeft + 2*c2->mRadius, r1->mTop - r1->mBottom + 2*c2->mRadius)) {
		// the positions inside this if are of the collision
		// we can use these to find out position of collision
		// to better reflect off the paddle
		Vector2D centerDistance;
		Vector2DSub(&centerDistance, &Pos2, &Pos1);

		//clamping center distance gives us the vector to the nearest side of the AABB
		centerDistance.x = (centerDistance.x > r1->GetExtents().x) ? r1->GetExtents().x : centerDistance.x;
		centerDistance.x = (centerDistance.x < -r1->GetExtents().x) ? -r1->GetExtents().x : centerDistance.x;
		centerDistance.y = (centerDistance.y > r1->GetExtents().y) ? r1->GetExtents().y : centerDistance.y;
		centerDistance.y = (centerDistance.y < -r1->GetExtents().y) ? -r1->GetExtents().y : centerDistance.y;

		// vector to closest point on AABB
		Vector2D closest;
		Vector2DAdd(&closest, &Pos1, &centerDistance);

		// vector from circle center to closest point
		Vector2D difference;
		Vector2DSub(&difference, &closest, &Pos2);


		Contact *pNewContact = new  Contact();


		pNewContact->mpBodies[0] = pShape1->mpOwnerBody;
		pNewContact->mpBodies[1] = pShape2->mpOwnerBody;
		pNewContact->differenceVector = difference;
		pNewContact->colDir = CheckDirection(difference);

		//std::cout << "AABB-Circle collision occured in " << pNewContact->colDir <<  " direction" << std::endl;
		
		Contacts.push_back(pNewContact);

		return true;
	}
	return false;
}

bool CheckCollisionCircleAABB(Shape *pShape1, Vector2D Pos1,
	Shape *pShape2, Vector2D Pos2, std::list<Contact*> &Contacts)
{
	return CheckCollisionAABBCircle(pShape2, Pos2, pShape1, Pos1, Contacts);
}

bool CheckCollisionAABBAABB(Shape *pShape1, Vector2D Pos1,
	Shape *pShape2, Vector2D Pos2, std::list<Contact*> &Contacts)
{
	ShapeAABB *r1 = static_cast<ShapeAABB*>(pShape1);
	ShapeAABB *r2 = static_cast<ShapeAABB*>(pShape2);

	if (StaticRectToStaticRect(&Pos1, r1->mRight - r1->mLeft, r1->mTop - r1->mBottom,
		&Pos2, r2->mRight - r2->mLeft, r2->mTop - r2->mBottom)) {

		// Generate Contact and set data for reflection
		Contact *pNewContact = new  Contact();

		//std::cout << "AABB-AABB collision occured" << std::endl;
		pNewContact->mpBodies[0] = pShape1->mpOwnerBody;
		pNewContact->mpBodies[1] = pShape2->mpOwnerBody;
		Contacts.push_back(pNewContact);

		return true;
	}

	return false;
}



CollisionManager::CollisionManager()
{
	

	// set all function pointers
	CollisionFunctions[CIRCLE][CIRCLE] = CheckCollisionCircleCircle;
	CollisionFunctions[CIRCLE][AABB] = CheckCollisionCircleAABB;
	CollisionFunctions[AABB][CIRCLE] = CheckCollisionAABBCircle;
	CollisionFunctions[AABB][AABB] = CheckCollisionAABBAABB;
}


CollisionManager::~CollisionManager()
{
	Reset();
}

void CollisionManager::Reset()
{
	for (auto contact : mContacts) {
		delete contact;
	}

	mContacts.clear();
}

bool CollisionManager::CheckCollisionAndGenerateContact(Shape * pShape1, Vector2D Pos1, Shape * pShape2, Vector2D Pos2)
{
	return CollisionFunctions[pShape1->mType][pShape2->mType](pShape1, Pos1, pShape2, Pos2, mContacts);
}
