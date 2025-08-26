#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "InputLayout.h"
#include "Test.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent()
	:
	pMesh_(nullptr),
	pMaterial_(nullptr),
	pInputLayout_(nullptr)
{
}

RenderComponent::~RenderComponent()
{
	if (nullptr != pMesh_) 
	{
		delete pMesh_;
		pMesh_ = nullptr;
	}
	if (nullptr != pMaterial_)
	{
		delete pMaterial_;
		pMaterial_ = nullptr;
	}
	if (nullptr != pInputLayout_)
	{
		delete pInputLayout_;
		pInputLayout_ = nullptr;
	}
}


void RenderComponent::Setting()
{
	pMaterial_->pMaterial_->Setting();

	pInputLayout_->pInputLayout_->Setting();

	pMesh_->pVertex_->Setting();
}

void RenderComponent::Draw()
{
	pMesh_->pVertex_->Draw();
}

//void RenderComponent::Create()
//{
//	std::vector<SimpleVertex> sphereVertices;
//	std::vector<WORD> sphereIndices;
//	CreateSphere(&sphereVertices, &sphereIndices);
//	IVertex* pSphereVertex = renderer_->CreateVertex(sphereVertices.data(), (uint32_t)sizeof(SimpleVertex), (uint32_t)sphereVertices.size(), sphereIndices.data(), (uint32_t)sizeof(WORD), (uint32_t)sphereIndices.size());
//
//
//	// Mesh
//	Mesh* pMeshSphere = new Mesh;
//	pMeshSphere->pVertex_ = pSphereVertex;
//	pMeshSphere->pVertex_->AddRef();
//	pMeshSphere->pVertex_->AddInputLayout("POSITION", 0, 6, 0, false);
//	pMeshSphere->pVertex_->AddInputLayout("COLOR", 0, 2, 0, false);
//	pMeshSphere->pVertex_->AddInputLayout("NORMAL", 0, 6, 0, false);
//	pMeshSphere->pVertex_->AddInputLayout("TEXCOORD", 0, 16, 0, false);
//	pMeshSphere->pVertex_->AddInputLayout("TANGENT", 0, 2, 0, false);
//
//	// Material
//	Material* pMaterialSphere = new Material;
//	IMaterial* pMaterial = renderer_->CreateMaterial();
//	pMaterialSphere->pMaterial_ = pMaterial;
//	pMaterialSphere->pMaterial_->AddRef();
//	IShader* pVertexShader = renderer_->CreateShader(ShaderType::Vertex, L"VertexShader.cso");
//	IShader* pPixelShader = renderer_->CreateShader(ShaderType::Pixel, L"PixelShader.cso");
//	pMaterialSphere->pMaterial_->SetVertexShader(pVertexShader);
//	pMaterialSphere->pMaterial_->SetPixelShader(pPixelShader);
//	pMaterialSphere->pVertexShader_ = pVertexShader;
//	pMaterialSphere->pVertexShader_->AddRef();
//	pMaterialSphere->pPixelShader_ = pPixelShader;
//	pMaterialSphere->pPixelShader_->AddRef();
//
//	// InputLayout
//	InputLayout* pInputLayoutSphere = new InputLayout;
//	IInputLayout* pInputLayout = renderer_->CreateLayout(pSphereVertex, pVertexShader);
//	pInputLayoutSphere->pInputLayout_ = pInputLayout;
//	pInputLayoutSphere->pInputLayout_->AddRef();
//
//	Actor* actor = new Actor;
//	actor->pRenderer_ = new Renderer;
//	actor->pRenderer_->pMesh_ = pMeshSphere;
//	actor->pRenderer_->pMaterial_ = pMaterialSphere;
//	actor->pRenderer_->pInputLayout_ = pInputLayoutSphere;
//
//	IConstantBuffer* constantBuffer = renderer_->CreateConstantBuffer((uint32_t)sizeof(ConstantBuffer));
//	if (nullptr == constantBuffer) {
//		DEBUG_BREAK();
//		return;
//	}
//}
