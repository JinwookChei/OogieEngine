#pragma once

struct IEditorBindCamera
{
	virtual const Float4x4& __stdcall GetViewMatrix() const = 0;
	virtual const Float4x4& __stdcall GetProjectionMatrix() const = 0;
	virtual IRenderTarget* __stdcall GetGBufferRenderTargetForEditor() const = 0;
	//virtual IRenderTarget* __stdcall GetParticleRenderTargetForEditor() const = 0;
	//virtual IRenderTarget* __stdcall GetDebugRenderTargetForEditor() const = 0;
	virtual IRenderTarget* __stdcall GetFinalRenderTargetForEditor() const = 0;
};


struct IEditorBindTransform
{
	virtual const Float4x4& __stdcall GetWorldMatrixForEditor() const = 0;
	virtual Float4& __stdcall GetActorScaleForEditor() = 0;
	virtual Float4& __stdcall GetActorRotationForEditor() = 0;
	virtual Float4& __stdcall GetActorQuaternionForEditor() = 0;	
	virtual Float4& __stdcall GetActorPositionForEditor() = 0;

	virtual void __stdcall SetActorScaleForEditor(const Float4& scale) = 0;
	virtual void __stdcall SetActorRotationForEditor(const Float4& rot) = 0;
	virtual void __stdcall SetActorPositionForEditor(const Float4& pos) = 0;

	virtual void __stdcall TransformUpdateForEditor() = 0;
	virtual void __stdcall TransformUpdateForEditorTTT() = 0;
};

struct IEditorBindPickedActor
{
	virtual IEditorBindTransform& __stdcall GetTransformForEditor() = 0;
};

