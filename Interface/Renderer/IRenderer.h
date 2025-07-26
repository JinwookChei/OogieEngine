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

struct IInputLayOut : public IUnknown {
	virtual bool AddInputLayOut(const char* semanticName, uint32_t format, bool isInstanceData, uint32_t inputSlot, uint32_t semanticIndex) = 0;
};

struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, UINT width, UINT height) = 0;

	virtual void __stdcall BeginRender() = 0;

	virtual void __stdcall EndRender() = 0;
};