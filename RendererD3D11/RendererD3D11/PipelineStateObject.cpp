#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "PipelineStateObject.h"

PipelineStateObject::PipelineStateObject()
	: refCount_(1)
	, meshSlotCount_(0)
	, ppMesheSlot_(nullptr)
	, materialSlotCount_(0)
	, ppMaterialSlot_(nullptr)
	//, pMesh_(nullptr)
	//, pMaterial_(nullptr)
	//, samplerState_(E_SAMPLER_PRESET::LINEAR_CLAMP)
	, depthState_(E_DEPTH_PRESET::DEPTH_ENABLE_WRITE)
	//, blendState_(E_BLEND_PRESET::ALPHA_BLEND)
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

IMesh* __stdcall PipelineStateObject::GetMesh(uint16_t slot) const
{
	if (slot >= meshSlotCount_)
	{
		Assert("Mesh slot index out of range.");
		return nullptr;
	}

	return ppMesheSlot_[slot];
}

IMaterial* __stdcall PipelineStateObject::GetMaterial(uint16_t slot) const
{
	if (slot >= materialSlotCount_)
	{
		Assert("Material slot index out of range.");
		return nullptr;
	}

	return ppMaterialSlot_[slot];
}

void __stdcall PipelineStateObject::SetMeshToSlot(uint16_t slot, IMesh* pMesh)
{
	if (slot >= meshSlotCount_)
	{
		Assert("Mesh slot index out of range.");
		return;
	}

	ppMesheSlot_[slot] = static_cast<Mesh*>(pMesh);
	ppMesheSlot_[slot]->AddRef();
}

void __stdcall PipelineStateObject::SetMaterialToSlot(uint16_t slot, IMaterial* pMaterial)
{
	if (slot >= materialSlotCount_)
	{
		Assert("Material slot index out of range.");
		return;
	}

	ppMaterialSlot_[slot] = static_cast<Material*>(pMaterial);
	ppMaterialSlot_[slot]->AddRef();
}

uint32_t PipelineStateObject::GetMeshSlotCount() const
{
	return meshSlotCount_;
}

uint32_t PipelineStateObject::GetMaterialSlotCount() const
{
	return materialSlotCount_;
}

//IMesh* PipelineStateObject::GetMesh() const
//{
//	return pMesh_;
//}

//IMaterial* PipelineStateObject::GetMaterial() const
//{
//	return pMaterial_;
//}

//const E_SAMPLER_PRESET& PipelineStateObject::GetSamplerState() const
//{
//	return samplerState_;
//}

const E_DEPTH_PRESET& PipelineStateObject::GetDepthState() const
{
	return depthState_;
}

//const E_BLEND_PRESET& PipelineStateObject::GetBlendState() const
//{
//	return blendState_;
//}

const E_RASTERIZER_PRESET& PipelineStateObject::GetRasterizerMode() const
{
	return rasterizerMode_;
}

void PipelineStateObject::CleanUp()
{
	for (int i = 0; i < meshSlotCount_; ++i)
	{
		if (nullptr != ppMesheSlot_[i])
		{
			ppMesheSlot_[i]->Release();
			ppMesheSlot_[i] = nullptr;
		}
	}
	delete[] ppMesheSlot_;
	ppMesheSlot_ = nullptr;

	for (int i = 0; i < materialSlotCount_; ++i)
	{
		if (nullptr != ppMaterialSlot_[i])
		{
			ppMaterialSlot_[i]->Release();
			ppMaterialSlot_[i] = nullptr;
		}
	}
	delete[] ppMaterialSlot_;
	ppMaterialSlot_ = nullptr;


	//if (nullptr != pMesh_)
	//{
	//	pMesh_->Release();
	//	pMesh_ = nullptr;
	//}
	//if (nullptr != pMaterial_)
	//{
	//	pMaterial_->Release();
	//	pMaterial_ = nullptr;
	//}
}

PipelineStateObject* PipelineStateObject::Create(const PipelineStateDesc& desc)
{
	//if (nullptr == desc.pMesh)
	//{
	//	DEBUG_BREAK();
	//	return nullptr;
	//}
	//if (nullptr == desc.pMaterial)
	//{
	//	DEBUG_BREAK();
	//	return nullptr;
	//}

	PipelineStateObject* pNewPSO = new PipelineStateObject;
	pNewPSO->meshSlotCount_ = desc.meshSlotCount;
	pNewPSO->ppMesheSlot_ = new Mesh * [pNewPSO->meshSlotCount_] {};
	pNewPSO->materialSlotCount_ = desc.materialSlotCount;
	pNewPSO->ppMaterialSlot_ = new Material * [pNewPSO->materialSlotCount_] {};

	//pNewPSO->pMesh_ = desc.pMesh;
	//pNewPSO->pMesh_->AddRef();
	//pNewPSO->pMaterial_ = desc.pMaterial;
	//pNewPSO->pMaterial_->AddRef();
	//pNewPSO->samplerState_ = desc.samplerState;
	pNewPSO->depthState_ = desc.depthState;
	//pNewPSO->blendState_ = desc.blendState;
	pNewPSO->rasterizerMode_ = desc.rasterizerMode;
	return pNewPSO;
}
