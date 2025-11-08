#pragma once

struct IImguiBindCamera
{
	virtual IRenderTarget* __stdcall GetGBufferTargetForImgui() const = 0;
};