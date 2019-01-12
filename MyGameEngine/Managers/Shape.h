#pragma once
class Body;
enum ShapeType
{
	CIRCLE,
	AABB,
	SHAPESNUM
};

class Shape
{
public:

	Shape(ShapeType type)
	{
		mType = type;
		mpOwnerBody = nullptr;
	}
	virtual ~Shape() {}
	virtual Vector2D GetExtents() = 0;
	virtual bool TestPoint(float PointX, float PointY) = 0;

	Body* mpOwnerBody;

public:
	ShapeType mType;
};

class ShapeCircle : public Shape
{
public:
	ShapeCircle(float radius) : Shape(CIRCLE)
	{
		mRadius = radius;
	}
	bool TestPoint(float PointX, float PointY)
	{
		return false;
	}
	Vector2D GetExtents() {
		Vector2D extents;
		Vector2DSet(&extents, mRadius, mRadius);
		return extents;
	}

	float mRadius;
};

class ShapeAABB : public Shape
{
public:
	ShapeAABB(float top, float bottom, float left, float right) : Shape(AABB)
	{
		mTop = top;
		mBottom = bottom;
		mLeft = left;
		mRight = right;
	}
	bool TestPoint(float PointX, float PointY)
	{
		return false;
	}
	Vector2D GetExtents() {
		Vector2D extents;
		Vector2DSet(&extents, (mRight-mLeft)/2, (mTop - mBottom)/2);
		return extents;
	}

	float mTop, mBottom, mLeft, mRight;
};

