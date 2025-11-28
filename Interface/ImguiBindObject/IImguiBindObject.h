#pragma once

struct IImGuiBindCamera
{
	virtual IRenderTarget* __stdcall GetGBufferTargetForImGui() const = 0;

	virtual IRenderTarget* __stdcall GetDebugBufferTargetForImGui() const = 0;
};

struct IImGuiBindPickedActor
{

};