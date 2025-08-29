#include "stdafx.h"
#include "Actor.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "InputLayout.h"
#include "ShaderConstants.h"
#include "Rasterizer.h"
#include "Test.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent(Actor* pOwner)
	: pOwner_(pOwner),
	pMesh_(nullptr),
	pMaterial_(nullptr),
	pInputLayout_(nullptr),
	pConstantBuffer_(nullptr),
	pRasterizer_(nullptr)
{
}

RenderComponent::~RenderComponent()
{
	CleanUp();
}

void RenderComponent::Render()
{
	// 카메라
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(-10.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f);

	// 상수 버퍼 업데이트
	ConstantBuffer cb;
	cb.world = pOwner_->GetWorldTransform().GetWorldMatrixTranspose();
	cb.view = DirectX::XMMatrixTranspose(view);
	cb.projection = DirectX::XMMatrixTranspose(projection);

	pMaterial_->Setting();
	pMesh_->Setting();
	pInputLayout_->Setting();

	pConstantBuffer_->Update(&cb);
	pConstantBuffer_->VSSetting(0);
	pConstantBuffer_->PSSetting(0);

	pMesh_->Draw();
}

//Transform RenderComponent::GetWorldTransform() const
//{
//	if (nullptr == pOwner_)
//	{
//		return Transform();
//	}
//
//	Transform ownerTransform = pOwner_->GetWorldTransform();
//
//	ownerTransform += *pRelativeTransform_;
//
//	return ;
//}
//
//Transform* RenderComponent::GetRelativeTransform() const
//{
//
//}


void RenderComponent::Create()
{
	std::vector<SimpleVertex> sphereVertices;
	std::vector<WORD> sphereIndices;
	CreateSphere(&sphereVertices, &sphereIndices);

	// Mesh
	IVertex* pSphereVertex = RenderDevice::Instance()->CreateVertex(sphereVertices.data(), (uint32_t)sizeof(SimpleVertex), (uint32_t)sphereVertices.size(), sphereIndices.data(), (uint32_t)sizeof(WORD), (uint32_t)sphereIndices.size());
	pMesh_ = new Mesh(pSphereVertex);
	pMesh_->AddInputLayout("POSITION", 0, 6, 0, false);
	pMesh_->AddInputLayout("COLOR", 0, 2, 0, false);
	pMesh_->AddInputLayout("NORMAL", 0, 6, 0, false);
	pMesh_->AddInputLayout("TEXCOORD", 0, 16, 0, false);
	pMesh_->AddInputLayout("TANGENT", 0, 2, 0, false);


	// Shader
	IShader* pVertexShader = RenderDevice::Instance()->CreateShader(ShaderType::Vertex, L"VertexShader.cso");
	IShader* pPixelShader = RenderDevice::Instance()->CreateShader(ShaderType::Pixel, L"PixelShader.cso");

	// Material
	IMaterial* pMaterial = RenderDevice::Instance()->CreateMaterial();
	pMaterial_ = new Material(pMaterial);
	pMaterial_->SetVertexShader(pVertexShader);
	pMaterial_->SetPixelShader(pPixelShader);

	// InputLayout
	IInputLayout* pInputLayout = RenderDevice::Instance()->CreateLayout(pSphereVertex, pVertexShader);
	pInputLayout_ = new InputLayout(pInputLayout);

	// ShaderConstants
	IConstantBuffer* pContantBuffer = RenderDevice::Instance()->CreateConstantBuffer((uint32_t)sizeof(ConstantBuffer));
	pConstantBuffer_ = new ShaderConstants(pContantBuffer);


	pSphereVertex->Release();
	pVertexShader->Release();
	pPixelShader->Release();
	pMaterial->Release();
	pInputLayout->Release();
	pContantBuffer->Release();
}


void RenderComponent::CleanUp()
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

	if (nullptr != pConstantBuffer_)
	{
		delete pConstantBuffer_;
		pConstantBuffer_ = nullptr;
	}

	if (nullptr != pRasterizer_)
	{
		delete pRasterizer_;
		pRasterizer_ = nullptr;
	}
}