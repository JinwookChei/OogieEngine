#pragma once
struct ITexture : public IUnknown {
};

struct IRenderTarget : public IUnknown {
};

struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, UINT width, UINT height) = 0;

	virtual void __stdcall BeginRender() = 0;

	virtual void __stdcall EndRender() = 0;
};