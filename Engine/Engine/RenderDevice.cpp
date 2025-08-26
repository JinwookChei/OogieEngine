#include "stdafx.h"
#include "Engine.h"
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
	if(nullptr != GRenderDevice)
	{
		GRenderDevice->CleanUp();
	}

	delete GRenderDevice;
	GRenderDevice = nullptr;
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


	if (false == pRendererImpl_->Initialize(pMainHwnd, 800, 600))
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

IInputLayout* RenderDevice::CreateLayout(IVertex* vertex, IShader* vertexShader)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererImpl_->CreateLayout(vertex, vertexShader);
}

IVertex* RenderDevice::CreateVertex(void* vertices, uint32_t vertexSize, uint32_t vertexCount, void* indices /*= nullptr*/, uint32_t indexTypeSize/* = 0*/, uint32_t indexCount/* = 0*/)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}
	return pRendererImpl_->CreateVertex(vertices, vertexSize, vertexCount, indices, indexTypeSize, indexCount);
}

IConstantBuffer* RenderDevice::CreateConstantBuffer(uint32_t bufferSize)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererImpl_->CreateConstantBuffer(bufferSize);
}

IShader* RenderDevice::CreateShader(ShaderType shaderType, const wchar_t* path)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererImpl_->CreateShader(shaderType, path);
}

IMaterial* RenderDevice::CreateMaterial()
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererImpl_->CreateMaterial();
}

ISamplerState* RenderDevice::CreateSampler(bool linear, bool clamp)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererImpl_->CreateSampler(linear, clamp);
}

IRasterizer* RenderDevice::CreateRasterizer(bool back)
{
	if (nullptr == pRendererImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererImpl_->CreateRasterizer(back);
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