#include "stdafx.h"
#include "StartLevel.h"
#include "MoveCamera.h"
#include "LookAtCamera.h"
#include "Sphere.h"
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
	pCamera1->GetWorldTransform().SetRotation({ 0.0f, 20.0f, 0.0f, 0.0f });
	pCamera1->GetWorldTransform().SetPosition({ -10.0f, 0.0f, 5.0f, 1.0f});
	

	TestActor* pActor1 = SpawnActor<TestActor>(E_ACTOR_TYPE::NORMAL);
	pActor1->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	pActor1->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//pActor1->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pActor1->GetWorldTransform().AddRotaionZ(90.0f);

	/*DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });*/

	SpotLight* pSpotLight = SpawnLight<SpotLight>();
	pSpotLight->GetWorldTransform().SetPosition({ 1.0f, 1.0f, 5.0f, 1.0f });
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

	Sphere* pSphere1 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	pSphere1->GetWorldTransform().SetPosition({ 10.0f, 0.0f, 3.0f, 1.0f });
	pSphere1->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });

	Sphere* pSphere2 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	pSphere2->GetWorldTransform().SetPosition({ 10.0f, 5.0f, 3.0f, 1.0f });
	pSphere2->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });

	SpotLight* pSpotLight3 = SpawnLight<SpotLight>();
	pSpotLight3->GetWorldTransform().SetPosition({ 6.0f, 0.0f, 3.0f, 1.0f });
	pSpotLight3->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });

	SpotLight* pSpotLight4 = SpawnLight<SpotLight>();
	pSpotLight4->GetWorldTransform().SetPosition({ 10.0f, 0.0f, 6.0f, 1.0f });
	pSpotLight4->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });

	Player* pPlayer = SpawnActor<Player>(E_ACTOR_TYPE::NORMAL);
	pPlayer->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	pPlayer->GetWorldTransform().SetScale({1.0f, 1.0f, 1.0f, 0.0f});
}

void StartLevel::Tick(double deltaTime)
{

}
