#pragma once
#include <vector>

class LevelManager
{
public:
	LevelManager();
	~LevelManager();
	void LoadLevel();
public:
	std::vector<std::string> levelFiles;
};

