#pragma once
#include "Actor.h"

enum class E_RENDER_PASS_TYPE
{
	GeometryPass = 0,
	AmbientPass,
	LightPass,
	ParticlePass,
	DebugPass,
};

class Transform;
class Camera
	: public Actor
	, public IEditorBindCamera
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
	void RenderPassBegin(E_RENDER_PASS_TYPE renderPassType);
	void RenderPassEnd();

	const Float4x4& View() const;
	const Float4x4& Projection() const;

	void SetSize(const Float2& size);
	void SetConfig(float fov, float Near, float Far);
	float GetFov() const;
	float GetNear() const;
	float GetFar() const;

private:
	bool InitGBufferRenderTarget();
	bool InitFianlRenderTarget();
	virtual ENGINE_API void CameraTransformUpdate();
	ENGINE_API void CleanUp() override;

public:
	// IImguiBindCamera Area
	ENGINE_API const Float4x4& __stdcall GetViewMatrix() const override;
	ENGINE_API const Float4x4& __stdcall GetProjectionMatrix() const override;
	ENGINE_API IRenderTarget* __stdcall GetGBufferRenderTargetForEditor() const override;
	//ENGINE_API IRenderTarget* __stdcall GetParticleRenderTargetForEditor() const override;
	//ENGINE_API IRenderTarget* __stdcall GetDebugRenderTargetForEditor() const override;
	ENGINE_API IRenderTarget* __stdcall GetFinalRenderTargetForEditor() const override;

protected:
	float cameraSensitivity_;
	float cameraSpeed_;

	Float4x4 view_;
	Float4x4 projection_;

	float fov_;
	float near_;
	float far_;

	Float2 screenOffset_;
	Float2 screenScale_;

private:
	friend class Level;
	IRenderTarget* pCurrentRenderTarget_;
	IRenderTarget* pGBufferRenderTarget_;
	IRenderTarget* pFinalRenderTarget_;
};
