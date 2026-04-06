#pragma once

class CameraComponent;
class PlayerCamera
	: public Camera
{
public:
	ENGINE_API PlayerCamera();
	ENGINE_API ~PlayerCamera() override;

	void Tick(double deltaTime) override;
	void BeginPlay() override;
	void Render() override;

	bool BindComponent(CameraComponent* pComponent);

private:
	void CleanUp() override;

	CameraComponent* pOwnerComponent_;
};
