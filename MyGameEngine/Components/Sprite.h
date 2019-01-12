#pragma once

#include "Component.h"
#include "GL/glew.h"
#include "../Shaders/Shader.h"

#define MAX_VERTICES 512

class Sprite : public Component {
	public:
	Sprite();
	~Sprite();
	
	void LoadMesh();
	void DrawMesh();

	void Serialize(const char* data);

	Sprite* Create();
	void Update() {}

	void LoadCircleMesh();
	void OverwriteTexFileName(const char*);

	private:
	GLuint vertexBuffer, vertexArray, mTexture, elementBuffer;
	GLfloat vertices[MAX_VERTICES];
	std::string textureFile;

};