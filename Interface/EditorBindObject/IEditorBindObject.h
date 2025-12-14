#pragma once

struct IEditorBindCamera
{
	virtual IRenderTarget* __stdcall GetGBufferRenderTargetForEditor() const = 0;

	virtual IRenderTarget* __stdcall GetParticleRenderTargetForEditor() const = 0;

	virtual IRenderTarget* __stdcall GetDebugRenderTargetForEditor() const = 0;

	virtual IRenderTarget* __stdcall GetFinalRenderTargetForEditor() const = 0;
};


struct IEditorBindTransform
{
	virtual Float4* __stdcall GetActorScaleForEditor() = 0;
	virtual Float4* __stdcall GetActorRotationForEditor() = 0;
	virtual Float4* __stdcall GetActorQuaternionForEditor() = 0;
	virtual Float4* __stdcall GetActorPositionForEditor() = 0;

	virtual void __stdcall TransformUpdateForEditor() = 0;
};

struct IEditorBindPickedActor
{
	virtual IEditorBindTransform* __stdcall GetTransformForEditor() const = 0;
};

