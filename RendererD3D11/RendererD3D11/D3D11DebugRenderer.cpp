#include "stdafx.h"
#include "D3D11DebugRenderer.h"


// Constant buffer structure must be 16-byte aligned (XMFLOAT4X4)
struct CBViewProj
{
	Float4x4 view;
	Float4x4 proj;
};

DebugRenderer::DebugRenderer()
	:refCount_(1)
	, pDevice_(nullptr)
	, pContext_(nullptr)
	, pVertexBuffer_(nullptr)
	, pInputLayout_(nullptr)
	, pVS_(nullptr)
	, pPS_(nullptr)
	, pCBViewProj_(nullptr)
{
}

DebugRenderer::~DebugRenderer()
{
	CleanUp();
}

HRESULT __stdcall DebugRenderer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall DebugRenderer::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall DebugRenderer::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}


bool DebugRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{
	if (!device || !context) return false;

	pDevice_ = device;
	pDevice_->AddRef();
	pContext_ = context;
	pContext_->AddRef();

	if (!CompileShaders())
		return false;

	if (!CreateBuffers())
		return false;

	return true;
}

void DebugRenderer::CleanUp()
{
	ReleaseResources();

	lineList_.clear();

	if (nullptr != pDevice_)
	{
		pDevice_->Release();
		pDevice_ = nullptr;
	}
	if (nullptr != pContext_)
	{
		pContext_->Release();
		pContext_ = nullptr;
	}
}

void DebugRenderer::ReleaseResources()
{
	if (pCBViewProj_) { pCBViewProj_->Release(); pCBViewProj_ = nullptr; }
	if (pPS_) { pPS_->Release(); pPS_ = nullptr; }
	if (pVS_) { pVS_->Release(); pVS_ = nullptr; }
	if (pInputLayout_) { pInputLayout_->Release(); pInputLayout_ = nullptr; }
	if (pVertexBuffer_) { pVertexBuffer_->Release(); pVertexBuffer_ = nullptr; }
}

bool DebugRenderer::CompileShaders()
{
	HRESULT hr = S_OK;
	ID3DBlob* vsBlob;
	ID3DBlob* psBlob;

	hr = D3DReadFileToBlob(L"DebugLineVS.cso", &vsBlob);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	hr = D3DReadFileToBlob(L"DebugLinePS.cso", &psBlob);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// Create shaders
	hr = pDevice_->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &pVS_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	hr = pDevice_->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pPS_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// Input layout (must match DebugLineVertex)
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(DebugLineVertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(DebugLineVertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = pDevice_->CreateInputLayout(layoutDesc, ARRAYSIZE(layoutDesc),
		vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &pInputLayout_);

	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool DebugRenderer::CreateBuffers()
{
	// Vertex buffer (dynamic)
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(DebugLineVertex) * 32768; // capacity for many lines (2 verts per line)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	HRESULT hr = pDevice_->CreateBuffer(&bd, nullptr, &pVertexBuffer_);
	if (FAILED(hr)) return false;

	// Constant buffer for viewProj
	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBViewProj);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;

	hr = pDevice_->CreateBuffer(&cbd, nullptr, &pCBViewProj_);
	if (FAILED(hr)) return false;

	return true;
}

void __stdcall DebugRenderer::SetViewProj(const Float4x4& view, const Float4x4& proj)
{
	if (!pContext_ || !pCBViewProj_) return;

	// HLSL expects column-major by default; transpose the matrix before uploading
	//DirectX::XMMATRIX vpT = XMMatrixTranspose(viewProj);
	//CBViewProj cb;
	//cb.viewProj = ret;
	//XMStoreFloat4x4(&cb.viewProj, vpT);

	Float4x4 viewT;
	MATH::MatrixTranspose(viewT, view);
	Float4x4 projT;
	MATH::MatrixTranspose(projT, proj);
	CBViewProj cb;
	cb.view = viewT;
	cb.proj = projT;
	
	pContext_->UpdateSubresource(pCBViewProj_, 0, nullptr, &cb, 0, 0);
}

void __stdcall DebugRenderer::DrawLine(const Float3& a, const Float3& b, const Float4& color)
{
	lineList_.push_back({ a, color });
	lineList_.push_back({ b, color });
}

void __stdcall DebugRenderer::DrawRay(const Float3& origin, Float3& dir, float length, const Color& color)
{
	Float3 normDir;
	MATH::VectorNormalize(normDir, dir);

	Float3 d;
	MATH::VectorScale(d, normDir, length);
	
	Float3 endPos;
	MATH::VectorAdd(endPos, origin, d);

	DrawLine(origin, endPos, color);
}

void __stdcall DebugRenderer::RenderAll()
{
	if (!pContext_ || !pVertexBuffer_ || !pVS_ || !pPS_ || !pInputLayout_)
		return;

	if (lineList_.empty())
		return;

	// Map vertex buffer and copy data
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HRESULT hr = pContext_->Map(pVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(hr)) return;

	size_t bytesToCopy = sizeof(DebugLineVertex) * lineList_.size();
	memcpy(mapped.pData, lineList_.data(), bytesToCopy);
	pContext_->Unmap(pVertexBuffer_, 0);

	// IA setup
	UINT stride = sizeof(DebugLineVertex);
	UINT offset = 0;
	pContext_->IASetInputLayout(pInputLayout_);
	pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Set shaders and constant buffer
	pContext_->VSSetShader(pVS_, nullptr, 0);
	pContext_->PSSetShader(pPS_, nullptr, 0);
	pContext_->VSSetConstantBuffers(0, 1, &pCBViewProj_);

	// Draw (each two vertices = one line)
	pContext_->Draw(static_cast<UINT>(lineList_.size()), 0);

	// Clear list for next frame
	//Clear();
}

void __stdcall DebugRenderer::Clear()
{
	lineList_.clear();
}