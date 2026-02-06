#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"	
#include "ConstantBuffer.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "FinalPass.h"

bool CreateScreenRecttt(std::vector<ScreenRectVertex>* outVertices, std::vector<WORD>* outIndices)
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


FinalPass::FinalPass()
	: refCount_(1)
	, pShader_(nullptr)
	, pScreenQuad_(nullptr)
	, pBlendState_(nullptr)
	, pSamplerState_(nullptr)
{
}

FinalPass::~FinalPass()
{
	CleanUp();
}

HRESULT __stdcall FinalPass::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall FinalPass::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall FinalPass::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

void FinalPass::Render(IRenderTarget* pSrcTarget)
{
	if (nullptr == pSrcTarget)
	{
		DEBUG_BREAK();
	}

	pSrcTarget->BindRenderTexturePS(4);

	pSamplerState_->BindPS(0);
	pBlendState_->Bind();
	pShader_->Bind();
	pScreenQuad_->Bind();
	GRenderer->Draw(pScreenQuad_->GetIndexCount(), true);

	pSrcTarget->UnBindRenderTexturePS(4);
}

bool FinalPass::Init()
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


bool FinalPass::InitScreenQuad()
{
	std::vector<ScreenRectVertex> vertices;
	std::vector<WORD> indices;
	CreateScreenRecttt(&vertices, &indices);

	MeshDesc meshDesc;
	meshDesc.vertexFormat = E_VERTEX_FORMAT::SCREEN_RECT;
	meshDesc.vertexFormatSize = sizeof(ScreenRectVertex);
	meshDesc.vertexCount = vertices.size();
	meshDesc.vertices = vertices.data();
	meshDesc.indexFormatSize = sizeof(WORD);
	meshDesc.indexCount = indices.size();
	meshDesc.indices = indices.data();
	pScreenQuad_ = (Mesh*)GResourceFactory->CreateMesh(meshDesc);
	if (nullptr == pScreenQuad_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool FinalPass::InitShaders()
{
	const wchar_t* pVSPath = L"FinalVS.cso";
	const wchar_t* pPSPath = L"FinalPS.cso";

	ShaderDesc finalShaderDesc;
	finalShaderDesc.inputDesc_.push_back({ "POSITION", 0, 16, 0, false });
	finalShaderDesc.inputDesc_.push_back({ "TEXCOORD", 0, 16, 0, false });
	finalShaderDesc.pathVS_ = pVSPath;
	finalShaderDesc.pathPS_ = pPSPath;
	pShader_ = Shader::Create(finalShaderDesc);

	if (nullptr == pShader_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}


bool FinalPass::InitBlendState()
{
	pBlendState_ = BlendState::Create(E_BLEND_MODE::OPAQUE_BLEND);

	if (nullptr == pBlendState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool FinalPass::InitSamplerState()
{
	pSamplerState_ = SamplerState::Create(E_SAMPLER_PRESET::LINEAR_CLAMP, 0, D3D11_FLOAT32_MAX, 1);
	if (nullptr == pSamplerState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void FinalPass::CleanUp()
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

	if (nullptr != pScreenQuad_)
	{
		pScreenQuad_->Release();
		pScreenQuad_ = nullptr;
	}
}
