#include "stdafx.h"
#include "Actor.h"
#include "Transform.h"
#include "GeometryGenerator.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent(Actor* pOwner)
	: pOwner_(pOwner),
	pMesh_(nullptr),
	pMaterial_(nullptr),
	pInputLayout_(nullptr),
	pRasterizer_(nullptr)
{
}

RenderComponent::~RenderComponent()
{
	CleanUp();
}

void RenderComponent::Render()
{
	CBPerObject cbPerObject;
	cbPerObject.world = pOwner_->GetWorldTransform().GetWorldMatrixTranspose();
	cbPerObject.materialSpecular = pMaterial_->GetSpecularColor();
	cbPerObject.materialShineness = pMaterial_->GetShineness();

	ConstantManager::Instance()->UpdatePerObejct(&cbPerObject);

	pMaterial_->Setting();
	pMesh_->Setting();
	pInputLayout_->Setting();
	pRasterizer_->Setting();
	pMesh_->Draw();
}


void RenderComponent::Create(MESH_TYPE meshType)
{
	std::vector<SimpleVertex> sphereVertices;
	std::vector<WORD> sphereIndices;

	switch (meshType)
	{
	case MESH_TYPE::SPHERE:
		GeometryGenerator::CreateSphere(&sphereVertices, &sphereIndices);
		break;
	case MESH_TYPE::CUBE:
		GeometryGenerator::CreateCube(&sphereVertices, &sphereIndices);
		break;
	default:
		break;
	}

	// Mesh
	pMesh_ = GRenderer->CreateMesh
	(
		sphereVertices.data(), (uint32_t)sizeof(SimpleVertex),
		(uint32_t)sphereVertices.size(), sphereIndices.data(),
		(uint32_t)sizeof(WORD),
		(uint32_t)sphereIndices.size()
	);
	pMesh_->AddInputLayout("POSITION", 0, 6, 0, false);
	pMesh_->AddInputLayout("COLOR", 0, 2, 0, false);
	pMesh_->AddInputLayout("NORMAL", 0, 6, 0, false);
	pMesh_->AddInputLayout("TEXCOORD", 0, 16, 0, false);
	pMesh_->AddInputLayout("TANGENT", 0, 2, 0, false);


	MaterialDesc matDesc;
	matDesc.VS = L"ObjectVS.cso";
	matDesc.PS = L"DeferredObjectPS.cso";
	matDesc.samplerLinear = true;
	matDesc.samplerClamp = false;
	matDesc.shineness = 0.7f;
	matDesc.specularColor = { 0.7f, 0.7f, 0.7f };

	pMaterial_ = GRenderer->CreateMaterial(matDesc);
	pInputLayout_ = GRenderer->CreateLayout(pMesh_, pMaterial_->GetVertexShader());
	pRasterizer_ = GRenderer->CreateRasterizer(false, true);
	pRasterizer_->SetFillMode(EFillModeType::Solid);
}


void RenderComponent::CleanUp()
{
	if (nullptr != pRasterizer_)
	{
		pRasterizer_->Release();
		pRasterizer_ = nullptr;
	}
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

	if (nullptr != pInputLayout_)
	{
		pInputLayout_->Release();
		pInputLayout_ = nullptr;
	}

	if (nullptr != pRasterizer_)
	{
		pRasterizer_->Release();
		pRasterizer_ = nullptr;
	}
}