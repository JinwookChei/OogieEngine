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

IMesh* PipelineStateObject::GetMesh() const
{
	return pMesh_;
}

IMaterial* PipelineStateObject::GetMaterial() const
{
	return pMaterial_;
}

const E_SAMPLER_PRESET& PipelineStateObject::GetSamplerState() const
{
	return samplerState_;
}

const E_DEPTH_PRESET& PipelineStateObject::GetDepthState() const
{
	return depthState_;
}

const E_BLEND_PRESET& PipelineStateObject::GetBlendState() const
{
	return blendState_;
}

const E_RASTERIZER_PRESET& PipelineStateObject::GetRasterizerMode() const
{
	return rasterizerMode_;
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
	pNewPSO->pMesh_->AddRef();
	pNewPSO->pMaterial_ = desc.pMaterial;
	pNewPSO->pMaterial_->AddRef();
	pNewPSO->samplerState_ = desc.samplerState;
	pNewPSO->depthState_ = desc.depthState;
	pNewPSO->blendState_ = desc.blendState;
	pNewPSO->rasterizerMode_ = desc.rasterizerMode;
	return pNewPSO;
}
