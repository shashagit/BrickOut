#pragma once

#include "Component.h"
extern "C" {
#include "../Libs/Matrix2D.h"
}

class Transform : public Component{
	public:
	Transform();
	~Transform();
	
	void Update();
	void SetPos(float, float);
	void SetScale(float, float);

	void Serialize(const char* data);

	Transform* Create();

	Matrix2D GetTransformMatrix();
	public:
		Vector2D mPos;
		float mAngle;
		Vector2D mScale;
private:
	// the transformation matrix
	Matrix2D mTransform;

	Matrix2D matScale;
	Matrix2D matTrans;
	Matrix2D matRot;

};