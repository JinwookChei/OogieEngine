#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"	
#include "ConstantBuffer.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "LightPass.h"
#include "MergePass.h"


bool CreateScreenRect(std::vector<ScreenRectVertex>* outVertices, std::vector<WORD>* outIndices)
{
	if (nullptr == outVertices || nullptr == outIndices)
	{
		return false;
	}

	outVertices->resize(4);
	(*outVertices)[0] = { {-1.0f, 1.0f},{0.0f, 0.0f} };
	(*outVertices)[1] = { {1.0f, 1.0f},{1.0f, 0.0f} };
	(*outVertices)[2] = { {1.0f, -1.0f},{1.0f, 1.0f} };
	(*outVertices)[3] = { {-1.0f, -1.0f},{0.0f, 1.0f} };

	outIndices->resize(6, 0);
	(*outIndices)[0] = { 0 };
	(*outIndices)[1] = { 1 };
	(*outIndices)[2] = { 2 };
	(*outIndices)[3] = { 0 };
	(*outIndices)[4] = { 2 };
	(*outIndices)[5] = { 3 };

	return true;
}


LightPass::LightPass()
	: refCount_(1)
	, pShader_(nullptr)
	, pCBPerLight_(nullptr)
	, pBlendState_(nullptr)
	, pSamplerState_(nullptr)
{
}

LightPass::~LightPass()
{
	CleanUp();
}

HRESULT __stdcall LightPass::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall LightPass::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall LightPass::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

bool LightPass::Init()
{
	if (false == InitScreenQuad())
	{
		DEBUG_BREAK();
		return false;
	}

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

	return true;
}


void LightPass::RenderBegin(IRenderTarget* pGBufferTarget)
{
	pBlendState_->Bind();
	pGBufferTarget->BindRenderTexturePS(0);
	pShader_->Bind();
	pScreenQuad_->Bind();
}

void LightPass::Render(const LightRenderData& lightData)
{
	CBPerLight cbPerLight = {};
	cbPerLight.lightDiffuse = lightData.lightDiffuse;
	cbPerLight.lightSpecular = lightData.lightSpecular;
	cbPerLight.lightAmbient = lightData.lightAmbient;

	cbPerLight.direction_D_S = lightData.direction_D_S;
	cbPerLight.range_S_P = lightData.range_S_P;
	cbPerLight.position_S_P = lightData.position_S_P;

	cbPerLight.exponent_S = lightData.exponent_S;
	cbPerLight.innerAngle_S = lightData.innerAngle_S;
	cbPerLight.outerAngle_S = lightData.outerAngle_S;

	cbPerLight.attenuationConst_S_P = lightData.attenuationConst_S_P;
	cbPerLight.attenuationLinear_S_P = lightData.attenuationLinear_S_P;
	cbPerLight.attenuationQuad_S_P = lightData.attenuationQuad_S_P;

	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
	cbPerLight.lightType = lightData.lightType;
	pCBPerLight_->Update(&cbPerLight);
	pCBPerLight_->BindConstantBufferPS(1);

	GRenderer->Draw(pScreenQuad_->GetIndexCount(), true);
}

void LightPass::RenderEnd(IRenderTarget* pGBufferTarget)
{
	pGBufferTarget->UnBindRenderTexturePS(0);
	pBlendState_->UnBind();
}

bool LightPass::InitScreenQuad()
{
	std::vector<ScreenRectVertex> vertices;
	std::vector<WORD> indices;
	CreateScreenRect(&vertices, &indices);

	MeshDesc meshDesc;
	meshDesc.vertexFormat = E_VERTEX_FORMAT::SCREEN_RECT;
	meshDesc.vertexFormatSize = sizeof(ScreenRectVertex);
	meshDesc.vertexCount = vertices.size();
	meshDesc.vertices = vertices.data();
	meshDesc.indexFormatSize = sizeof(WORD);
	meshDesc.indexCount = indices.size();
	meshDesc.indices = indices.data();
	pScreenQuad_ = (Mesh*)GRenderer->CreateMesh(meshDesc);
	if (nullptr == pScreenQuad_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool LightPass::InitShaders()
{
	const wchar_t* pVSPath = L"LightVS.cso";
	const wchar_t* pPSPath = L"LightPS.cso";

	ShaderDesc lightShaderDesc;
	lightShaderDesc.inputDesc_.push_back({ "POSITION", 0, 16, 0, false });
	lightShaderDesc.inputDesc_.push_back({ "TEXCOORD", 0, 16, 0, false });
	lightShaderDesc.pathVS_ = pVSPath;
	lightShaderDesc.pathPS_ = pPSPath;
	pShader_ = Shader::Create(lightShaderDesc);

	if (nullptr == pShader_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool LightPass::InitConstantBuffer()
{
	pCBPerLight_ = ConstantBuffer::Create(sizeof(CBPerLight));
	if (nullptr == pCBPerLight_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool LightPass::InitBlendState()
{
	pBlendState_ = BlendStateT::Create(E_BLEND_MODE::ADDITIVE_BLEND);

	if (nullptr == pBlendState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool LightPass::InitSamplerState()
{
	pSamplerState_ = SamplerStateT::Create(E_SAMPLER_TYPE_T::LINEAR_CLAMP, 0, D3D11_FLOAT32_MAX, 1);
	if (nullptr == pSamplerState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void LightPass::CleanUp()
{
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
	if (nullptr != pCBPerLight_)
	{
		pCBPerLight_->Release();
		pCBPerLight_ = nullptr;
	}
	if (nullptr != pScreenQuad_)
	{
		pScreenQuad_->Release();
		pScreenQuad_ = nullptr;
	}

}
