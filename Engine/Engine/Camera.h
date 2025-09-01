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

	ENGINE_API void Render() override;

	const Float4x4& View() const;

	const Float4x4& Projection() const;

	void SetWidth(float width);

	void SetHeight(float height);

	void SetConfig(float fov, float Near, float Far);

private:
	void CleanUp() override;

	void CameraTransformUpdate();

	Float4x4 view_;

	Float4x4 projection_;

	float fov_;
	float width_;
	float height_;
	float near_;
	float far_;

	float cameraSensitivity_;

	float cameraSpeed_;
};
