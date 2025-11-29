#pragma once

struct IImGuiBindCamera
{
	virtual IRenderTarget* __stdcall GetGBufferTargetForImGui() const = 0;

	virtual IRenderTarget* __stdcall GetDebugBufferTargetForImGui() const = 0;
};


struct IImGuiBindTransform
{
	virtual Float4* __stdcall GetActorScaleForImGui() = 0;
	virtual Float4* __stdcall GetActorRotationForImGui() = 0;
	virtual Float4* __stdcall GetActorQuaternionForImGui() = 0;
	virtual Float4* __stdcall GetActorPositionForImGui() = 0;

	virtual void __stdcall TransformUpdateForImGui() = 0;
};

struct IImGuiBindPickedActor
{
	virtual IImGuiBindTransform* __stdcall GetTransformForImGui() const = 0;
};

