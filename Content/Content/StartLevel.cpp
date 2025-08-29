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
	//Camera* pCamear = SpawnActor<Camera>();
	TestActor* pActor = SpawnActor<TestActor>(ACTOR_TYPE::NORMAL);
}

void StartLevel::Tick(double deltaTime)
{

}
