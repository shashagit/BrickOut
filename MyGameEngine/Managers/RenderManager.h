#pragma once
#include "../Shaders/Shader.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager();

	void Draw();

public:
	bool isDebugMode;
	Shader myShader, debugShader;
};

