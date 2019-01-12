#include "ObjectFactory.h"
#include "../Components/Transform.h"
#include "../Components/Sprite.h"
#include "../Components/Component.h"
#include "../Components/Controller.h"
#include "../Components\Body.h"
#include "../Components\Brick.h"

#include "../Managers/EventManager.h"
#include "../Managers/LevelManager.h"

#include "GameObjectManager.h"
#include "GameObject.h"

extern GameObjectManager* gpGoManager;
extern EventManager* gpEventManager;
extern LevelManager* gpLevelManager;


ObjectFactory::ObjectFactory()
{
}


ObjectFactory::~ObjectFactory()
{
}

void ObjectFactory::LoadLevel(const char * pFileName)
{
	std::string fullPath = "..\\Resources\\";
	fullPath += pFileName;

	std::ifstream file(fullPath);
	std::stringstream ss;
	ss << file.rdbuf();
	std::string temp = ss.str();
	const char* str = temp.c_str();

	Document document;
	document.Parse(str);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);

	const rapidjson::Value& V = document;
	for (Value::ConstMemberIterator iter = V.MemberBegin(); iter != V.MemberEnd(); ++iter) {
		if (iter->value.IsObject()) {// means it refers to an object
			GameObject* pGO = LoadObject(iter->name.GetString());
			// set position according to data in level file
			Transform* pTr = static_cast<Transform*>(pGO->GetComponent(TRANSFORM));
			if (pTr) {
				writer.Reset(buffer);
				iter->value.Accept(writer);
				pTr->Serialize(buffer.GetString());
			}
			buffer.Clear();

			Body* pBr = static_cast<Body*>(pGO->GetComponent(BODY));
			if (pBr != nullptr)
				pBr->Initialize();
		}
		else { // means it refers to raw data
			const rapidjson::Value& V = iter->value;
			for (Value::ConstValueIterator iter = V.Begin(); iter != V.End(); ++iter) {
				gpLevelManager->levelFiles.push_back(iter->GetString());
			}
		}


	}
}

void ObjectFactory::LoadBrickPattern(const char* pFileName) {

	std::string fullPath = "..\\Resources\\";
	fullPath = fullPath + pFileName;

	std::ifstream file(fullPath);
	std::stringstream ss;
	ss << file.rdbuf();
	std::string temp = ss.str();
	const char* str = temp.c_str();

	Document document;
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	document.Parse(str);

	int rows = document["Rows"].GetInt();
	int cols = document["Columns"].GetInt();
	Value arr = document["Pattern"].GetArray();
	
	for (int i = 0; i < rows; ++i) {// 4
		for (int j = 0; j < cols; ++j) {//8
			GameObject* go = LoadBrick(arr[(i*cols)+j].GetInt(), i, j);
		}
	}
}

GameObject* ObjectFactory::LoadBrick(int index, int i, int j) {
	
	GameObject* go = LoadObject("Brick");

	Transform* pTr = static_cast<Transform*>(go->GetComponent(TRANSFORM));
	pTr->SetPos(-210.0f + j * 60.0f, -60.0f + i * 40.0f); // for given brick size

	Sprite* pSp = static_cast<Sprite*>(go->GetComponent(SPRITE));
	std::string texFile = "brick" + std::to_string(index);
	pSp->OverwriteTexFileName(texFile.c_str());

	Brick* pBr = static_cast<Brick*>(go->GetComponent(BRICK));
	pBr->SetMaxHitCount(index); // also subscribes to event

	gpGoManager->totalBricks++;
	if (index > 2) {
		gpGoManager->winningNumOfBricks++;
	}

	Body* pBo = static_cast<Body*>(go->GetComponent(BODY));
	if (pBo != nullptr)
		pBo->Initialize();

	return go;
}

GameObject* ObjectFactory::LoadObject(const char * pFileName)
{
	std::string fullPath = "..\\Resources\\";
	fullPath = fullPath + pFileName + ".json";

	std::ifstream file(fullPath);
	std::stringstream ss;
	ss << file.rdbuf();
	std::string temp = ss.str();
	const char* str = temp.c_str();

	return CreateObject(str, pFileName);
}

GameObject* ObjectFactory::CreateObject(const char * str, const char * pFileName) {
	GameObject* pNewGameObject = nullptr;
	Document document;
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	document.Parse(str);
	
	//std::cout << str << std::endl;

	assert(document.IsObject());
	pNewGameObject = new GameObject();
	
	pNewGameObject->ID = (GameObjectID)document["ID"].GetInt();

	// set IDs for all gameobjects
	std::string objName = pFileName;
	if ("Paddle" == objName) {
		gpGoManager->paddles.push_back(pNewGameObject);
	}
	else if ("Ball" == objName) {
		gpGoManager->ball = pNewGameObject;
		gpEventManager->Subscribe(OUT_OF_BOUNDS, gpGoManager->ball);
	}
	
	const rapidjson::Value& V = document;
	for (Value::ConstMemberIterator iter = V.MemberBegin()+1; iter != V.MemberEnd(); ++iter) {
		
		std::string componentName = iter->name.GetString();


		Component *pNewComponent = nullptr;
		if ("Transform" == componentName) {
			pNewComponent = pNewGameObject->AddComponent(TRANSFORM);
		}
		else if ("Sprite" == componentName) {
			pNewComponent = pNewGameObject->AddComponent(SPRITE);
		}
		else if ("Controller" == componentName) {
			pNewComponent = pNewGameObject->AddComponent(CONTROLLER);
		}
		else if ("Body" == componentName) {
			pNewComponent = pNewGameObject->AddComponent(BODY);
		}
		else if ("Brick" == componentName) {
			pNewComponent = pNewGameObject->AddComponent(BRICK);
		}
		else if ("Enemy" == componentName) {
			pNewComponent = pNewGameObject->AddComponent(LEFTRIGHT);
		}

		if (pNewComponent != nullptr) {
			writer.Reset(buffer);
			iter->value.Accept(writer);
			pNewComponent->Serialize(buffer.GetString());
		}
		buffer.Clear();
	}
		
	gpGoManager->mGameObjects.push_back(pNewGameObject);
		
	return pNewGameObject;
}
