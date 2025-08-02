#pragma once
struct ITexture : public IUnknown {
};

struct IRenderTarget : public IUnknown 
{
	virtual void __stdcall Clear() = 0;
	virtual void __stdcall Setting() = 0;
};

struct IShader : public IUnknown {
	virtual void __stdcall Setting() = 0;
};

struct IVertex : public IUnknown 
{
	virtual void __stdcall Setting() = 0;

	virtual bool __stdcall AddInputLayout(const char* semanticName, unsigned int semanticIndex, unsigned int format, unsigned int inputSlot, bool isInstanceData) = 0;

	virtual bool __stdcall CreateInputLayout(IShader* vertexShader) = 0;
};

struct IMaterial : public IUnknown {
};



struct IInputLayout : public IUnknown {
	virtual void __stdcall Setting() = 0;
};

struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, UINT width, UINT height) = 0;

	virtual void __stdcall BeginRender() = 0;
	
	virtual void __stdcall EndRender() = 0;

	virtual void __stdcall Render() = 0;

	virtual bool __stdcall CreateTriangle() = 0;

	virtual bool __stdcall CreateVertex(void* vertices, UINT vertexSize, UINT vertexCount, void* indices, UINT indexSize, IVertex** outVertex) = 0;

	virtual bool __stdcall CreateVertex(void* vertices, UINT vertexSize, UINT vertexCount, IVertex** outVertex) = 0;

	virtual bool __stdcall CreateShader(LPCWSTR pFileName, bool isVertexShader, IShader** outShader) = 0;
};