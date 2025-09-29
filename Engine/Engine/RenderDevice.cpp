#include "stdafx.h"
#include "Engine.h"
#include "InputLayout.h"
#include "Mesh.h"
#include "Material.h"
#include "RenderTarget.h"
#include "ShaderConstants.h"
#include "RenderDevice.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);

RenderDevice* GRenderDevice = nullptr;

RenderDevice::RenderDevice()
	: pRendererImpl_(nullptr),
	rendererModule_(nullptr)
{
}

RenderDevice::~RenderDevice()
{
	CleanUp();
}
RenderDevice* RenderDevice::Create()
{
	if (nullptr != GRenderDevice)
	{
		RenderDevice::Destroy();
	}

	GRenderDevice = new RenderDevice;
	return GRenderDevice;
}
void RenderDevice::Destroy()
{
	if (nullptr != GRenderDevice)
	{
		delete GRenderDevice;

		GRenderDevice = nullptr;
	}
}
RenderDevice* RenderDevice::Instance()
{
	return GRenderDevice;
}

bool RenderDevice::Load(void* pMainHwnd, LPCWSTR libFileName)
{
	rendererModule_ = LoadLibrary(libFileName);
	if (!rendererModule_)
	{
		DEBUG_BREAK();
		return false;
	}

	DLL_FUNCTION_ARG1 CreateRenderer = (DLL_FUNCTION_ARG1)GetProcAddress(rendererModule_, "CreateRenderer");
	if (!CreateRenderer)
	{
		DEBUG_BREAK();
		return false;
	}

	CreateRenderer((void**)&pRendererImpl_);
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return false;
	}


	if (false == pRendererImpl_->Initialize(pMainHwnd, 2560, 1440))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}


void  RenderDevice::RenderBegin()
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pRendererImpl_->RenderBegin();
}

void  RenderDevice::RenderEnd()
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pRendererImpl_->RenderEnd();
}

uint64_t  RenderDevice::DrawCallCount()
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return -1;
	}

	return pRendererImpl_->DrawCallCount();
}


//ISamplerState* RenderDevice::CreateSampler(bool linear, bool clamp)
//{
//	if (nullptr == pRendererImpl_)
//	{
//		DEBUG_BREAK();
//		return nullptr;
//	}
//
//	return pRendererImpl_->CreateSampler(linear, clamp);
//}

//IRasterizer* RenderDevice::CreateRasterizer(bool back)
//{
//	if (nullptr == pRendererImpl_)
//	{
//		DEBUG_BREAK();
//		return nullptr;
//	}
//
//	return pRendererImpl_->CreateRasterizer(back);
//}

RenderTarget* RenderDevice::CreateRenderTarget(const Float2& size, const Color& clearColor, bool useDepthStencil/* = true*/)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	IRenderTarget* pRenderTargetImpl = pRendererImpl_->CreateRenderTarget(size, clearColor, useDepthStencil);

	RenderTarget* pRenderTarget = new RenderTarget(pRenderTargetImpl);

	return pRenderTarget;
}

InputLayout* RenderDevice::CreateLayout(Mesh* pMesh, Material* pMaterial)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}
	IInputLayout* pInputLayoutImpl = pRendererImpl_->CreateLayout(pMesh->GetVertex(), pMaterial->GetVertexShader());
	InputLayout* pInputLayout = new InputLayout(pInputLayoutImpl);

	return pInputLayout;
}

Mesh* RenderDevice::CreateMesh(void* vertices, uint32_t vertexSize, uint32_t vertexCount, void* indices, uint32_t indexTypeSize, uint32_t indexCount)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	IVertex* pVertexImpl = pRendererImpl_->CreateVertex(vertices, vertexSize, vertexCount, indices, indexTypeSize, indexCount);
	Mesh* pMesh = new Mesh(pVertexImpl);

	return pMesh;
}

Material* RenderDevice::CreateMaterial(const wchar_t* VS, const wchar_t* PS)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	IShader* pVertexShaderImpl = nullptr;
	if (nullptr != VS)
	{
		pVertexShaderImpl = pRendererImpl_->CreateShader(ShaderType::Vertex, VS);
	}

	IShader* pPixelShaderImpl = nullptr;
	if (nullptr != PS)
	{
		pPixelShaderImpl = pRendererImpl_->CreateShader(ShaderType::Pixel, PS);
	}
	ISamplerState* pSamplerState = nullptr;

	Material* pMaterial = new Material(pVertexShaderImpl, pPixelShaderImpl, pSamplerState);

	return pMaterial;
}

ShaderConstants* RenderDevice::CreateShaderConstants(uint32_t bufferSize)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	IConstantBuffer* pConstantBufferImpl = pRendererImpl_->CreateConstantBuffer(bufferSize);
	ShaderConstants* pShaderConstants = new ShaderConstants(pConstantBufferImpl);

	return pShaderConstants;
}


void RenderDevice::CleanUp()
{
	if (nullptr != pRendererImpl_)
	{
		pRendererImpl_->Release();
		pRendererImpl_ = nullptr;
	}

	if (nullptr != rendererModule_)
	{
		FreeLibrary(rendererModule_);
		rendererModule_ = nullptr;
	}
}