#pragma once

class RenderDevice final
{
public:

	RenderDevice();

	virtual ~RenderDevice();

	static RenderDevice* Create();

	static void Destroy();

	static RenderDevice* Instance();

	bool Load(void* pMainHwnd, LPCWSTR libFileName);

	void  RenderBegin();

	void  RenderEnd();

	uint64_t DrawCallCount();

	IInputLayout* CreateLayout(IVertex* vertex, IShader* vertexShader);

	IVertex* CreateVertex(void* vertices, uint32_t vertexSize, uint32_t vertexCount, void* indices = nullptr, uint32_t indexTypeSize = 0, uint32_t indexCount = 0);

	IConstantBuffer* CreateConstantBuffer(uint32_t bufferSize);

	IShader* CreateShader(ShaderType shaderType, const wchar_t* path);

	IMaterial* CreateMaterial();

	ISamplerState* CreateSampler(bool linear, bool clamp);

	IRasterizer* CreateRasterizer(bool back);

private:
	void CleanUp();

	IRenderer* pRendererImpl_;

	HMODULE rendererModule_;

};
