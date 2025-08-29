#include "stdafx.h"
#include "StartLevel.h"
#include "TestActor.h"

StartLevel::StartLevel()
{
}

StartLevel::~StartLevel()
{
}

void StartLevel::BeginPlay()
{
	TestActor* pActor1 = SpawnActor<TestActor>(ACTOR_TYPE::NORMAL);
	pActor1->GetWorldTransform().SetPosition({0.0f, 2.0f, 0.0f, 1.0f});

	TestActor* pActor2 = SpawnActor<TestActor>(ACTOR_TYPE::CAMERA);
	pActor2->GetWorldTransform().SetPosition({2.0f, 0.0f, 2.0f, 1.0f });
}

void StartLevel::Tick(double deltaTime)
{

}
