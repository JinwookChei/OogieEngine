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
	//GetEditorCamera()->GetWorldTransform().SetPosition({ -4.0f, 4.0f, 6.0f, 1.0f });
	//GetEditorCamera()->GetWorldTransform().SetRotation({ 0.0f, 47.0f, -45.0f, 0.0f });

	// Phong Light 2
	//ColorCube* pCube = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCube->GetWorldTransform().SetScale({ 15.0f, 15.0f, 1.0f, 0.0f });
	//pCube->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//pCube->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//ColorCube* pCube1 = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCube1->GetWorldTransform().SetScale({ 15.0f, 15.0f, 1.0f, 0.0f });
	//pCube1->GetWorldTransform().SetRotation({ 90.0f, 0.0f, 0.0f, 0.0f });
	//pCube1->GetWorldTransform().SetPosition({ 0.0f, 7.5f, 7.5f, 1.0f });
	//ColorCube* pCube2 = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCube2->GetWorldTransform().SetScale({ 15.0f, 15.0f, 1.0f, 0.0f });
	//pCube2->GetWorldTransform().SetRotation({ -90.0f, 0.0f, 0.0f, 0.0f });
	//pCube2->GetWorldTransform().SetPosition({ 0.0f, -7.5f, 7.5f, 1.0f });
	//ColorCube* pCube3 = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCube3->GetWorldTransform().SetScale({ 15.0f, 15.0f, 1.0f, 0.0f });
	//pCube3->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	//pCube3->GetWorldTransform().SetPosition({ 7.5f, 0.0f, 7.5f, 1.0f });

	//ColorCube* pCubeA = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCubeA->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//pCubeA->GetWorldTransform().SetPosition({ 0.0f, 5.0f, 0.0f, 1.0f });
	//ColorCube* pCubeB = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCubeB->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//pCubeB->GetWorldTransform().SetPosition({ 0.0f, 5.0f, 0.0f, 1.0f });
	//ColorCube* pCubeC = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	//pCubeC->GetWorldTransform().SetPosition({ 0.0f, 5.0f, 0.0f, 1.0f });
	//pCubeC->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//ColorSphere* pSphereA = SpawnActor<ColorSphere>(E_ACTOR_TYPE::NORMAL);
	//pSphereA->GetWorldTransform().SetPosition({ 0.0f, -5.0f, 0.0f, 1.0f });
	//pSphereA->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//ColorSphere* pSphereB = SpawnActor<ColorSphere>(E_ACTOR_TYPE::NORMAL);
	//pSphereB->GetWorldTransform().SetPosition({ 0.0f, -5.0f, 0.0f, 1.0f });
	//pSphereB->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//ColorSphere* pSphereC = SpawnActor<ColorSphere>(E_ACTOR_TYPE::NORMAL);
	//pSphereC->GetWorldTransform().SetPosition({ 0.0f, -5.0f, 0.0f, 1.0f });
	//pSphereC->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//PointLight* pPointLightA = SpawnLight<PointLight>();
	//pPointLightA->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });
	//pPointLightA->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//PointLight* pPointLightB = SpawnLight<PointLight>();
	//pPointLightB->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });
	//pPointLightB->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//PointLight* pPointLightC = SpawnLight<PointLight>();
	//pPointLightC->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 5.0f, 1.0f });
	//pPointLightC->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });


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
	//pPointLight1->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 3.5f, 1.0f });
	//pPointLight1->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//PointLight* pPointLight2 = SpawnLight<PointLight>();
	//pPointLight2->GetWorldTransform().SetPosition({ -3.5f, 0.0f, 0.0f, 1.0f });
	//pPointLight2->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//PointLight* pPointLight3 = SpawnLight<PointLight>();
	//pPointLight3->GetWorldTransform().SetPosition({ 0.0f, 3.5f, 0.0f, 1.0f });
	//pPointLight3->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
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
	//House* pHouse = SpawnActor<House>(E_ACTOR_TYPE::NORMAL);
	//pHouse->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	//pHouse->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 12.0f, 1.0f });
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	//PointLight* pPointLight1 = SpawnLight<PointLight>();
	//pPointLight1->GetWorldTransform().SetPosition({ 12.0f, 0.0f, 0.0f, 1.0f });
	//PointLight* pPointLight2 = SpawnLight<PointLight>();
	//pPointLight2->GetWorldTransform().SetPosition({ -12.0f, 0.0f, 0.0f, 1.0f });
	//PointLight* pPointLight3 = SpawnLight<PointLight>();
	//pPointLight3->GetWorldTransform().SetPosition({ 0.0f, 8.0f, 0.0f, 1.0f });
	//PointLight* pPointLight4 = SpawnLight<PointLight>();
	//pPointLight4->GetWorldTransform().SetPosition({ 0.0f, -8.0f, 0.0f, 1.0f });

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
	//TestPlayer* pPlayer = SpawnActor<TestPlayer>(E_ACTOR_TYPE::NORMAL);
	//pPlayer->GetWorldTransform().SetPosition({ 1.0f, 3.5f, 0.5f, 1.0f });
	//pPlayer->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 180.0f, 0.0f });
	//pPlayer->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//WereWolf* pWereWolf = SpawnActor<WereWolf>(E_ACTOR_TYPE::NORMAL);
	//pWereWolf->GetWorldTransform().SetPosition({ -15.0f, 3.5f, 0.0f, 1.0f });
	//pWereWolf->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//// Left Trees
	//Tree* pTree = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pTree->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 30.0f, 0.0f });
	//pTree->GetWorldTransform().SetPosition({ 5.0f, -3.0f, 0.0f, 1.0f });
	//Tree* pTree1 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree1->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pTree1->GetWorldTransform().SetRotation({ 0.0f, 0.0f, -60.0f, 0.0f });
	//pTree1->GetWorldTransform().SetPosition({ -2.0f, -6.0f, 0.0f, 1.0f });
	//Tree* pTree2 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree2->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pTree2->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 70.0f, 0.0f });
	//pTree2->GetWorldTransform().SetPosition({ -10.0f, -7.0f, 0.0f, 1.0f });
	//Tree* pTree3 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree3->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pTree3->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 60.0f, 0.0f });
	//pTree3->GetWorldTransform().SetPosition({ -17.0f, -5.0f, 0.0f, 1.0f });
	//// Right Trees
	//Tree* pTree4 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree4->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pTree4->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 30.0f, 0.0f });
	//pTree4->GetWorldTransform().SetPosition({ 4.0f, 12.5f, 0.0f, 1.0f });
	//Tree* pTree5 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree5->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pTree5->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	//pTree5->GetWorldTransform().SetPosition({ -2.0f, 15.0f, 0.0f, 1.0f });
	//Tree* pTree6 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree6->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pTree6->GetWorldTransform().SetRotation({ 0.0f, 0.0f, -30.0f, 0.0f });
	//pTree6->GetWorldTransform().SetPosition({ -10.0f, 17.0f, 0.0f, 1.0f });
	//Tree* pTree7 = SpawnActor<Tree>(E_ACTOR_TYPE::NORMAL);
	//pTree7->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pTree7->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 70.0f, 0.0f });
	//pTree7->GetWorldTransform().SetPosition({ -17.0f, 14.0f, 0.0f, 1.0f });
	//House* pHouse = SpawnActor<House>(E_ACTOR_TYPE::NORMAL);
	//pHouse->GetWorldTransform().SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	//pHouse->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 90.0f, 0.0f });
	//pHouse->GetWorldTransform().SetPosition({ 13.0f, 5.0f, 0.2f, 1.0f });
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
	//Floor* pFloor5 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor5->GetWorldTransform().SetPosition({ -5.0f, 15.0f, -0.5f, 1.0f });
	//pFloor5->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Floor* pFloor6 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor6->GetWorldTransform().SetPosition({ 5.0f, 15.0f, -0.5f, 1.0f });
	//pFloor6->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Floor* pFloor7 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor7->GetWorldTransform().SetPosition({ -15.0f, -5.0f, -0.5f, 1.0f });
	//pFloor7->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Floor* pFloor8 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor8->GetWorldTransform().SetPosition({ -15.0f, 5.0f, -0.5f, 1.0f });
	//pFloor8->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Floor* pFloor9 = SpawnActor<Floor>(E_ACTOR_TYPE::NORMAL);
	//pFloor9->GetWorldTransform().SetPosition({ -15.0f, 15.0f, -0.5f, 1.0f });
	//pFloor9->GetWorldTransform().SetScale({ 10.0f, 10.0f, 1.0f, 0.0f });
	//Sphere* pSphere1 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere1->GetWorldTransform().SetPosition({ 10.5f, 3.4f, 11.0f, 1.0f });
	//pSphere1->GetWorldTransform().SetScale({ 4.0f, 4.0f, 4.0f, 0.0f });
	//ParticleActor* pParticleTest1 = SpawnActor<ParticleActor>(E_ACTOR_TYPE::NORMAL);
	//pParticleTest1->GetWorldTransform().SetPosition({ 4.5f, 10.0f, 0.0f, 1.0f });
	//ParticleActor* pParticleTest2 = SpawnActor<ParticleActor>(E_ACTOR_TYPE::NORMAL);
	//pParticleTest2->GetWorldTransform().SetPosition({ 4.5f, -1.0f, 0.0f, 1.0f });
	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 10.0f, 1.0f });
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 65.0f, 0.0f, 0.0f });
	//pDirectionLight->SetLightIntensity(0.7f);
	//SpotLight* pSpotLight = SpawnLight<SpotLight>();
	//pSpotLight->GetWorldTransform().SetPosition({ 0.0f, 5.0f, 15.0f, 1.0f });
	//pSpotLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	//PointLight* pPointLight = SpawnLight<PointLight>();
	//pPointLight->GetWorldTransform().SetPosition({ -3.0f, -3.0f, 3.0f, 1.0f });
	//PointLight* pPointLight2 = SpawnLight<PointLight>();
	//pPointLight2->GetWorldTransform().SetPosition({ -3.0f, 10.0f, 3.0f, 1.0f });
	//PointLight* pPointLight3 = SpawnLight<PointLight>();
	//pPointLight3->GetWorldTransform().SetPosition({ 5.8f, 3.4f, 11.0f, 1.0f });


	// DebugRendering, Object Picking
	//DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	//pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 10.0f, 1.0f });
	//pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 65.0f, 0.0f, 0.0f });
	//pDirectionLight->SetLightIntensity(0.7f);
	//Sphere* pSphere1 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere1->GetWorldTransform().SetPosition({ -3.5f, 3.4f, 11.0f, 1.0f });
	//pSphere1->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//Sphere* pSphere2 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere2->GetWorldTransform().SetPosition({ 3.5f, 6.4f, -10.0f, 1.0f });
	//pSphere2->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//Sphere* pSphere3 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere3->GetWorldTransform().SetPosition({ -7.5f, 1.4f, -3.0f, 1.0f });
	//pSphere3->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
	//Sphere* pSphere4 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere4->GetWorldTransform().SetPosition({ 9.5f, 6.0f, 2.0f, 1.0f });
	//pSphere4->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });	
	//Sphere* pSphere5 = SpawnActor<Sphere>(E_ACTOR_TYPE::NORMAL);
	//pSphere5->GetWorldTransform().SetPosition({ 2.5f, 7.4f, 7.0f, 1.0f });
	//pSphere5->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });


	// Phong Light 2
	float maxR = 50.0f;
	float halfR = 25.0f;
	ColorCube* pCube = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	pCube->GetWorldTransform().SetScale({ maxR, maxR, 1.0f, 0.0f });
	pCube->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pCube->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	ColorCube* pCube1 = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	pCube1->GetWorldTransform().SetScale({ maxR, maxR, 1.0f, 0.0f });
	pCube1->GetWorldTransform().SetRotation({ 90.0f, 0.0f, 0.0f, 0.0f });
	pCube1->GetWorldTransform().SetPosition({ 0.0f, halfR, halfR, 1.0f });
	ColorCube* pCube2 = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	pCube2->GetWorldTransform().SetScale({ maxR, maxR, 1.0f, 0.0f });
	pCube2->GetWorldTransform().SetRotation({ -90.0f, 0.0f, 0.0f, 0.0f });
	pCube2->GetWorldTransform().SetPosition({ 0.0f, -halfR, halfR, 1.0f });
	ColorCube* pCube3 = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
	pCube3->GetWorldTransform().SetScale({ maxR, maxR, 1.0f, 0.0f });
	pCube3->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
	pCube3->GetWorldTransform().SetPosition({ halfR, 0.0f, halfR, 1.0f });
	for (int i = 0; i < 100; ++i)
	{
		// -25.0f ~ 25.0f 사이의 랜덤 좌표 계산
		float randomX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR - halfR;
		float randomY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR - halfR;
		float randomZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR;

		// 1. 액터 스폰
		ColorCube* pNewCube = SpawnActor<ColorCube>(E_ACTOR_TYPE::NORMAL);
		if (pNewCube)
		{
			pNewCube->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
			pNewCube->GetWorldTransform().SetPosition({ randomX, randomY, randomZ, 1.0f });
		}
	}

	for (int i = 0; i < 100; ++i)
	{
		// -25.0f ~ 25.0f 사이의 랜덤 좌표 계산
		float randomX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR - halfR;
		float randomY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR - halfR;
		float randomZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR;

		// 1. 액터 스폰
		ColorSphere* pNewSphere = SpawnActor<ColorSphere>(E_ACTOR_TYPE::NORMAL);
		if (pNewSphere)
		{
			pNewSphere->GetWorldTransform().SetScale({ 3.0f, 3.0f, 3.0f, 0.0f });
			pNewSphere->GetWorldTransform().SetPosition({ randomX, randomY, randomZ, 1.0f });
		}
	}
	for (int i = 0; i < 100; ++i)
	{
		float randomX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR - halfR;
		float randomY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR - halfR;
		float randomZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * maxR;
		float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		PointLight* pNewLight = SpawnLight<PointLight>();
		if (pNewLight)
		{
			pNewLight->SetDiffuseColorForEditor({ r, g, b, 1.0f });
			pNewLight->SetLightRange(maxR);
			pNewLight->GetWorldTransform().SetPosition({ randomX, randomY, randomZ, 1.0f });
			pNewLight->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
		}
	}
	GetEditorCamera()->GetWorldTransform().SetPosition({ -40.0f, 0.0f, 5.0f, 1.0f });
	GetEditorCamera()->GetWorldTransform().SetRotation({ 0.0f, -20.0f, 0.0f, 1.0f });
}

void PortfolioLevel::Tick(double deltaTime)
{
	Level::Tick(deltaTime);
}
