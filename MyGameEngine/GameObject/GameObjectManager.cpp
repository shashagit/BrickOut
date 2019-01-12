/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: <put file name here>
Purpose: <explain the contents of this file>
Language: <specify language and compiler>
Platform: <specify compiler version, hardware requirements, operating
systems>
Project: <specify student login, class, and assignment. For example:
if foo.boo is in class CS 529 and this file is a part of
assignment 2, then write: CS529_fooboo_2>
Author: <provide your name, student login, and student id>
Creation date: <date on which you created this file>
- End Header --------------------------------------------------------*/

#include "GameObjectManager.h"
#include "GameObject.h"
#include "ObjectFactory.h"

#include "../Components/Transform.h"
#include "../Components/Sprite.h"
#include "../Components/Component.h"
#include "../Components/Controller.h"
#include "../Components/Body.h"
#include "../Components/Enemy.h"
#include "../Components/Brick.h"
#include "../Managers/EventManager.h"
#include "../Managers/Shape.h"

#include <cstdlib>
#include <ctime>

extern ObjectFactory* gpObjectFactory;
extern EventManager* gpEventManager;

GameObjectManager::GameObjectManager() {

	ball = nullptr;
	hitCount = 0;
	spawnTop = false;
	youLose = youWin = false;
	lives = 3;

	// creating map of components 
	componentMap[TRANSFORM] = new Transform();
	componentMap[SPRITE] = new Sprite();
	componentMap[BODY] = new Body();
	componentMap[BRICK] = new Brick();
	componentMap[CONTROLLER] = new Controller();
	componentMap[LEFTRIGHT] = new Enemy();
}

GameObjectManager::~GameObjectManager() {
	for(auto go: mGameObjects) {
		delete go;
	}
}

void GameObjectManager::DeleteGameObject(GameObject* go) {
	for (int i = 0; i < NUM_OF_EVENTS; ++i) {
		gpEventManager->Unsubscribe((EventType)i, go);
	}

	std::vector<GameObject*>::iterator position = std::find(mGameObjects.begin(), mGameObjects.end(), go);
	if (position != mGameObjects.end()) {
		mGameObjects.erase(position);
	}

}

void GameObjectManager::ApplyPowerUp(GameObjectID ID) {
	if (!youWin && !youLose) {
		if (ID == POWERUP) { // grow length of paddles
			for (int i = 0; i < 2; ++i) {
				Body* pB = static_cast<Body*>(paddles[i]->GetComponent(BODY));
				Sprite* pSp = static_cast<Sprite*>(paddles[i]->GetComponent(SPRITE));
				Transform* pTr = static_cast<Transform*>(paddles[i]->GetComponent(TRANSFORM));

				pTr->SetScale(200.0f, pTr->mScale.y);
				ShapeAABB* shape = static_cast<ShapeAABB*>(pB->mpShape);
				shape->mLeft = -100.0f;
				shape->mRight = 100.0f;
				pSp->OverwriteTexFileName("paddle_long");
				std::cout << "Power Up - Length" << std::endl;
			}
		}
		else if (ID == POWERUP2) { // Slow Ball
			std::cout << "Power Up - SlowBall" << std::endl;
			ResetPaddles();
			Body* pB = static_cast<Body*>(ball->GetComponent(BODY));
			Vector2DScale(&pB->mVel, &pB->mVel, 0.6f);
		}
	}
}



void GameObjectManager::Update()
{
	if (!youLose) {
		Body* pB = static_cast<Body*>(ball->GetComponent(BODY));
		if (pB->mPos.y < -SCREEN_HEIGHT / 2 - 25.0f || pB->mPos.y > SCREEN_HEIGHT / 2 + 25.0f) {
			ResetToStart();
		}
	}

	for (auto go : mGameObjects)
		go->Update();
}

void GameObjectManager::CheckWinCondition() {
	if (totalBricks == winningNumOfBricks) {
		std::cout << "Win Condition" << std::endl;
		youWin = true;
		DeleteDropsAndEnemies();
		Body* pB = static_cast<Body*>(ball->GetComponent(BODY));
		Vector2DZero(&pB->mVel);
		gpObjectFactory->LoadObject("Win");
	}
}

void GameObjectManager::ShootBall() {
	Body* pB = static_cast<Body*>(ball->GetComponent(BODY));
	Vector2DSetV(&pB->mVel, &ballStartVelocity);
}

void GameObjectManager::BallOutOfBounds() {
	--lives;
	std::cout << "Lives Left : " << lives << std::endl;
	if (lives < 1) {
		youLose = true;
		std::cout << "Lose Condition" << std::endl;
		gpObjectFactory->LoadObject("GameOver");
	}
}

void GameObjectManager::SpawnDrops(GameObject* mpOwner) {
	++hitCount;
	--totalBricks;

	CheckWinCondition();

	if (youLose || youWin) {
		return;
	}

	// create drop object here
	std::srand(std::time(nullptr)); // use current time as seed for random generator
	int random_variable = std::rand();
	GameObject* go = nullptr;
	if (hitCount % 5 == 0) {
		if (random_variable % 2 == 0) {
			go = gpObjectFactory->LoadObject("Drop1");
		}
		else {
			go = gpObjectFactory->LoadObject("Drop2");
		}

		Transform* pB = static_cast<Transform*>(go->GetComponent(TRANSFORM));
		Transform* pBrick = static_cast<Transform*>(mpOwner->GetComponent(TRANSFORM));
		pB->mPos = pBrick->mPos;
		Brick* pBr = static_cast<Brick*>(go->GetComponent(BRICK));
		pBr->SetMaxHitCount(1);
		Body* pBo = static_cast<Body*>(go->GetComponent(BODY));
		pBo->Initialize();
	}

	SpawnEnemies(random_variable);
}



void GameObjectManager::SpawnEnemies(int random) {
	GameObject* go = nullptr;
	if (hitCount % 2 == 0) {
		if (random % 2 == 0) {
			go = gpObjectFactory->LoadObject("Enemy1");
			Transform* pB = static_cast<Transform*>(go->GetComponent(TRANSFORM));
			spawnTop ? pB->SetPos(SCREEN_WIDTH / 2 - 100.0f, 200.0f) : pB->SetPos(SCREEN_WIDTH/2 - 100.0f, -200.0f);
			spawnTop = !spawnTop;
			Body* pBo = static_cast<Body*>(go->GetComponent(BODY));
			pBo->Initialize();
		}
		else {
			go = gpObjectFactory->LoadObject("Enemy2");
			Transform* pB = static_cast<Transform*>(go->GetComponent(TRANSFORM));
			pB->SetPos(SCREEN_WIDTH/2 - 100.0f, -200.0f);
			Body* pBo = static_cast<Body*>(go->GetComponent(BODY));
			pBo->Initialize();
		}
		Brick* pBr = static_cast<Brick*>(go->GetComponent(BRICK));
		pBr->SetMaxHitCount(1);
	}
}

void GameObjectManager::ResetPaddles() {
	// reset paddles
	for (int i = 0; i < 2; ++i) {
		Body* pB = static_cast<Body*>(paddles[i]->GetComponent(BODY));
		Sprite* pSp = static_cast<Sprite*>(paddles[i]->GetComponent(SPRITE));
		Transform* pTr = static_cast<Transform*>(paddles[i]->GetComponent(TRANSFORM));

		pTr->SetScale(100.0f, pTr->mScale.y);
		ShapeAABB* shape = static_cast<ShapeAABB*>(pB->mpShape);
		shape->mLeft = -50.0f;
		shape->mRight = 50.0f;
		pSp->OverwriteTexFileName("paddle");
	}
}

void GameObjectManager::DeleteDropsAndEnemies() {
	// delete all drops and enemies
	for (auto go : mGameObjects) {
		if (go->ID == POWERUP ||
			go->ID == POWERUP2 ||
			go->ID == ENEMY ||
			go->ID == ENEMY2) {
			DeleteGameObject(go);
		}
	}
}

void GameObjectManager::ResetToStart()
{
	ResetPaddles();

	// reset ball
	Body* pB = static_cast<Body*>(ball->GetComponent(BODY));
	Transform* pTr = static_cast<Transform*>(ball->GetComponent(TRANSFORM));

	pB->isOutOfBounds = false;
	pTr->SetPos(0.0f, -150.0f);
	pB->Initialize();
	Vector2DSet(&pB->mVel, 20.0f, -150.0f);

	DeleteDropsAndEnemies();
}
