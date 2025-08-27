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
	TestActor* pActor = SpawnActor<TestActor>();
}

void StartLevel::Tick(double deltaTime)
{

}
