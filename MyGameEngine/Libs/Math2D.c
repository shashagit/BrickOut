/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Math2D.c
Purpose: Defines collision functions for static shapes and collisions 
between animated points/circles and static lines
Language: C++, MS C++ Compiler
Platform: MS Visual Studio 2017, Windows 10
Project: CS529_shashwatpandey_2
Author: Shashwat Pandey, shashwat.pandey, ID: 60003718
Creation date: 1 October 2018
- End Header --------------------------------------------------------*/

#include "Math2D.h"
#include "stdio.h"

#define EPSILON 0.00001f

////////////////////
// From Project 1 //
////////////////////


/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius)
{
	if (Vector2DSquareDistance(pP, pCenter) > (Radius * Radius)) {
		return 0;
	}
	else {
		return 1;
	}
	return 0;
}


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height)
{
	if ((pPos->x < (pRect->x - Width / 2)) ||
		(pPos->x > (pRect->x + Width / 2)) ||
		(pPos->y < (pRect->y - Height / 2)) ||
		(pPos->y > (pRect->y + Height / 2))) {
		return 0;
	}
	else {
		return 1;
	}
	return 0;
}

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1)
{
	if (Vector2DSquareDistance(pCenter0, pCenter1) > (Radius0 + Radius1)*(Radius0 + Radius1)) {
		return 0;
	}
	else {
		return 1;
	}
	return 0;
}

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1)
{
	if (((pRect0->x - Width0 / 2) > (pRect1->x + Width1 / 2)) ||
		((pRect1->x - Width1 / 2) > (pRect0->x + Width0 / 2)) ||
		((pRect0->y - Height0 / 2) > (pRect1->y + Height1 / 2)) ||
		((pRect1->y - Height1 / 2) > (pRect0->y + Height0 / 2))) {
		return 0;
	}
	else {
		return 1;
	}
	return 0;
}



//////////////////////
// New to project 2 //
//////////////////////


/*
This function determines the distance separating a point from a line

 - Parameters
	- P:		The point whose location should be determined
	- LS:		The line segment

 - Returned value: The distance. Note that the returned value should be:
	- Negative if the point is in the line's inside half plane
	- Positive if the point is in the line's outside half plane
	- Zero if the point is on the line
*/
float StaticPointToStaticLineSegment(Vector2D *P, LineSegment2D *LS)
{	
	Vector2D diff;
	Vector2DSub(&diff, P, &LS->mP0);
	return Vector2DDotProduct(&LS->mN, &diff);
}


/*
This function checks whether an animated point is colliding with a line segment

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticLineSegment(Vector2D *Ps, Vector2D *Pe, LineSegment2D *LS, Vector2D *Pi)
{
	return AnimatedCircleToStaticLineSegment(Ps, Pe, 0.0f, LS, Pi);
}


/*
This function checks whether an animated circle is colliding with a line segment

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticLineSegment(Vector2D *Ps, Vector2D *Pe, float Radius, LineSegment2D *LS, Vector2D *Pi)
{
	float t = 0.0f;
	Vector2D moveVec, v1, v2, impact;
	float dotp1 = Vector2DDotProduct(&(LS->mN), Ps);
	float dPsLS = StaticPointToStaticLineSegment(Ps, LS);
	float dPeLS = StaticPointToStaticLineSegment(Pe, LS);

	/* check if both points are on one side of the line */
	if (dPsLS < -Radius && dPeLS < -Radius ||
		dPsLS > Radius && dPeLS > Radius) {
		return -1.0f;
	}

	/* if Ps is in the inside half then radius is negative in line eq */
	if (dPsLS < 0) {
		Radius = -Radius;
	}

	Vector2DSub(&moveVec, Pe, Ps);
	float dotdenom = Vector2DDotProduct(&LS->mN, &moveVec);
	if (dotdenom == 0.0f) { /* denominator should not be zero */
		return -1.0f;
	}
	t = (LS->mNdotP0 - dotp1 + Radius) / dotdenom;

	Vector2DScaleAdd(&impact, &moveVec, Ps, t);

	/* before returning check if the point lies on the segment*/
	Vector2DSub(&v1, &impact, &LS->mP0);
	Vector2DSub(&v2, &impact, &LS->mP1);
	if (Vector2DDotProduct(&v1, &v2) <= 0) {
		// collides with line segment
		*Pi = impact;
		return t;
	}
	return -1.0f;

}


/*
This function reflects an animated point on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticLineSegment(Vector2D *Ps, Vector2D *Pe, LineSegment2D *LS, Vector2D *Pi, Vector2D *R)
{

	return ReflectAnimatedCircleOnStaticLineSegment(Ps, Pe, 0.0f, LS, Pi, R);
}


/*
This function reflects an animated circle on a line segment.
It should first make sure that the animated point is intersecting with the line 

 - Parameters
	- Ps:		The center's starting location
	- Pe:		The center's ending location
	- Radius:	The circle's radius
	- LS:		The line segment
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:				If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticLineSegment(Vector2D *Ps, Vector2D *Pe, float Radius, LineSegment2D *LS, Vector2D *Pi, Vector2D *R)
{
	Vector2D colToEnd, vec;
	float collisionTime = AnimatedCircleToStaticLineSegment(Ps, Pe, Radius, LS, Pi);
	
	if (collisionTime >= 0 && collisionTime <= 1) {
		
		Vector2DSub(&colToEnd, Pe, Pi);
		Vector2DScale(&vec, &LS->mN, 2 * Vector2DDotProduct(&colToEnd, &LS->mN));

		Vector2DSub(R, &colToEnd, &vec);
		return collisionTime;
	}
	return -1.0f;
}


/*
This function checks whether an animated point is colliding with a static circle

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedPointToStaticCircle(Vector2D *Ps, Vector2D *Pe, Vector2D *Center, float Radius, Vector2D *Pi)
{
	// vector m is PsC vector dot V(PsPe) vec
	Vector2D startToCenter, velocity, velocityNormal;
	float approach, startToCenterLength, ti, s;
	
	Vector2DSub(&velocity, Pe, Ps);
	Vector2DSub(&startToCenter, Center, Ps);

	Vector2DNormalize(&velocityNormal, &velocity);
	approach = Vector2DDotProduct(&startToCenter, &velocityNormal);

	// if approach is negative the point is moving away from the circle
	if (approach < 0.0f)
	{
		return -1.0f;
	} 

	// if approach is positive check if intersecting
	if (Vector2DSquareDistance(Center, Ps) - approach * approach > Radius * Radius) {
		return -1.0f;
	}
	else {
		s = sqrt(Radius*Radius - (Vector2DSquareDistance(Center, Ps) - approach * approach));
		ti = (approach - s)/ Vector2DLength(&velocity);
	}

	Vector2DScaleAdd(Pi, &velocity, Ps, ti);
	// Make sure that Pi is between Pe and Ps

	return ti;
}



/*
This function reflects an animated point on a static circle.
It should first make sure that the animated point is intersecting with the circle 

 - Parameters
	- Ps:		The point's starting location
	- Pe:		The point's ending location
	- Center:	The circle's center
	- Radius:	The circle's radius
	- Pi:		This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:		Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedPointOnStaticCircle(Vector2D *Ps, Vector2D *Pe, Vector2D *Center, float Radius, Vector2D *Pi, Vector2D *R)
{
	float intersectionTime = AnimatedPointToStaticCircle(Ps, Pe, Center, Radius, Pi);
	
	if (intersectionTime >= 0.0f && intersectionTime <= 1.0f) {
		Vector2D dist;
		Vector2DSub(&dist, Pe, Ps);
		float length = Vector2DLength(&dist);
		Vector2D toReflect, normal, temp;
		Vector2DSub(&toReflect, Ps, Pi);
		Vector2DSub(&normal, Pi, Center);
		Vector2DNormalize(&normal, &normal);

		Vector2DScale(&temp, &normal, 2 * Vector2DDotProduct(&toReflect, &normal));
		Vector2DSub(R, &temp, &toReflect);
		Vector2DNormalize(R, R);
		Vector2DScale(R, R, (1-intersectionTime) * length);
		return intersectionTime;
	}

	return -1.0f;
}


/*
This function checks whether an animated circle is colliding with a static circle

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float AnimatedCircleToStaticCircle(Vector2D *Center0s, Vector2D *Center0e, float Radius0, Vector2D *Center1, float Radius1, Vector2D *Pi)
{
	return AnimatedPointToStaticCircle(Center0s, Center0e, Center1, Radius0 + Radius1, Pi);
}


/*
This function reflects an animated circle on a static circle.
It should first make sure that the animated circle is intersecting with the static one 

 - Parameters
	- Center0s:		The starting position of the animated circle's center 
	- Center0e:		The ending position of the animated circle's center 
	- Radius0:		The animated circle's radius
	- Center1:		The static circle's center
	- Radius1:		The static circle's radius
	- Pi:			This will be used to store the intersection point's coordinates (In case there's an intersection)
	- R:			Reflected vector R

 - Returned value: Intersection time t
	- -1.0f:		If there's no intersection
	- Intersection time:	If there's an intersection
*/
float ReflectAnimatedCircleOnStaticCircle(Vector2D *Center0s, Vector2D *Center0e, float Radius0, Vector2D *Center1, float Radius1, Vector2D *Pi, Vector2D *R)
{
	return ReflectAnimatedPointOnStaticCircle(Center0s, Center0e, Center1, Radius0 + Radius1, Pi, R);
}
