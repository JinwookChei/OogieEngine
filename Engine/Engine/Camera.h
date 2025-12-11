#pragma once
#include "Actor.h"

class Transform;

class Camera
	: public Actor, 
	public IImGuiBindCamera
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

	void ParticlePassBegin();

	void ParticlePassEnd();

	void BlitToBackBuffer();

	//void BlitToBackBuffer(const Float2& offset, const Float2& scale);

	const Float4x4& View() const;

	const Float4x4& Projection() const;

	void SetSize(const Float2& size);

	void SetConfig(float fov, float Near, float Far);

	float GetFov() const;

	float GetNear() const;

	float GetFar() const;

private:
	bool InitGBuffer();

	bool InitLightBuffer();

	bool InitParticleBuffer();

	bool InitDebugBuffer();

	bool InitScreenRect();

	virtual ENGINE_API void CameraTransformUpdate();

	ENGINE_API void CleanUp() override;

public:
	// IImguiBindCamera Area
	ENGINE_API IRenderTarget* __stdcall GetGBufferRenderTargetForImGui() const override;

	ENGINE_API IRenderTarget* __stdcall GetParticleRenderTargetForImGui() const override;

	ENGINE_API IRenderTarget* __stdcall GetDebugRenderTargetForImGui() const override;

protected:
	float cameraSensitivity_;
	float cameraSpeed_;

	Float4x4 view_;
	Float4x4 projection_;

	float fov_;
	float near_;
	float far_;

private:
	friend class Level;
	// Geometry Pass
	IRenderTarget* pGBufferRenderTarget_;
	
	IRenderTarget* pFinalRenderTarget;

	// Light Pass
	//IShader* pLightPassShader_;

	// Particle Pass
	IRenderTarget* pParticleRenderTarget_;

	// Debug Pass
	IRenderTarget* pDebugRenderTarget_;
	//IShader* pDebugPassShader_;

	// Blit to Screen
	IMesh* pScreenVertex_;
	IShader* pScreenPassShader_;

	Float2 screenOffset_;
	Float2 screenScale_;
};
