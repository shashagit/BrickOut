#include "LevelManager.h"
#include "../GameObject/ObjectFactory.h"

extern ObjectFactory* gpObjectFactory;

LevelManager::LevelManager()
{
}


LevelManager::~LevelManager()
{
}

void LevelManager::LoadLevel() {
	gpObjectFactory->LoadLevel("Level0.json");
	gpObjectFactory->LoadBrickPattern(levelFiles[0].c_str());
}
