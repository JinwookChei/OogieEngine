#pragma once

#include "Transform.h"

class RenderComponent;
class BoundVolume;

class Actor
{
public:
	friend class Level;

	ENGINE_API Actor();

	ENGINE_API virtual ~Actor();

	ENGINE_API virtual void Tick(double deltaTime) = 0;

	ENGINE_API virtual void BeginPlay() = 0;

	ENGINE_API virtual void Render();

	ENGINE_API Transform& GetWorldTransform() const;

	LINK_NODE* LevelLink();

protected:
	ENGINE_API virtual void CleanUp();

protected:
	RenderComponent* pRenderer_;

	Transform* pTransform_;

	BoundVolume* pBoundVolume;

	LINK_NODE levelLink_;
};
