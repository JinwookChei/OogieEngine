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

	ENGINE_API void SetClearColor(const Color& clearColor);

	ENGINE_API void SetScreenPlacement(const Float2& screenOffset, const Float2& screenScale);

	ENGINE_API Float2 GetRenderSize() const;

	IRenderTarget* GetGBufferTarget() const;

	void UpdatePerFrameConstant();

	void GeometryPassBegin();

	void GeometryPassEnd();

	void LightPassBegin();

	void RenderLight();

	void LightPassEnd();

	void BlitToBackBuffer();

	void BlitToBackBuffer(const Float2& offset, const Float2& scale);

	const Float4x4& View() const;

	const Float4x4& Projection() const;

	void SetSize(const Float2& size);

	void SetConfig(float fov, float Near, float Far);

private:
	bool InitGBuffer();

	bool InitLightBuffer();

	void InitScreenRect();

	virtual ENGINE_API void CameraTransformUpdate();

	ENGINE_API void CleanUp() override;

protected:
	float cameraSensitivity_;
	float cameraSpeed_;

	Float4x4 view_;
	Float4x4 projection_;

	float fov_;
	float near_;
	float far_;

private:
	// Geometry Pass
	IRenderTarget* pGBufferTarget_;

	// Light Pass
	IRenderTarget* pLightBufferTarget_;
	IMaterial* pLightBufferMaterial_;

	// Blit to Screen
	IMesh* pScreenVertex_;
	IMaterial* pScreenMaterial_;
	IInputLayout* pScreenInputLayout_;
	IRasterizer* pScreenRasterizer_;
	Float2 screenOffset_;
	Float2 screenScale_;
};
