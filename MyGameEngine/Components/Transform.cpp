#include "Transform.h"

Transform::Transform() : Component(TRANSFORM) {
	Vector2DZero(&mPos);
	Vector2DZero(&mScale);
	mAngle = 0.0f;
}
Transform::~Transform() {}

Transform* Transform::Create() {
	return new Transform();
}

void Transform::SetPos(float x, float y) {
	Vector2DSet(&mPos, x, y);
}
void Transform::SetScale(float x, float y) {
	Vector2DSet(&mScale, x, y);
}

void Transform::Serialize(const char* data)
{
	Document d;
	d.Parse(data);

	SetPos(d["Position"]["x"].GetFloat(), d["Position"]["y"].GetFloat());

	if(d.HasMember("Scale"))
		SetScale(d["Scale"]["x"].GetFloat(), d["Scale"]["y"].GetFloat());

	if (d.HasMember("Angle"))
		mAngle = d["Angle"].GetFloat();
}


Matrix2D Transform::GetTransformMatrix()
{
	return mTransform;
}

void Transform::Update() {
	Matrix2DScale(&matScale, mScale.x, mScale.y);
	Matrix2DRotDeg(&matRot, mAngle);
	Matrix2DTranslate(&matTrans, mPos.x, mPos.y);

	Matrix2DConcat(&mTransform, &matTrans, &matRot);
	Matrix2DConcat(&mTransform, &mTransform, &matScale);
}