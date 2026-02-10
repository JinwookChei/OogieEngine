#include "stdafx.h"
#include "Actor.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent(Actor* pOwner)
	: pOwner_(pOwner)
	, pPSO_(nullptr)
	, pMesh_(nullptr)
	, pMaterial_(nullptr)
	//, meshTag_(0)
	//, materialTag_(0)
	//, samplerState_(E_SAMPLER_PRESET::LINEAR_CLAMP)
	//, depthState_(E_DEPTH_PRESET::DEPTH_ENABLE_WRITE)
	//, blendState_(E_BLEND_PRESET::ALPHA_BLEND)
	//, rasterizerMode_(E_RASTERIZER_PRESET::SOLID)

	//, pShader_(nullptr)
	//, pTextureColor_(nullptr)
	//, pTextureNormal_(nullptr)
	, OnMeshLoaded_()
{
}

RenderComponent::~RenderComponent()
{
	CleanUp();
}

void RenderComponent::Render()
{
	//ObjectRenderData objectData;
	//objectData.world = pOwner_->GetWorldTransform().GetWorldMatrix();
	//objectData.pMesh = pMesh_;
	//objectData.pMaterial = pMaterial_;
	//objectData.albedo_ = pTextureColor_;
	//objectData.normal_ = pTextureNormal_;
	//Renderer::Instance()->RenderGBuffer(objectData);

	ObjectFrameData objectFrameData;
	objectFrameData.world = pOwner_->GetWorldTransform().GetWorldMatrix();
	Renderer::Instance()->UpdateObjectFrame(objectFrameData);
	Renderer::Instance()->RenderTest(pPSO_);
}

//
//void RenderComponent::Create(uint16_t meshTag, uint16_t materialTag, uint16_t albedoTexTag, uint16_t normalTexTag)
//{
//	CleanUp();
//
//	// 1 : CUBE
//	// 2 : SPHERE
//	// 3 : WUKONG
//	// 4 : Capoeira
//	if (!MeshManager::Instance()->GetMesh(&pMesh_, meshTag))
//	{
//		DEBUG_BREAK();
//		return;
//	}
//	pMesh_->AddRef();
//	
//
//	// 1 : 	matDesc.shineness = 0.7f    matDesc.specularColor = { 0.7f, 0.7f, 0.7f };
//	if (!MaterialManager::Instance()->GetMaterial(&pMaterial_, materialTag))
//	{
//		DEBUG_BREAK();
//		return;
//	}
//	pMaterial_->AddRef();
//
//	// 1 : "../Resource/Texture/Bricks_4K/Bricks_Color.png";
//	// 2 : "../Resource/Texture/Bricks_4K/Bricks_NormalDX.png";
//	// 3 : WhiteTexture
//	// 4 : "../Resource/Fbx/Mixamo/maria_diffuse.png"
//	// 5 : "../Resource/Fbx/Mixamo/maria_normal.png"
//	if (!TextureManager::Instance()->GetTexture(&pTextureColor_, albedoTexTag))
//	{
//		DEBUG_BREAK();
//		return;
//	}
//	pTextureColor_->AddRef();
//
//	if (!TextureManager::Instance()->GetTexture(&pTextureNormal_, normalTexTag))
//	{
//		DEBUG_BREAK();
//		return;
//	}
//	pTextureNormal_->AddRef();
//
//	BroadcastOnMeshLoaded();
//}

void RenderComponent::Setting
(
	uint16_t meshTag, 
	uint16_t materialTag, 
	E_SAMPLER_PRESET samplerState, 
	E_DEPTH_PRESET depthState, 
	E_BLEND_PRESET blendState, 
	E_RASTERIZER_PRESET rasterizerMode
)
{
	CleanUp();

	IMesh* tmpMesh = nullptr;
	if (false == MeshManager::Instance()->GetMesh(&tmpMesh, meshTag)) DEBUG_BREAK();
	pMesh_ = tmpMesh;
	pMesh_->AddRef();

	IMaterial* tmpMaterial = nullptr;
	if (false == MaterialManager::Instance()->GetMaterial(&tmpMaterial, materialTag)) DEBUG_BREAK();
	pMaterial_ = tmpMaterial;
	pMaterial_->AddRef();

	PipelineStateDesc psoDesc;
	psoDesc.pMesh = pMesh_;
	psoDesc.pMaterial = pMaterial_;
	psoDesc.samplerState = samplerState;
	psoDesc.blendState = blendState;
	psoDesc.depthState = depthState;
	psoDesc.rasterizerMode = rasterizerMode;

	pPSO_ = Renderer::GetFactory()->CreatePipelineStateObject(psoDesc);
	if (nullptr == pPSO_)
	{
		DEBUG_BREAK();
	}
}

IMesh* RenderComponent::GetMesh() const
{
	return pMesh_;
}

void RenderComponent::BindOnMeshLoaded(MeshLoadedDelegate callback)
{
	if (!callback)
	{
		DEBUG_BREAK();
		return;
	}
	OnMeshLoaded_.push_back(callback);
}

void RenderComponent::BroadcastOnMeshLoaded()
{
	for (const auto& callback : OnMeshLoaded_)
	{
		if (!callback)
		{
			DEBUG_BREAK();
			continue;
		}
		callback(pMesh_);
	}
}

void RenderComponent::CleanUp()
{
	//if (nullptr != pTextureColor_)
	//{
	//	pTextureColor_->Release();
	//	pTextureColor_ = nullptr;
	//}

	//if (nullptr != pTextureNormal_)
	//{
	//	pTextureNormal_->Release();
	//	pTextureNormal_ = nullptr;
	//}

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

	if (nullptr != pPSO_)
	{
		pPSO_->Release();
		pPSO_ = nullptr;
	}
}