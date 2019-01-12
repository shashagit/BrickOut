/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2D.c
Purpose: Defines all functions for Matrix Operations
Language: C++, MS C++ Compiler
Platform: MS Visual Studio 2017, Windows 10
Project: CS529_shashwatpandey_1
Author: Shashwat Pandey, shashwat.pandey, ID: 60003718
Creation date: 19 September 2018
- End Header --------------------------------------------------------*/

#include "Matrix2D.h"

#define PI      3.1415926535897932384626433832795


//void PrintMatrix(Matrix2D *pResult, char* name) {
//	printf("%s\n", name);
//	printf("%f\t%f\t%f\n", pResult->m[0][0], pResult->m[0][1], pResult->m[0][2]);
//	printf("%f\t%f\t%f\n", pResult->m[1][0], pResult->m[1][1], pResult->m[1][2]);
//	printf("%f\t%f\t%f\n\n", pResult->m[2][0], pResult->m[2][1], pResult->m[2][2]);
//}

/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult)
{
	pResult->m[0][0] = pResult->m[1][1] = pResult->m[2][2] = 1;
	pResult->m[1][0] = pResult->m[2][0] = pResult->m[2][1] = 0;
	pResult->m[0][1] = pResult->m[0][2] = pResult->m[1][2] = 0;
}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	//PrintMatrix(pMtx);

	pResult->m[0][0] = pMtx->m[0][0];
	pResult->m[1][1] = pMtx->m[1][1];
	pResult->m[2][2] = pMtx->m[2][2];

	float temp = pMtx->m[0][1];
	pResult->m[0][1] = pMtx->m[1][0];
	pResult->m[1][0] = temp;

	temp = pMtx->m[0][2];
	pResult->m[0][2] = pMtx->m[2][0];
	pResult->m[2][0] = temp;

	temp = pMtx->m[1][2];
	pResult->m[1][2] = pMtx->m[2][1];
	pResult->m[2][1] = temp;

	//PrintMatrix(pResult, __func__);
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
	int i = 0, j = 0, k = 0;
	Matrix2D temp;

	// triple for loop for multiplying matrices
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			temp.m[i][j] = 0;
			for (k = 0; k < 3; ++k) {
				temp.m[i][j] += pMtx0->m[i][k] * pMtx1->m[k][j];
			}
			 
		}
	}

	*pResult = temp;

	//PrintMatrix(pResult, __func__);
}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x & y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);

	pResult->m[0][2] = x;
	pResult->m[1][2] = y;

	//PrintMatrix(pResult, __func__);
}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x & y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);

	pResult->m[0][0] = x;
	pResult->m[1][1] = y;

	//PrintMatrix(pResult, __func__);
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
	Matrix2DRotRad(pResult, (Angle * PI) / 180);
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
	Matrix2DIdentity(pResult);

	pResult->m[0][0] = cosf(Angle);
	pResult->m[0][1] = -sinf(Angle);
	pResult->m[1][0] = -(pResult->m[0][1]);
	pResult->m[1][1] = pResult->m[0][0];

	//PrintMatrix(pResult, __func__);
}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
	Vector2D temp;
	temp.x = (pMtx->m[0][0] * pVec->x) + (pMtx->m[0][1] * pVec->y) + pMtx->m[0][2];
	temp.y = (pMtx->m[1][0] * pVec->x) + (pMtx->m[1][1] * pVec->y) + pMtx->m[1][2];
	*pResult = temp;

	//PrintMatrix(pResult, __func__);
}

// ---------------------------------------------------------------------------
