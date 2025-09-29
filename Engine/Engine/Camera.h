#pragma once
#include "Actor.h"

class Transform;
class RenderTarget;

class Camera
	: public Actor
{
public:
	ENGINE_API Camera();

	ENGINE_API ~Camera() override;

	ENGINE_API void Tick(double deltaTime) override;

	ENGINE_API void BeginPlay() override;

	ENGINE_API void Render() override;

	void RenderTest();

	const Float4x4& View() const;

	const Float4x4& Projection() const;

	void SetSize(const Float2& size);

	void SetConfig(float fov, float Near, float Far);

	void SetClearColor(const Color& clearColor);

private:
	void InitScreenRect();

	void CleanUp() override;

	void CameraTransformUpdate();

	Float4x4 view_;
	Float4x4 projection_;

	float fov_;
	Float2 size_;
	float near_;
	float far_;

	Color clearColor_;
	float cameraSensitivity_;
	float cameraSpeed_;

	RenderTarget* pRenderTarget_;

	Mesh* pScreenVertex_;
	Material* pScreenMaterial_;
	InputLayout* pScreenInputLayout_;
	ShaderConstants* pScreenConstantBuffer_;
};
