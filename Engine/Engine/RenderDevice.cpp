#include "stdafx.h"
#include "Engine.h"
#include "RenderDevice.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);

RenderDevice::RenderDevice()
	: pRendererInterface_(nullptr),
	rendererModule_(nullptr)
{
}

RenderDevice::~RenderDevice()
{
	CleanUp();
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

	CreateRenderer((void**)&pRendererInterface_);
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return false;
	}


	if (false == pRendererInterface_->Initialize(pMainHwnd, 800, 600))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void  RenderDevice::RenderBegin()
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return;
	}

	pRendererInterface_->RenderBegin();
}

void  RenderDevice::RenderEnd()
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return;
	}

	pRendererInterface_->RenderEnd();
}

uint64_t  RenderDevice::DrawCallCount()
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return -1;
	}

	return pRendererInterface_->DrawCallCount();
}

IInputLayout* RenderDevice::CreateLayout(IVertex* vertex, IShader* vertexShader)
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererInterface_->CreateLayout(vertex, vertexShader);
}

IVertex* RenderDevice::CreateVertex(void* vertices, uint32_t vertexSize, uint32_t vertexCount, void* indices /*= nullptr*/, uint32_t indexTypeSize/* = 0*/, uint32_t indexCount/* = 0*/)
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return nullptr;
	}
	return pRendererInterface_->CreateVertex(vertices, vertexSize, vertexCount, indices, indexTypeSize, indexCount);
}

IConstantBuffer* RenderDevice::CreateConstantBuffer(uint32_t bufferSize)
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererInterface_->CreateConstantBuffer(bufferSize);
}

IShader* RenderDevice::CreateShader(ShaderType shaderType, const wchar_t* path)
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererInterface_->CreateShader(shaderType, path);
}

IMaterial* RenderDevice::CreateMaterial()
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererInterface_->CreateMaterial();
}

ISamplerState* RenderDevice::CreateSampler(bool linear, bool clamp)
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererInterface_->CreateSampler(linear, clamp);
}

IRasterizer* RenderDevice::CreateRasterizer(bool back)
{
	if (nullptr == pRendererInterface_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pRendererInterface_->CreateRasterizer(back);
}


void RenderDevice::CleanUp()
{
	if (nullptr != pRendererInterface_)
	{
		pRendererInterface_->Release();
		pRendererInterface_ = nullptr;
	}

	if (nullptr != rendererModule_)
	{
		FreeLibrary(rendererModule_);
		rendererModule_ = nullptr;
	}
}