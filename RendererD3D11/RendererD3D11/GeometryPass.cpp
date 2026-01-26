#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"	
#include "ConstantBuffer.h"
#include "BlendState.h"
#include "DepthState.h"
#include "SamplerState.h"
#include "GeometryPass.h"
#include "LightPass.h"

GeometryPass::GeometryPass()
	: refCount_(1)
	, pShader_(nullptr)
	, pCBPerFrame_(nullptr)
	, pCBPerObject_(nullptr)
	, pBlendState_(nullptr)
	, pSamplerState_(nullptr)
	, pDepthState_(nullptr)
{
}

GeometryPass::~GeometryPass()
{
	CleanUp();
}

HRESULT __stdcall GeometryPass::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall GeometryPass::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall GeometryPass::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

bool GeometryPass::Init()
{
	if (false == InitShaders())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitConstantBuffer())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitBlendState())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitSamplerState())
	{
		DEBUG_BREAK();
		return false;
	}
	if (false == InitDepthState())
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}


void GeometryPass::UpdatePerFrameConstant(const CameraFrameData& cameraFrameData)
{
	Float4x4 viewTrans;
	MATH::MatrixTranspose(viewTrans, cameraFrameData.view);
	Float4x4 projectionTrans;
	MATH::MatrixTranspose(projectionTrans, cameraFrameData.projection);
	Float4x4 invViewTrans;
	MATH::MatrixInverse(invViewTrans, cameraFrameData.view);
	MATH::MatrixTranspose(invViewTrans, invViewTrans);
	Float4x4 invProjectionTrans;
	MATH::MatrixInverse(invProjectionTrans, cameraFrameData.projection);
	MATH::MatrixTranspose(invProjectionTrans, invProjectionTrans);


	CBPerFrame cbPerFrame;
	cbPerFrame.camPos = cameraFrameData.camPos;
	cbPerFrame.view = viewTrans;
	cbPerFrame.projection = projectionTrans;
	cbPerFrame.inverseView = invViewTrans;
	cbPerFrame.inverseProjection = invProjectionTrans;
	cbPerFrame.screenOffset = cameraFrameData.screenOffset;
	cbPerFrame.screenScale = cameraFrameData.screenScale;
	cbPerFrame.screenResolution = cameraFrameData.screenResolution;
	pCBPerFrame_->Update(&cbPerFrame);
	pCBPerFrame_->BindConstantBufferVS(0);
}

void GeometryPass::Render(const ObjectRenderData& objectData)
{
	Mesh* pMeshImpl = (Mesh*)objectData.pMesh;
	Material* pMaterialImpl = (Material*)objectData.pMaterial;
	Texture* pAlbedoImpl = (Texture*)objectData.albedo_;
	Texture* pNormalImpl = (Texture*)objectData.normal_;

	CBPerObject cbObject;
	MATH::MatrixTranspose(cbObject.world, objectData.world);
	cbObject.materialSpecular = objectData.pMaterial->GetSpecularColor();
	cbObject.materialShineness = objectData.pMaterial->GetShineness();
	pCBPerObject_->Update(&cbObject);
	

	pShader_->Bind();
	pSamplerState_->BindPS(0);
	pDepthState_->Bind();

	pCBPerFrame_->BindConstantBufferVS(0);
	pCBPerFrame_->BindConstantBufferPS(0);
	pCBPerObject_->BindConstantBufferVS(1);
	pCBPerObject_->BindConstantBufferPS(1);
	

	pMeshImpl->Bind();
	pAlbedoImpl->BindRenderTextureForPS(0);
	pNormalImpl->BindRenderTextureForPS(1);

	GRenderer->Draw(pMeshImpl->GetIndexCount(), true);

	pDepthState_->UnBind();
}

bool GeometryPass::InitShaders()
{
	const wchar_t* pVSPath = L"GeometryVS.cso";
	const wchar_t* pPSPath = L"GeometryPS.cso";
	
	ShaderDesc computeShaderDesc;
	computeShaderDesc.inputDesc_.push_back({ "POSITION", 0, 6, 0, false });
	computeShaderDesc.inputDesc_.push_back({ "COLOR", 0, 2, 0, false });
	computeShaderDesc.inputDesc_.push_back({ "NORMAL", 0, 6, 0, false });
	computeShaderDesc.inputDesc_.push_back({ "TEXCOORD", 0, 16, 0, false });
	computeShaderDesc.inputDesc_.push_back({ "TANGENT", 0, 2, 0, false });
	computeShaderDesc.pathVS_ = pVSPath;
	computeShaderDesc.pathPS_ = pPSPath;
	pShader_ = Shader::Create(computeShaderDesc);

	if (nullptr == pShader_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool GeometryPass::InitConstantBuffer()
{
	pCBPerFrame_ = ConstantBuffer::Create(sizeof(CBPerFrame));

	pCBPerObject_ = ConstantBuffer::Create(sizeof(CBPerObject));

	if (nullptr == pCBPerFrame_ || nullptr == pCBPerObject_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool GeometryPass::InitBlendState()
{
	pBlendState_ = BlendState::Create(E_BLEND_MODE::ALPHA_BLEND);

	if (nullptr == pBlendState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool GeometryPass::InitSamplerState()
{
	pSamplerState_ = SamplerState::Create(E_SAMPLER_TYPE::LINEAR_CLAMP, 0, D3D11_FLOAT32_MAX, 1);
	if (nullptr == pSamplerState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool GeometryPass::InitDepthState()
{
	pDepthState_ = DepthState::Create(true, true);
	if (nullptr == pDepthState_)
	{
		DEBUG_BREAK();
		return false;
	}
	return true;
}

void GeometryPass::CleanUp()
{
	if (nullptr != pDepthState_)
	{
		pDepthState_->Release();
		pDepthState_ = nullptr;
	}
	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Release();
		pSamplerState_ = nullptr;
	}
	if (nullptr != pBlendState_)
	{
		pBlendState_->Release();
		pBlendState_ = nullptr;
	}
	if (nullptr != pShader_)
	{
		pShader_->Release();
		pShader_ = nullptr;
	}

	if (nullptr != pCBPerFrame_)
	{
		pCBPerFrame_->Release();
		pCBPerFrame_ = nullptr;
	}

	if (nullptr != pCBPerObject_)
	{
		pCBPerObject_->Release();
		pCBPerObject_ = nullptr;
	}
}
