#pragma once

class PlayerCamera;
class CameraComponent
	: public SceneComponent
{
public:
	ENGINE_API CameraComponent();
	ENGINE_API ~CameraComponent() override;

	void BeginPlay() override;
	void Tick(double deltaTime) override;
	void Render() override;

private:
	void CleanUp() override;

	PlayerCamera* pBindedCamera_;
};
