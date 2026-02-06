#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"	
#include "ConstantBuffer.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "MergePass.h"

bool CreateScreenRectt(std::vector<ScreenRectVertex>* outVertices, std::vector<WORD>* outIndices)
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


MergePass::MergePass()
	: refCount_(1)
	, pShader_(nullptr)
	, pScreenQuad_(nullptr)
	, pBlendState_(nullptr)
	, pSamplerState_(nullptr)
{
}

MergePass::~MergePass()
{
	CleanUp();
}

HRESULT __stdcall MergePass::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall MergePass::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall MergePass::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

//void MergePass::Render(IRenderTarget* pDepthTarget, IRenderTarget* pSrcTarget)
//{
//	pDepthTarget->BindRenderTexturePS(0);
//	pSrcTarget->BindRenderTexturePS(4);
//
//
//	pSamplerState_->BindPS(0);
//	pBlendState_->Bind();
//	pShader_->Bind();
//	pScreenQuad_->Bind();
//	GRenderer->Draw(pScreenQuad_->GetIndexCount(), true);
//
//
//	pSrcTarget->UnBindRenderTexturePS(4);
//	pDepthTarget->UnBindRenderTexturePS(0);
//}


void MergePass::Render(IRenderTarget* pSrcTarget)
{
	pSrcTarget->BindRenderTexturePS(0);

	pSamplerState_->BindPS(0);
	pBlendState_->Bind();
	pShader_->Bind();
	pScreenQuad_->Bind();
	GRenderer->Draw(pScreenQuad_->GetIndexCount(), true);

	pSrcTarget->UnBindRenderTexturePS(0);
}


bool MergePass::Init()
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


bool MergePass::InitScreenQuad()
{
	std::vector<ScreenRectVertex> vertices;
	std::vector<WORD> indices;
	CreateScreenRectt(&vertices, &indices);

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

bool MergePass::InitShaders()
{
	const wchar_t* pVSPath = L"MergeVS.cso";
	const wchar_t* pPSPath = L"MergePS.cso";

	ShaderDesc mergeShaderDesc;
	mergeShaderDesc.inputDesc_.push_back({ "POSITION", 0, 16, 0, false });
	mergeShaderDesc.inputDesc_.push_back({ "TEXCOORD", 0, 16, 0, false });
	mergeShaderDesc.pathVS_ = pVSPath;
	mergeShaderDesc.pathPS_ = pPSPath;
	pShader_ = Shader::Create(mergeShaderDesc);

	if (nullptr == pShader_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}


bool MergePass::InitBlendState()
{
	pBlendState_ = BlendState::Create(E_BLEND_PRESET::ALPHA_BLEND);

	if (nullptr == pBlendState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool MergePass::InitSamplerState()
{
	pSamplerState_ = SamplerState::Create(E_SAMPLER_PRESET::LINEAR_CLAMP, 0, D3D11_FLOAT32_MAX, 1);
	if (nullptr == pSamplerState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void MergePass::CleanUp()
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
