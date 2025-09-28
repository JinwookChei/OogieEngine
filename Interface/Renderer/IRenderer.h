#pragma once

enum class ShaderType
{
	Vertex = 0,
	Pixel
};

enum class FillModeType
{
	WireFrame = 0,
	Solid
};

struct ITexture : public IUnknown {
};

struct IRenderTarget : public IUnknown 
{
	virtual void __stdcall Clear() = 0;

	virtual void __stdcall Setting() = 0;

	virtual void __stdcall SetClearColor(const Color& color) = 0;
};

struct IVertex : public IUnknown 
{
	virtual void __stdcall Setting() = 0;

	virtual bool __stdcall AddInputLayout(const char* pSemanticName, uint32_t semanticIndex, uint32_t format, uint32_t inputSlot, bool isInstanceData) = 0;

	virtual bool __stdcall Draw() = 0;
};

struct IShader : public IUnknown {
	virtual void __stdcall Setting() = 0;
};

struct ISamplerState : public IUnknown {
	virtual void __stdcall Setting(uint32_t slot) = 0;
};

struct IInputLayout : public IUnknown {
	virtual void __stdcall Setting() = 0;
};

struct IConstantBuffer : public IUnknown {
	virtual void __stdcall Update(void* pSrcData) = 0;

	virtual void __stdcall VSSetting(uint32_t slot) = 0;

	virtual void __stdcall PSSetting(uint32_t slot) = 0;
};


struct IRasterizer : public IUnknown {
	virtual void __stdcall Setting() = 0;

	virtual void __stdcall SetFillMode(FillModeType fillModeType) = 0;
};

struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, uint32_t width, uint32_t height) = 0;

	virtual void __stdcall RenderBegin() = 0;
	
	virtual void __stdcall RenderEnd() = 0;

	virtual uint64_t __stdcall DrawCallCount() = 0;

	virtual IInputLayout* __stdcall CreateLayout(IVertex* pVertex, IShader* pVertexShader) = 0;

	virtual IVertex* __stdcall CreateVertex(void* pVertices, uint32_t vertexSize, uint32_t vertexCount, void* pIndices = nullptr, uint32_t indexTypeSize = 0, uint32_t indexCount = 0) = 0;

	virtual IConstantBuffer* __stdcall CreateConstantBuffer(uint32_t bufferSize) = 0;

	virtual IShader* __stdcall CreateShader(ShaderType shaderType, const wchar_t* pPath) = 0;

	virtual ISamplerState* __stdcall  CreateSampler(bool linear, bool clamp) = 0;

	virtual IRasterizer* __stdcall  CreateRasterizer(bool back) = 0;

	virtual IRenderTarget* __stdcall CreateRenderTarget(const Float2& size, const Color& clearColor) = 0;
};