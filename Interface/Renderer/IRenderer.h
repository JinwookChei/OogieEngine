#pragma once
struct ITexture : public IUnknown {
};

struct IRenderTarget : public IUnknown {
};

struct IVertex : public IUnknown {

	virtual bool AddInputLayout(const char* semanticName, unsigned int semanticIndex, unsigned int format, unsigned int inputSlot, bool isInstanceData) = 0;
};

struct IMaterial : public IUnknown {
};

struct IShader : public IUnknown {
};

struct IInputLayout : public IUnknown {
	//virtual bool AddInputLayout(const char* semanticName, unsigned int semanticIndex, unsigned int format, unsigned int inputSlot, bool isInstanceData) = 0;
};

struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, UINT width, UINT height) = 0;

	virtual void __stdcall BeginRender() = 0;
	
	virtual void __stdcall EndRender() = 0;

	virtual void __stdcall Render() = 0;	

	virtual bool __stdcall CreateTriangle() = 0;
};