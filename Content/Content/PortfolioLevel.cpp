#include "stdafx.h"
#include "PortfolioLevel.h"
#include "ColorSphere.h"
#include "ColorCube.h"
#include "BricksCube.h"
#include "Floor.h"
#include "Sphere.h"
#include "House.h"
#include "Tree.h"
#include "WereWolf.h"
#include "ParticleActor.h"
#include "TestPlayer.h"

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
	/*House* pHouse = SpawnActor<House>(E_ACTOR_TYPE::NORMAL);
	pHouse->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	pHouse->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 12.0f, 1.0f });
	pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	PointLight* pPointLight1 = SpawnLight<PointLight>();
	pPointLight1->GetWorldTransform().SetPosition({ 12.0f, 0.0f, 0.0f, 1.0f });
	PointLight* pPointLight2 = SpawnLight<PointLight>();
	pPointLight2->GetWorldTransform().SetPosition({ -12.0f, 0.0f, 0.0f, 1.0f });
	PointLight* pPointLight3 = SpawnLight<PointLight>();
	pPointLight3->GetWorldTransform().SetPosition({ 0.0f, 8.0f, 0.0f, 1.0f });
	PointLight* pPointLight4 = SpawnLight<PointLight>();
	pPointLight4->GetWorldTransform().SetPosition({ 0.0f, -8.0f, 0.0f, 1.0f });*/

	// StaticMesh - Tree
	//Tree* pTree = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//pTree->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 12.0f, 1.0f });
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	//PointLight* pPointLight1 = SpawnLight<PointLight>();
	//pPointLight1->GetWorldTransform().SetPosition({ 7.0f, 0.0f, 8.0f, 1.0f });
	//PointLight* pPointLight2 = SpawnLight<PointLight>();
	//pPointLight2->GetWorldTransform().SetPosition({ -7.0f, 0.0f, 8.0f, 1.0f });
	//PointLight* pPointLight3 = SpawnLight<PointLight>();
	//pPointLight3->GetWorldTransform().SetPosition({ 0.0f, 7.0f, 8.0f, 1.0f });
	//PointLight* pPointLight4 = SpawnLight<PointLight>();
	//pPointLight4->GetWorldTransform().SetPosition({ 0.0f, -7.0f, 8.0f, 1.0f });

	// SkeletalMesh - WereWolf
	//WereWolf* pWereWolf = SpawnActor<WereWolf>(E_ACTOR_TYPE::NORMAL);
	//pWereWolf->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//pWereWolf->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 8.0f, 1.0f });
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	//PointLight* pPointLight1 = SpawnLight<PointLight>();
	//pPointLight1->GetWorldTransform().SetPosition({ 5.0f, 0.0f, 4.0f, 1.0f });
	//PointLight* pPointLight2 = SpawnLight<PointLight>();
	//pPointLight2->GetWorldTransform().SetPosition({ -5.0f, 0.0f, 4.0f, 1.0f });
	//PointLight* pPointLight3 = SpawnLight<PointLight>();
	//pPointLight3->GetWorldTransform().SetPosition({ 0.0f, 5.0f, 4.0f, 1.0f });
	//PointLight* pPointLight4 = SpawnLight<PointLight>();
	//pPointLight4->GetWorldTransform().SetPosition({ 0.0f, -5.0f, 4.0f, 1.0f });

	// Particle
	//ParticleActor* pParticle = SpawnActor<ParticleActor>(E_ACTOR_TYPE::NORMAL);
	//pParticle->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//pParticle->GetWorldTransform().SetScale({ 2.0f, 2.0f, 2.0f, 0.0f });
	//DirectionLight * pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 10.0f, 1.0f });
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });

	// Deferred Rendering
	TestPlayer* pPlayer = SpawnActor<TestPlayer>(E_ACTOR_TYPE::NORMAL);
	pPlayer->GetWorldTransform().SetPosition({ 1.0f, 3.5f, 0.5f, 1.0f });
	pPlayer->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 180.0f, 0.0f });
	pPlayer->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	WereWolf* pWereWolf = SpawnActor<WereWolf>(E_ACTOR_TYPE::NORMAL);
	pWereWolf->GetWorldTransform().SetPosition({ -15.0f, 3.5f, 0.0f, 1.0f });
	pWereWolf->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });

	// Left Trees
	Tree* pTree = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	pTree->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTree->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 30.0f, 0.0f });
	pTree->GetWorldTransform().SetPosition({ 5.0f, -3.0f, 0.0f, 1.0f });
	Tree* pTree1 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	pTree1->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTree1->GetWorldTransform().SetRotation({ 0.0f, 0.0f, -60.0f, 0.0f });
	pTree1->GetWorldTransform().SetPosition({ -2.0f, -6.0f, 0.0f, 1.0f });
	Tree* pTree2 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	pTree2->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTree2->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 70.0f, 0.0f });
	pTree2->GetWorldTransform().SetPosition({ -10.0f, -7.0f, 0.0f, 1.0f });
	Tree* pTree3 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	pTree3->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTree3->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 60.0f, 0.0f });
	pTree3->GetWorldTransform().SetPosition({ -17.0f, -5.0f, 0.0f, 1.0f });
	// Right Trees
	Tree* pTree4 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	pTree4->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTree4->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 30.0f, 0.0f });
	pTree4->GetWorldTransform().SetPosition({ 4.0f, 12.5f, 0.0f, 1.0f });
	Tree* pTree5 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	pTree5->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTree5->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTree5->GetWorldTransform().SetPosition({ -2.0f, 15.0f, 0.0f, 1.0f });
	Tree* pTree6 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	pTree6->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTree6->GetWorldTransform().SetRotation({ 0.0f, 0.0f, -30.0f, 0.0f });
	pTree6->GetWorldTransform().SetPosition({ -10.0f, 17.0f, 0.0f, 1.0f });
	Tree* pTree7 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	pTree7->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTree7->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 70.0f, 0.0f });
	pTree7->GetWorldTransform().SetPosition({ -17.0f, 14.0f, 0.0f, 1.0f });
	House* pHouse = SpawnActor<House>(E_ACTOR_TYPE::NORMAL);
	pHouse->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pHouse->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 90.0f, 0.0f });
	pHouse->GetWorldTransform().SetPosition({ 13.0f, 5.0f, 0.2f, 1.0f });
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
	Floor* pFloor5 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor5->GetWorldTransform().SetPosition({ -5.0f, 15.0f, -0.5f, 1.0f });
	pFloor5->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Floor* pFloor6 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor6->GetWorldTransform().SetPosition({ 5.0f, 15.0f, -0.5f, 1.0f });
	pFloor6->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Floor* pFloor7 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor7->GetWorldTransform().SetPosition({ -15.0f, -5.0f, -0.5f, 1.0f });
	pFloor7->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Floor* pFloor8 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor8->GetWorldTransform().SetPosition({ -15.0f, 5.0f, -0.5f, 1.0f });
	pFloor8->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Floor* pFloor9 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	pFloor9->GetWorldTransform().SetPosition({ -15.0f, 15.0f, -0.5f, 1.0f });
	pFloor9->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	Sphere* pSphere1 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	pSphere1->GetWorldTransform().SetPosition({ 10.5f, 3.4f, 11.0f, 1.0f });
	pSphere1->GetWorldTransform().SetScale({ 4.0f, 4.0f, 4.0f, 0.0f });
	ParticleActor* pParticleTest1 = SpawnActor<ParticleActor>(E_ACTOR_TYPE::NORMAL);
	pParticleTest1->GetWorldTransform().SetPosition({ 4.5f, 10.0f, 0.0f, 1.0f });
	ParticleActor* pParticleTest2 = SpawnActor<ParticleActor>(E_ACTOR_TYPE::NORMAL);
	pParticleTest2->GetWorldTransform().SetPosition({ 4.5f, -1.0f, 0.0f, 1.0f });

	DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 10.0f, 1.0f });
	pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 65.0f, 0.0f, 0.0f });
	pDirectionLight->SetLightIntensity(0.7f);
	SpotLight* pSpotLight = SpawnLight<SpotLight>();
	pSpotLight->GetWorldTransform().SetPosition({ 0.0f, 5.0f, 15.0f, 1.0f });
	pSpotLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });

	PointLight* pPointLight = SpawnLight<PointLight>();
	pPointLight->GetWorldTransform().SetPosition({ -3.0f, -3.0f, 3.0f, 1.0f });
	PointLight* pPointLight2 = SpawnLight<PointLight>();
	pPointLight2->GetWorldTransform().SetPosition({ -3.0f, 10.0f, 3.0f, 1.0f });
	PointLight* pPointLight3 = SpawnLight<PointLight>();
	pPointLight3->GetWorldTransform().SetPosition({ 5.8f, 3.4f, 11.0f, 1.0f });

	//PointLight* pPointLight5 = SpawnLight<PointLight>();
	//pPointLight5->GetWorldTransform().SetPosition({ 0.0f, 1.0f, 3.0f, 1.0f });
	//PointLight* pPointLight7 = SpawnLight<PointLight>();
	//pPointLight7->GetWorldTransform().SetPosition({ -7.0f, 8.0f, 3.0f, 1.0f });
	//PointLight* pPointLight9 = SpawnLight<PointLight>();
	//pPointLight9->GetWorldTransform().SetPosition({ 7.0f, 8.0f, 3.0f, 1.0f });
}

void PortfolioLevel::Tick(double deltaTime)
{
	Level::Tick(deltaTime);
}
