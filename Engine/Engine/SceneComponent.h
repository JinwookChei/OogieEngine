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

	ENGINE_API Float4x4 GetWorldMatrix() const;
	ENGINE_API Float4 GetWorldScale() const;
	ENGINE_API Float4 GetWorldRotation() const;
	ENGINE_API Float4 GetWorldPosition() const;
	ENGINE_API Float4 GetLocalScale() const;
	ENGINE_API Float4 GetLocalRotation() const;
	ENGINE_API Float4 GetLocalPosition() const;

private:
	void CleanUp() override;

	Transform* pTransform_;
};
