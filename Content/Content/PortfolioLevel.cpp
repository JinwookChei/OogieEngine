#include "stdafx.h"
#include "PortfolioLevel.h"
#include "ColorSphere.h"
#include "ColorCube.h"

PortfolioLevel::PortfolioLevel()
{
}

PortfolioLevel::~PortfolioLevel()
{
}

void PortfolioLevel::BeginPlay()
{
	Level::BeginPlay();

	ColorSphere* pSphere = SpawnActor<ColorSphere>(E_ACTOR_TYPE::NORMAL);
	pSphere->GetWorldTransform().SetScale({ 5.0f, 5.0f, 5.0f, 0.0f });
	pSphere->GetWorldTransform().SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pSphere->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
	
	DirectionLight* pDirectionLight = SpawnLight<DirectionLight>();
	pDirectionLight->GetWorldTransform().SetPosition({ 0.0f, 0.0f, 4.0f, 1.0f });
	pDirectionLight->GetWorldTransform().SetRotation({ 0.0f, 90.0f, 0.0f, 0.0f });
}

void PortfolioLevel::Tick(double deltaTime)
{
	Level::Tick(deltaTime);
}
