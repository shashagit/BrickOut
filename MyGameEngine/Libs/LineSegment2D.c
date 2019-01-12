#include "LineSegment2D.h"


int BuildLineSegment2D(LineSegment2D *LS, Vector2D *Point0, Vector2D *Point1)
{
	LS->mP0 = *Point0;
	LS->mP1 = *Point1;

	Vector2D diff, normalVec;
	Vector2DSub(&diff, Point1, Point0);
	Vector2DSet(&normalVec, diff.y, -diff.x);
	Vector2DNormalize(&LS->mN, &normalVec);

	LS->mNdotP0 = Vector2DDotProduct(&LS->mN, Point0);

	return 1;
}