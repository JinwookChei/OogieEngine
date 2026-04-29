#include "stdafx.h"
#include "PortfolioLevel.h"
#include "ColorSphere.h"
#include "ColorCube.h"
#include "BricksCube.h"
#include "Floor.h"
#include "Sphere.h"
#include "House.h"

PortfolioLevel::PortfolioLevel()
{
}

PortfolioLevel::~PortfolioLevel()
{
}

void PortfolioLevel::BeginPlay()
{
	Level::BeginPlay();

	// Phong Light
	//ColorSphere* pSphere = SpawnActor<ColorSphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere->GetWorldTransform().SetScale({ 5.0f, 5.0f, 5.0f, 0.0f });
	//pSphere->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//pSphere->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 4.0f, 1.0f });
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });

	// Direction Light
	//ColorCube* pCube = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCube->GetWorldTransform().SetScale({ 10.0f, 10.0f, 3.0f, 0.0f });
	//pCube->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//pCube->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });

	// SpotLight
	//ColorCube* pCube = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCube->GetWorldTransform().SetScale({ 10.0f, 10.0f, 3.0f, 0.0f });
	//pCube->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//pCube->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//SpotLight* pSpotLight = SpawnLight<SpotLight>();
	//pSpotLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });
	//pSpotLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });

	// PointLight
	/*ColorCube* pCube = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	pCube->GetWorldTransform().SetScale({ 10.0f, 10.0f, 3.0f, 0.0f });
	pCube->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pCube->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	PointLight* pPointLight = SpawnLight<PointLight>();
	pPointLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });
	pPointLight->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });*/


	// Normal Mapping
	//BricksCube* pBricksCube = SpawnActor<BricksCube>(E_ACTOR_TYPE::NORMAL);
	//pBricksCube->GetWorldTransform().SetScale({ 5.0f, 5.0f, 5.0f, 0.0f });
	//pBricksCube->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//pBricksCube->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//PointLight* pPointLight1 = SpawnLight<PointLight>();
	//pPointLight1->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 3.0f, 1.0f });
	//pPointLight1->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//PointLight* pPointLight2 = SpawnLight<PointLight>();
	//pPointLight2->GetWorldTransform().SetPosition({ -3.0f, 0.0f, 0.0f, 1.0f });
	//pPointLight2->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//GetEditorCamera()->GetWorldTransform().SetPosition({ -4.0f, 4.0f, 6.0f, 1.0f });
	//GetEditorCamera()->GetWorldTransform().SetRotation({ 0.0f, 47.0f, -45.0f, 0.0f });


	// Deferred Rendering
	//Floor* pFloor1 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor1->GetWorldTransform().SetPosition({ 5.0f, -5.0f, -0.5f, 1.0f });
	//pFloor1->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Floor* pFloor2 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor2->GetWorldTransform().SetPosition({ 5.0f, 5.0f, -0.5f, 1.0f });
	//pFloor2->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Floor* pFloor3 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor3->GetWorldTransform().SetPosition({ -5.0f, 5.0f, -0.5f, 1.0f });
	//pFloor3->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Floor* pFloor4 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor4->GetWorldTransform().SetPosition({ -5.0f, -5.0f, -0.5f, 1.0f });
	//pFloor4->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Sphere* pSphere1 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere1->GetWorldTransform().SetPosition({ 10.0f, -5.0f, 3.0f, 1.0f });
	//pSphere1->GetWorldTransform().SetScale({ 4.0f, 4.0f, 3.0f, 0.0f });
	//Sphere* pSphere2 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere2->GetWorldTransform().SetPosition({ 10.0f, 5.0f, 3.0f, 1.0f });
	//pSphere2->GetWorldTransform().SetScale({ 4.0f, 4.0f, 3.0f, 0.0f });
	//PointLight* pPointLight1 = SpawnLight<PointLight>();
	//pPointLight1->GetWorldTransform().SetPosition({ 5.0f, -5.0f, 1.5f, 1.0f });
	//PointLight* pPointLight2 = SpawnLight<PointLight>();
	//pPointLight2->GetWorldTransform().SetPosition({ 5.0f, 5.0f, 1.5f, 1.0f });
	//PointLight* pPointLight3 = SpawnLight<PointLight>();
	//pPointLight3->GetWorldTransform().SetPosition({ -5.0f, 5.0f, 1.5f, 1.0f });
	//PointLight* pPointLight4 = SpawnLight<PointLight>();
	//pPointLight4->GetWorldTransform().SetPosition({ -5.0f, -5.0f, 1.5f, 1.0f });
	//SpotLight* pSpotLight = SpawnLight<SpotLight>();
	//pSpotLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });
	//pSpotLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });


	// StaticMesh - House
	House* pHouse = SpawnActor<House>(E_ACTOR_TYPE::NORMAL);
	pHouse->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	pHouse->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 12.0f, 1.0f });
	pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	PointLight* pPointLight1 = SpawnLight<PointLight>();
	pPointLight1->GetWorldTransform().SetPosition({ 7.0f, 0.0f, 0.0f, 1.0f });
	PointLight* pPointLight2 = SpawnLight<PointLight>();
	pPointLight2->GetWorldTransform().SetPosition({ -7.0f, 0.0f, 0.0f, 1.0f });
	PointLight* pPointLight3 = SpawnLight<PointLight>();
	pPointLight3->GetWorldTransform().SetPosition({ 0.0f, 7.0f, 0.0f, 1.0f });
	PointLight* pPointLight4 = SpawnLight<PointLight>();
	pPointLight4->GetWorldTransform().SetPosition({ 0.0f, -7.0f, 0.0f, 1.0f });
}

void PortfolioLevel::Tick(double deltaTime)
{
	Level::Tick(deltaTime);
}
