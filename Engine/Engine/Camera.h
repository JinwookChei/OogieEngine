#pragma once
#include "Actor.h"

class Transform;

class Camera
	: public Actor
{
public:
	ENGINE_API Camera();

	ENGINE_API ~Camera() override;

	ENGINE_API void Tick(double deltaTime) override;

	ENGINE_API void BeginPlay() override;

	const DirectX::XMMATRIX& View() const;

	const DirectX::XMMATRIX& Projection() const;

	void SetWidth(float width);

	void SetHeight(float height);

	void SetConfig(float fov, float Near, float Far);

private:
	void CleanUp() override;

	void CameraTransformUpdate();

	DirectX::XMMATRIX view_;

	DirectX::XMMATRIX projection_;

	float fov_;
	float width_;
	float height_;
	float near_;
	float far_;

	float cameraSensitivity_;

	float cameraSpeed_;
};
