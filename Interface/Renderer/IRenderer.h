#pragma once
struct ITexture : public IUnknown {
};

struct IRenderTarget : public IUnknown {
};

struct IVertex : public IUnknown {
};

struct IMaterial : public IUnknown {
};

struct IShader : public IUnknown {
};

struct IInputLayout : public IUnknown {
	virtual bool AddInputLayout(const char* semanticName, UINT semanticIndex, DXGI_FORMAT format, UINT inputSlot, bool isInstanceData) = 0;
};

struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, UINT width, UINT height) = 0;

	virtual void __stdcall BeginRender() = 0;

	virtual void __stdcall EndRender() = 0;
};