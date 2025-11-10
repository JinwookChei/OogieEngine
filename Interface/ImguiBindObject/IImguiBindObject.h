#pragma once

struct IImGuiBindCamera
{
	virtual IRenderTarget* __stdcall GetGBufferTargetForImGui() const = 0;
};