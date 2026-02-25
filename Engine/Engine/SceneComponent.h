#pragma once

class SceneComponent 
	: public ActorComponent
{
public:
	ENGINE_API SceneComponent();
	ENGINE_API ~SceneComponent() override;

	void BeginPlay() override;

	void Tick(double deltaTime) override;

	void Render() override;

private:
	void CleanUp() override;

	Transform* pTransform_;

};
