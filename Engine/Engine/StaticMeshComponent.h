#pragma once

class StaticMeshComponent : public MeshComponent
{
public:
	ENGINE_API StaticMeshComponent();

	ENGINE_API ~StaticMeshComponent() override;

	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render(bool isFirst) override;

private:
	void CleanUp() override;
};
