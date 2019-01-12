/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.h
Purpose: Declares the Resourcemanager class
Language: C++, MS C++ Compiler
Platform: MS Visual Studio 2017, Windows 10
Project: CS529_shashwatpandey_milestone1
Author: Shashwat Pandey, shashwat.pandey, ID: 60003718
Creation date: 18 October 2018
- End Header --------------------------------------------------------*/

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>

class Texture {
public:
	Texture() {
		textureData = nullptr;
		width = 0;
		height = 0;
	}

	unsigned char* textureData;
	int width;
	int height;
};

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	Texture LoadTexture(const char *pFilePath);

private:
	std::unordered_map<std::string, Texture> mTextures;
};

#endif
