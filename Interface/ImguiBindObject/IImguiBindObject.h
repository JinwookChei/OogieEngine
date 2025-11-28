#pragma once

struct IImGuiBindCamera
{
	virtual IRenderTarget* __stdcall GetGBufferTargetForImGui() const = 0;

	virtual IRenderTarget* __stdcall GetDebugBufferTargetForImGui() const = 0;
};

struct IImGuiBindPickedActor
{
	virtual Float4x4& __stdcall GetActorTransformForImGui() const = 0;
};