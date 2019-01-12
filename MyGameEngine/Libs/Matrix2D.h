/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix2D.h
Purpose: Declares all functions for Matrix Operations
Language: C++, MS C++ Compiler
Platform: MS Visual Studio 2017, Windows 10
Project: CS529_shashwatpandey_1
Author: Shashwat Pandey, shashwat.pandey, ID: 60003718
Creation date: 19 September 2018
- End Header --------------------------------------------------------*/


#ifndef MATRIX2D_H
#define MATRIX2D_H


#include "Vector2D.h"
#include <string.h>

typedef struct Matrix2D
{
	float m[3][3];
}Matrix2D;

/*
This function prints a matrix in human readable format
*/
void PrintMatrix(Matrix2D *pResult);

/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult);

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx);

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1);

/*
This function creates a translation matrix from x & y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y);

/*
This function creates a scaling matrix from x & y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y);

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle);

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle);

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec);


#endif