#include "stdafx.h"
#include "StartLevel.h"
#include "MoveCamera.h"
#include "Sphere.h"
#include "TestPlayer.h"
#include "TestActor.h"
#include "ParticleActor.h"
#include "WereWolf.h"
#include "Floor.h"
#include "ColorSphere.h"
#include "ColorCube.h"
#include "House.h"

StartLevel::StartLevel()
{
}

StartLevel::~StartLevel()
{
}

void StartLevel::BeginPlay()
{
	Level::BeginPlay();

	TestPlayer* pPlayer = SpawnActor<TestPlayer>(E_ACTOR_TYPE::NORMAL);
	pPlayer->GetWorldTransform().SetPosition({ 1.5f, 0.0f, 0.5f, 1.0f });
	pPlayer->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 180.0f, 0.0f });
	pPlayer->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	WereWolf* pWereWolf = SpawnActor<WereWolf>(E_ACTOR_TYPE::NORMAL);
	pWereWolf->GetWorldTransform().SetPosition({ -3.0f, 0.0f, 0.0f, 1.0f });
	pWereWolf->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });

	House* pHouse = SpawnActor<House>(E_ACTOR_TYPE::NORMAL);
	pHouse->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pHouse->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 89.99f, 0.0f });
	pHouse->GetWorldTransform().SetPosition({ 14.0f, 0.0f, 0.2f, 1.0f });
	Floor* pFloor1 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor1->GetWorldTransform().SetPosition({ 5.0f, -5.0f, -0.5f, 1.0f });
	pFloor1->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Floor* pFloor2 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor2->GetWorldTransform().SetPosition({ 5.0f, 5.0f, -0.5f, 1.0f });
	pFloor2->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Floor* pFloor3 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor3->GetWorldTransform().SetPosition({ -5.0f, 5.0f, -0.5f, 1.0f });
	pFloor3->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Floor* pFloor4 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor4->GetWorldTransform().SetPosition({ -5.0f, -5.0f, -0.5f, 1.0f });
	pFloor4->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Sphere* pSphere1 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	pSphere1->GetWorldTransform().SetPosition({ 5.0f, 10.0f, 3.0f, 1.0f });
	pSphere1->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	Sphere* pSphere2 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	pSphere2->GetWorldTransform().SetPosition({ 0.0f, 10.0f, 3.0f, 1.0f });
	pSphere2->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });

	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({0.0f, 0.0f, 5.0f, 1.0f});
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 45.0f, 0.0f, 0.0f });
	
	SpotLight* pSpotLight = SpawnLight<SpotLight>();
	pSpotLight->GetWorldTransform().SetPosition({ 1.5f, 0.0f, 5.0f, 1.0f });
	pSpotLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	SpotLight* pSpotLight2 = SpawnLight<SpotLight>();
	pSpotLight2->GetWorldTransform().SetPosition({ -3.0f, 0.0f, 3.0f, 1.0f });
	pSpotLight2->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	PointLight* pPointLight = SpawnLight<PointLight>();
	pPointLight->GetWorldTransform().SetPosition({ 0.0f, -3.0f, 2.0f, 1.0f });
	PointLight* pPointLight2 = SpawnLight<PointLight>();
	pPointLight2->GetWorldTransform().SetPosition({ 0.0f, 3.0f, 2.0f, 1.0f });
	PointLight* pPointLight3 = SpawnLight<PointLight>();
	pPointLight3->GetWorldTransform().SetPosition({ 3.0f, 4.0f, 2.0f, 1.0f });

	ParticleActor* pParticleTest1 = SpawnActor<ParticleActor>(E_ACTOR_TYPE::NORMAL);
	pParticleTest1->GetWorldTransform().SetPosition({5.0f, 10.0f, 0.0f, 1.0f});
	ParticleActor* pParticleTest2 = SpawnActor<ParticleActor>(E_ACTOR_TYPE::NORMAL);
	pParticleTest2->GetWorldTransform().SetPosition({0.0f, -10.0f, 0.0f, 1.0f });
}

void StartLevel::Tick(double deltaTime)
{
	Level::Tick(deltaTime);
}
