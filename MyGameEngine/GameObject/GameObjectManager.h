#pragma once
#include<vector>
#include<unordered_map>
#include"..\Components\Component.h"
#include "GameObject.h"
extern "C" {
#include "../Libs/Vector2D.h"
}

class GameObjectManager {
	public:
	GameObjectManager();
	~GameObjectManager();
	
	public:
		GameObject* ball;
		std::vector<GameObject*> paddles;
		void DeleteGameObject(GameObject* go);
		void Update();
		void ResetToStart();
		void ResetPaddles();
		void SpawnDrops(GameObject* mpOwner);
		void SpawnEnemies(int);
		void DeleteDropsAndEnemies();
		void ApplyPowerUp(GameObjectID);
		void CheckWinCondition();
		void ShootBall();
		void BallOutOfBounds();

		int lives;
		bool youWin, youLose;
		int hitCount;
		bool spawnTop;
		int totalBricks;
		Vector2D ballStartVelocity;
		int winningNumOfBricks;

	std::vector<GameObject*> mGameObjects;
	std::unordered_map<COMPONENT_TYPE, Component*> componentMap;
};