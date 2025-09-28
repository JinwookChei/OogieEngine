#pragma once
class RenderTarget;
class Mesh;
class Material;
class InputLayout;
class ShaderConstants;

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

	//ISamplerState* CreateSampler(bool linear, bool clamp);

	//IRasterizer* CreateRasterizer(bool back);

	RenderTarget* CreateRenderTarget(const Float2& size, const Color& clearColor);

	InputLayout* CreateLayout(Mesh* pMesh, Material* pMaterial);

	Mesh* CreateMesh(void* vertices, uint32_t vertexSize, uint32_t vertexCount, void* indices = nullptr, uint32_t indexTypeSize = 0, uint32_t indexCount = 0);

	Material* CreateMaterial(const wchar_t* VS, const wchar_t* PS);

	ShaderConstants* CreateShaderConstants(uint32_t bufferSize);

private:
	void CleanUp();

	IRenderer* pRendererImpl_;

	HMODULE rendererModule_;
};
