/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ResouceManager.cpp
Purpose: Defines the LoadSurface method
Language: C++, MS C++ Compiler
Platform: MS Visual Studio 2017, Windows 10
Project: CS529_shashwatpandey_milestone1
Author: Shashwat Pandey, shashwat.pandey, ID: 60003718
Creation date: 18 October 2018
- End Header --------------------------------------------------------*/

#include "ResourceManager.h"
#include "../Shaders/stb_image.h"

ResourceManager::ResourceManager()
{
	stbi_set_flip_vertically_on_load(true);
}


ResourceManager::~ResourceManager()
{
	for (auto &element : mTextures) {
		stbi_image_free(element.second.textureData); // second is the key which is surface*
	}

	mTextures.clear();
}

Texture ResourceManager::LoadTexture(const char * pFilePath)
{
	Texture pTexture = mTextures[pFilePath];

	if (pTexture.textureData)
		return pTexture;
	
	int width, height, numChannels;
	pTexture.textureData = stbi_load(pFilePath, &width, &height, &numChannels, 4);
	

	if (pTexture.textureData) {
		pTexture.width = width;
		pTexture.height = height;
		mTextures[pFilePath] = pTexture;
	}
	else {
		printf("No Texture Data loaded : Check your file path\n");
	}
	return pTexture;
}
