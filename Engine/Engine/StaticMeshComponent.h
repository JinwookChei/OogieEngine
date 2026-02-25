#pragma once

class StaticMeshComponent : public MeshComponent
{
public:
	ENGINE_API StaticMeshComponent(Actor* pOwner);

	ENGINE_API ~StaticMeshComponent() override;

	ENGINE_API void Tick(double deltaTime) override;

	ENGINE_API void Render() override;

private:
	virtual void CleanUp();
};
