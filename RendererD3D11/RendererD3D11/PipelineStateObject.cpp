#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "PipelineStateObject.h"

PipelineStateObject::PipelineStateObject()
	:refCount_(1)
	, pMesh_(nullptr)
	, pMaterial_(nullptr)
	, samplerState_(E_SAMPLER_PRESET::LINEAR_CLAMP)
	, depthState_(E_DEPTH_PRESET::DEPTH_ENABLE_WRITE)
	, blendState_(E_BLEND_PRESET::ALPHA_BLEND)
	, rasterizerMode_(E_RASTERIZER_PRESET::SOLID)
{
}

PipelineStateObject::~PipelineStateObject()
{
	CleanUp();
}

HRESULT __stdcall PipelineStateObject::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall PipelineStateObject::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall PipelineStateObject::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}


void PipelineStateObject::CleanUp()
{
	if (nullptr != pMesh_)
	{
		pMesh_->Release();
		pMesh_ = nullptr;
	}
	if (nullptr != pMaterial_)
	{
		pMaterial_->Release();
		pMaterial_ = nullptr;
	}
}

PipelineStateObject* PipelineStateObject::Create(const PipelineStateDesc& desc)
{
	if (nullptr == desc.pMesh)
	{
		DEBUG_BREAK();
		return nullptr;
	}
	if (nullptr == desc.pMaterial)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	PipelineStateObject* pNewPSO = new PipelineStateObject;
	pNewPSO->pMesh_ = desc.pMesh;
	pNewPSO->pMaterial_ = desc.pMaterial;
	pNewPSO->samplerState_ = desc.samplerState;
	pNewPSO->depthState_ = desc.depthState;
	pNewPSO->blendState_ = desc.blendState;
	pNewPSO->rasterizerMode_ = desc.rasterizerMode;
	return pNewPSO;
}
