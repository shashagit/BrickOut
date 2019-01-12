#include "RenderManager.h"
#include "../GameObject/GameObjectManager.h"
#include "../Components/Transform.h"
#include "../Components/Sprite.h"
#include "../GameObject/GameObject.h"

extern GameObjectManager* gpGoManager;

RenderManager::RenderManager() : isDebugMode(false)
{
	glewInit();
	glewExperimental = GL_TRUE;

	// this shader draws everything
	debugShader = Shader("Shaders/vertexDebug.vs", "Shaders/fragmentDebug.fs");
	myShader = Shader("Shaders/vertex.vs", "Shaders/fragment.fs");
}


RenderManager::~RenderManager()
{
}

void RenderManager::Draw()
{
	//Clearing the buffer
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// use shader program to draw what we have set up
	if (!isDebugMode) {
		myShader.use();
		//printf("shader ID : %d\n", myShader.ID);
	}
	else {
		// use Debug Shader
		debugShader.use();
		//printf("shader ID : %d\n", debugShader.ID);
	}

	// Draw all objects
	for (auto go : gpGoManager->mGameObjects) {
		Sprite* pSp = static_cast<Sprite*>(go->GetComponent(SPRITE));
		Transform* pTr = static_cast<Transform*>(go->GetComponent(TRANSFORM));
		if (!isDebugMode)
			myShader.setTransform("transform", pTr->GetTransformMatrix());
		else
			debugShader.setTransform("transformx", pTr->GetTransformMatrix());
		pSp->DrawMesh();
	}

}


