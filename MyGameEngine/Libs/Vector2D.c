/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D.c
Purpose: Defines all functions for Vector2D Operations
Language: C++, MS C++ Compiler
Platform: MS Visual Studio 2017, Windows 10
Project: CS529_shashwatpandey_1
Author: Shashwat Pandey, shashwat.pandey, ID: 60003718
Creation date: 19 September 2018
- End Header --------------------------------------------------------*/

#include "Vector2D.h"

#define EPSILON 0.0001
#define PI      3.1415926535897932384626433832795

// ---------------------------------------------------------------------------

void Vector2DZero(Vector2D *pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D *pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DSetV(Vector2D *pResult, Vector2D *other)
{
	pResult->x = other->x;
	pResult->y = other->y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
	pResult->x = -(pVec0->x);
	pResult->y = -(pVec0->y);
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = pVec0->x + pVec1->x;
	pResult->y = pVec0->y + pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
	float magnitude = Vector2DLength(pVec0);
	pResult->x = (pVec0->x)/magnitude;
	pResult->y = (pVec0->y)/magnitude;
}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
	pResult->x = (pVec0->x) * c;
	pResult->y = (pVec0->y) * c;
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = (pVec0->x) * c + pVec1->x;
	pResult->y = (pVec0->y) * c + pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = (pVec0->x) * c - pVec1->x;
	pResult->y = (pVec0->y) * c - pVec1->y;
}

// ---------------------------------------------------------------------------

float Vector2DLength(Vector2D *pVec0)
{
	return sqrtf((pVec0->x * pVec0->x) + (pVec0->y * pVec0->y));
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(Vector2D *pVec0)
{
	return (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
}

// ---------------------------------------------------------------------------

float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return sqrtf((pVec0->x - pVec1->x)*(pVec0->x - pVec1->x) + (pVec0->y - pVec1->y)*(pVec0->y - pVec1->y));
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return (pVec0->x - pVec1->x)*(pVec0->x - pVec1->x) + (pVec0->y - pVec1->y)*(pVec0->y - pVec1->y);
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
	return (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
	float angleInRadians = (angle * PI) / 180.0f;
	Vector2DFromAngleRad(pResult, angleInRadians);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
	pResult->x = cosf(angle);
	pResult->y = sinf(angle);
}

// ---------------------------------------------------------------------------
