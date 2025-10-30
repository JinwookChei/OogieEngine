#include "stdafx.h"
#include "StartLevel.h"
#include "MoveCamera.h"
#include "LookAtCamera.h"
#include "TestActor.h"

StartLevel::StartLevel()
{
}

StartLevel::~StartLevel()
{
}

void StartLevel::BeginPlay()
{
	MoveCamera* pCamera1 = SpawnCamera<MoveCamera>();
	pCamera1->SetScreenPlacement({ 0.0f, 0.0f }, {1.0f, 1.0f});

	//pCamera1->GetWorldTransform().AddRotaionY(50);
	//pCamera1->GetWorldTransform().SetPosition({0.0f, 0.0f, 0.0f, 1.0f});
	

	//MoveCamera* pCamera2 = SpawnCamera<MoveCamera>();
	//pCamera2->SetClearColor({ 0.0f, 0.0f, 0.6f, 1.0f });
	//pCamera2->SetScreenPlacement({ -0.5f, 0.0f }, { 0.5f, 1.0f });
	//pCamera2->GetWorldTransform().AddPosition({0.0f, 0.0f, 0.0f,0.0f});
	//pCamera2->SetScreenPlacement({ 0.5f, 0.0f }, { 0.5f, 1.0f });

	/*LookAtCamera* pCamera2 = SpawnCamera<LookAtCamera>();
	pCamera2->SetClearColor({ 0.0f, 0.0f, 0.6f, 1.0f });
	pCamera2->SetScreenPlacement({ 0.5f, 0.0f }, { 0.5f, 1.0f });
	pCamera2->SetFocusActor(pCamera1);*/
	//pCamera2->GetWorldTransform().SetPosition({ 0.0f, 3.0f, 0.0f, 1.0f });



	TestActor* pActor1 = SpawnActor<TestActor>(ACTOR_TYPE::NORMAL);
	pActor1->GetWorldTransform().SetPosition({0.0f, 0.0f, 0.0f, 1.0f});

	TestActor* pActor2 = SpawnActor<TestActor>(ACTOR_TYPE::NORMAL);
	pActor2->GetWorldTransform().SetPosition({0.0f, -2.0f, 0.0f, 1.0f });

	
	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetRotation({0.0f, 0.0f, 0.0f, 0.0f});

	//SpotLight* pSpotLight = SpawnLight<SpotLight>();
	//pSpotLight->GetWorldTransform().SetPosition({0.0f, 0.0f, 1.0f, 1.0f});
	//pSpotLight->GetWorldTransform().SetRotation({0.0f, 90.0f, 0.0f, 0.0f});

	PointLight* pPointLight = SpawnLight<PointLight>();
	pPointLight->GetWorldTransform().SetPosition({ 0.0f, -1.0f, 0.0f, 1.0f });

}

void StartLevel::Tick(double deltaTime)
{

}
