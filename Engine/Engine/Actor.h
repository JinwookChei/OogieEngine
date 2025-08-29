#pragma once

#include "Transform.h"

class RenderComponent;

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

	LINK_ITEM* LevelLink();

protected:
	ENGINE_API virtual void CleanUp();

protected:
	Transform* pTransform_;

	//RenderComponent* pRenderComponent_;

	LINK_ITEM levelLink_;
};
