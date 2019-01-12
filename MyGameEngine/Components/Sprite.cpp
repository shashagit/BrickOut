#include "Sprite.h"
#include "../Managers/ResourceManager.h"
#include "../Managers/RenderManager.h"
#include "../GameObject/GameObject.h"

extern ResourceManager* gpResourceManager;
extern RenderManager* gpRenderManager;

Sprite::Sprite() : Component(SPRITE) {
	
	glGenBuffers(1, &vertexBuffer);
	glGenVertexArrays(1, &vertexArray);
	glGenBuffers(1, &elementBuffer);
	glGenTextures(1, &mTexture);
}
Sprite::~Sprite() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &vertexArray);
	glDeleteBuffers(1, &elementBuffer);
}

void Sprite::LoadMesh()
{
	// anti-clockwise triangles
	GLuint indices[] = {
		0,1,2,
		2,3,0
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// bind vertex array first
	glBindVertexArray(vertexArray);

	// bind vertex buffer next and set data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// bind element buffer next and set data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set pointers in the data
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(0));
	glEnableVertexAttribArray(0);


	// set pointers in the data
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindTexture(GL_TEXTURE_2D, mTexture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Texture tex = gpResourceManager->LoadTexture(textureFile.c_str());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.textureData);
	
	glBindVertexArray(0);
}

void Sprite::DrawMesh()
{
	if (gpRenderManager->isDebugMode && mpOwner->ID == BALL)
		LoadCircleMesh();
	else
		LoadMesh();
	
	// bind vertex array
	glBindVertexArray(vertexArray);
	
	// the final draw call
	if(!gpRenderManager->isDebugMode)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	else
	{
		if (mpOwner->ID == BALL) {
			glDrawArrays(GL_LINE_LOOP, 0, 16);
		}
		else {
			glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
		}

	}

	// unbind after draw
	glBindVertexArray(0);
}

// Used for bricks only
void Sprite::OverwriteTexFileName(const char* str) {
	std::string file = str;
	textureFile = "..\\Resources\\" + file + ".png";
}

void Sprite::LoadCircleMesh() {
	float circle[] = {
		0.5f, 0.0f,
		0.433f, 0.25f,
		0.353f, 0.353f,
		0.25f, 0.433f,
		0.0f, 0.5f,
		-0.25f, 0.433f,
		-0.353f, 0.353f,
		-0.433f, 0.25f,
		-0.5f, 0.0f,
		-0.433f, -0.25f,
		-0.353f, -0.353f,
		-0.25f, -0.433f,
		0.0f, -0.5f,
		0.25f, -0.433f,
		0.353f, -0.353f,
		0.433f, -0.25f,
		0.5f, 0.0f
	};

		// bind vertex array first
	glBindVertexArray(vertexArray);

	// bind vertex buffer next and set data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

	// set pointers in the data
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)(0));
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
}


void Sprite::Serialize(const char* data)
{
	float quad[] = {
		-0.5f, -0.5f, 0.0f, 0.0f,  // lower-left corner  
		 0.5f, -0.5f, 1.0f, 0.0f,  // lower-right corner
		 0.5f, 0.5f, 1.0f, 1.0f,   // top-left corner
		-0.5f, 0.5f, 0.0f, 1.0f   // top-right corner
	};

	

	for (int i = 0; i < 16; ++i) {
		vertices[i] = quad[i];
	}

	Document d;
	d.Parse(data);

	std::string file = d["fileName"].GetString();
	textureFile = "..\\Resources\\" + file;

}

Sprite * Sprite::Create()
{
	return new Sprite();
}

