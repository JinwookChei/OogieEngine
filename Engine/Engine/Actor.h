#pragma once

class RenderComponent;
class BoundVolume;

class Actor
	: public IImGuiBindPickedActor
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

	BoundVolume* GetBoundVolume() const;

	RenderComponent* GetRenderComponent() const;

protected:
	ENGINE_API virtual void CleanUp();

protected:
	RenderComponent* pRenderer_;

	Transform* pTransform_;

	BoundVolume* pBoundVolume;

	LINK_NODE levelLink_;

public:
	ENGINE_API IImGuiBindTransform* __stdcall GetTransformForImGui() const override;
};
