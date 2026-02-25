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

	ENGINE_API Transform& GetComponentTransform() const;

private:
	void CleanUp() override;

	Transform* pTransform_;

};
