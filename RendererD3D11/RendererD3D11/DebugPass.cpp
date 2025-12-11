#include "stdafx.h"
#include "Shader.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "DebugPass.h"

// Constant buffer structure must be 16-byte aligned (XMFLOAT4X4)
//struct CBViewProj
//{
//	Float4x4 view;
//	Float4x4 proj;
//};

DebugPass::DebugPass()
	: refCount_(1)
	, lineList_()
	, pVertexBuffer_(nullptr) // -> LIne
	, pShader_(nullptr)
	//, pSamplerState_(nullptr)
	, pBlendState_(nullptr)

	//refCount_(1)
	///*, pDevice_(nullptr)
	//, pContext_(nullptr)*/
	//, pVertexBuffer_(nullptr)
	////, pInputLayout_(nullptr)
	////, pVS_(nullptr)
	////, pPS_(nullptr)
	////, pCBViewProj_(nullptr)
	//, pSamplerState_(nullptr)
{
}

DebugPass::~DebugPass()
{
	CleanUp();
}

bool DebugPass::Init()
{
	if (false == InitVertexBuffer())
	{
		return false;
	}
	if (false == InitShaders())
	{
		return false;
	}
	if (false == InitBlendState())
	{
		return false;
	}
	//if (false == InitSamplerState())
	//{
	//	return false;
	//}

	return true;
}


HRESULT __stdcall DebugPass::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall DebugPass::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall DebugPass::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

void DebugPass::Render()
{
	if (lineList_.empty())
	{
		return;
	}

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HRESULT hr = GRenderer->DeviceContext()->Map(pVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return;
	}

	size_t bytesToCopy = sizeof(DebugLineVertex) * lineList_.size();
	memcpy(mapped.pData, lineList_.data(), bytesToCopy);
	GRenderer->DeviceContext()->Unmap(pVertexBuffer_, 0);


	pShader_->Bind();
	pBlendState_->Bind();
	//pSamplerState_->BindPS();
	// IA setup
	UINT stride = sizeof(DebugLineVertex);
	UINT offset = 0;
	GRenderer->DeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	GRenderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	GRenderer->Draw((UINT)lineList_.size(), false);
}

void __stdcall DebugPass::DrawLine(const Float3& start, const Float3& end, const Float4& color)
{
	lineList_.push_back({ start, color });
	lineList_.push_back({ end, color });
}

void __stdcall DebugPass::DrawRay(const Float3& origin, Float3& dir, float length, const Color& color)
{
	Float3 normDir;
	MATH::VectorNormalize(normDir, dir);

	Float3 d;
	MATH::VectorScale(d, normDir, length);

	Float3 endPos;
	MATH::VectorAdd(endPos, origin, d);

	DrawLine(origin, endPos, color);
}

//void __stdcall DebugPass::RenderAll()
//{
//	if (lineList_.empty())
//	{
//		DEBUG_BREAK();
//		return;
//	}
//
//	D3D11_MAPPED_SUBRESOURCE mapped = {};
//	HRESULT hr = GRenderer->DeviceContext()->Map(pVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return;
//	}
//
//	size_t bytesToCopy = sizeof(DebugLineVertex) * lineList_.size();
//	memcpy(mapped.pData, lineList_.data(), bytesToCopy);
//	GRenderer->DeviceContext()->Unmap(pVertexBuffer_, 0);
//
//
//	pShader_->Bind();
//	pBlendState_->Bind();
//	//pSamplerState_->BindPS();
//	// IA setup
//	UINT stride = sizeof(DebugLineVertex);
//	UINT offset = 0;
//	GRenderer->DeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
//	GRenderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//
//	GRenderer->Draw(lineList_.size());
//
//	//GRenderer->DeviceContext()->IASetInputLayout(pInputLayout_);
//
//
//	// Set shaders and constant buffer
//	//GRenderer->DeviceContext()->VSSetShader(pVS_, nullptr, 0);
//	//GRenderer->DeviceContext()->PSSetShader(pPS_, nullptr, 0);
//	//GRenderer->DeviceContext()->VSSetConstantBuffers(0, 1, &pCBViewProj_);
//
//	//GRenderer->DeviceContext()->PSSetSamplers(0, 1, &pSamplerState_);
//
//	// Draw (each two vertices = one line)
//	//GRenderer->DeviceContext()->Draw(static_cast<UINT>(lineList_.size()), 0);
//	
//
//
//	// Clear list for next frame
//
//
//
//
//
//	//Clear();
//	//if (lineList_.empty())
//	//{
//	//	return;
//	//}
//
//	//// Map vertex buffer and copy data
//	//D3D11_MAPPED_SUBRESOURCE mapped = {};
//	//HRESULT hr = GRenderer->DeviceContext()->Map(pVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
//	//if (FAILED(hr))
//	//{
//	//	return;
//	//}
//
//	//size_t bytesToCopy = sizeof(DebugLineVertex) * lineList_.size();
//	//memcpy(mapped.pData, lineList_.data(), bytesToCopy);
//	//GRenderer->DeviceContext()->Unmap(pVertexBuffer_, 0);
//
//	//// IA setup
//	//UINT stride = sizeof(DebugLineVertex);
//	//UINT offset = 0;
//	//GRenderer->DeviceContext()->IASetInputLayout(pInputLayout_);
//	//GRenderer->DeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
//	//GRenderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//
//	//// Set shaders and constant buffer
//	//GRenderer->DeviceContext()->VSSetShader(pVS_, nullptr, 0);
//	//GRenderer->DeviceContext()->PSSetShader(pPS_, nullptr, 0);
//	//GRenderer->DeviceContext()->VSSetConstantBuffers(0, 1, &pCBViewProj_);
//
//	//GRenderer->DeviceContext()->PSSetSamplers(0, 1, &pSamplerState_);
//
//	//// Draw (each two vertices = one line)
//	//GRenderer->DeviceContext()->Draw(static_cast<UINT>(lineList_.size()), 0);
//
//	//// Clear list for next frame
//	////Clear();
//}

void __stdcall DebugPass::Clear()
{
	lineList_.clear();
}

//bool DebugPass::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
//{
//	if (!device || !context) return false;
//
//	pDevice_ = device;
//	pDevice_->AddRef();
//	pContext_ = context;
//	pContext_->AddRef();
//
//	if (!CompileShaders())
//	{
//		return false;
//	}
//
//	if (!CreateBuffers())
//	{
//		return false;
//	}
//
//	if (!CreateSamplerState())
//	{
//		return false;
//	}
//
//	return true;
//}

bool DebugPass::InitVertexBuffer()
{
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(DebugLineVertex) * 32768; // capacity for many lines (2 verts per line)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, nullptr, &pVertexBuffer_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool DebugPass::InitShaders()
{
	const wchar_t* pVSPath = L"DebugLineVS.cso";
	const wchar_t* pPSPath = L"DebugLinePS.cso";

	ShaderDesc debugLineShaderDesc;
	//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(DebugLineVertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(DebugLineVertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	debugLineShaderDesc.inputDesc_.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, false });
	debugLineShaderDesc.inputDesc_.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, false });
	debugLineShaderDesc.pathVS_ = pVSPath;
	debugLineShaderDesc.pathPS_ = pPSPath;
	pShader_ = Shader::Create(debugLineShaderDesc);

	if (nullptr == pShader_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}


bool DebugPass::InitBlendState()
{
	pBlendState_ = BlendState::Create(E_BLEND_MODE::OPAQUE_BLEND);

	if (nullptr == pBlendState_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

//bool DebugPass::InitSamplerState()
//{
//	pSamplerState_ = SamplerStateT::Create(E_SAMPLER_TYPE_T::LINEAR_CLAMP, 0, D3D11_FLOAT32_MAX, 1);
//	if (nullptr == pSamplerState_)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	return true;
//}


void DebugPass::CleanUp()
{
	//ReleaseResources();

	lineList_.clear();

	if (nullptr != pVertexBuffer_)
	{
		pVertexBuffer_->Release();
		pVertexBuffer_ = nullptr;
	}
	if (nullptr != pShader_)
	{
		pShader_->Release();
		pShader_ = nullptr;
	}
	//if (nullptr != pSamplerState_)
	//{
	//	pSamplerState_->Release();
	//	pSamplerState_ = nullptr;
	//}

	if (nullptr != pBlendState_)
	{
		pBlendState_->Release();
		pBlendState_ = nullptr;
	}

	


	//if (nullptr != pDevice_)
	//{
	//	pDevice_->Release();
	//	pDevice_ = nullptr;
	//}
	//if (nullptr != pContext_)
	//{
	//	pContext_->Release();
	//	pContext_ = nullptr;
	//}
}

//void DebugPass::ReleaseResources()
//{
//	if (pCBViewProj_) { pCBViewProj_->Release(); pCBViewProj_ = nullptr; }
//	if (pPS_) { pPS_->Release(); pPS_ = nullptr; }
//	if (pVS_) { pVS_->Release(); pVS_ = nullptr; }
//	if (pInputLayout_) { pInputLayout_->Release(); pInputLayout_ = nullptr; }
//	if (pVertexBuffer_) { pVertexBuffer_->Release(); pVertexBuffer_ = nullptr; }
//}

//bool DebugPass::CompileShaders()
//{
//	HRESULT hr = S_OK;
//	ID3DBlob* vsBlob;
//	ID3DBlob* psBlob;
//
//	hr = D3DReadFileToBlob(L"DebugLineVS.cso", &vsBlob);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	hr = D3DReadFileToBlob(L"DebugLinePS.cso", &psBlob);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	// Create shaders
//	hr = pDevice_->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &pVS_);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	hr = pDevice_->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pPS_);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	// Input layout (must match DebugLineVertex)
//	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(DebugLineVertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(DebugLineVertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
//	};
//
//	hr = pDevice_->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc),
//		vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &pInputLayout_);
//
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	return true;
//}

//bool DebugPass::CreateBuffers()
//{
//	// Vertex buffer (dynamic)
//	D3D11_BUFFER_DESC bd = {};
//	bd.Usage = D3D11_USAGE_DYNAMIC;
//	bd.ByteWidth = sizeof(DebugLineVertex) * 32768; // capacity for many lines (2 verts per line)
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	bd.MiscFlags = 0;
//
//	HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, nullptr, &pVertexBuffer_);
//	if (FAILED(hr))
//	{
//		return false;
//	}
//
//	//// Constant buffer for viewProj
//	//D3D11_BUFFER_DESC cbd = {};
//	//cbd.Usage = D3D11_USAGE_DEFAULT;
//	//cbd.ByteWidth = sizeof(CBViewProj);
//	//cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	//cbd.CPUAccessFlags = 0;
//
//	//hr = pDevice_->CreateBuffer(&cbd, nullptr, &pCBViewProj_);
//	//if (FAILED(hr)) return false;
//
//	return true;
//}

//bool DebugPass::CreateSamplerState()
//{
//	D3D11_SAMPLER_DESC samplerDesc;
//	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
//	samplerDesc.MipLODBias = 0.0f;
//	samplerDesc.MaxAnisotropy = 1;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//	//samplerDesc.BorderColor[4];
//	samplerDesc.MinLOD = FLT_MIN;
//	samplerDesc.MaxLOD = FLT_MAX;
//
//	HRESULT hr = GRenderer->Device()->CreateSamplerState(&samplerDesc, &pSamplerState_);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//
//	return true;
//}
//
//void __stdcall DebugPass::SetViewProj(const Float4x4& view, const Float4x4& proj)
//{
//	if (!pContext_ || !pCBViewProj_) return;
//
//	Float4x4 viewT;
//	MATH::MatrixTranspose(viewT, view);
//	Float4x4 projT;
//	MATH::MatrixTranspose(projT, proj);
//	CBViewProj cb;
//	cb.view = viewT;
//	cb.proj = projT;
//	
//	pContext_->UpdateSubresource(pCBViewProj_, 0, nullptr, &cb, 0, 0);
//}

