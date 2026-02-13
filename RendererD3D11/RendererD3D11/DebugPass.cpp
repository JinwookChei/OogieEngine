#include "stdafx.h"
#include "Shader.h"
#include "BlendState.h"
#include "SamplerState.h"
#include "DebugPass.h"


//DebugPass::DebugPass()
//	: refCount_(1)
//	, lineList_()
//	, pVertexBuffer_(nullptr) // -> LIne
//	, pShader_(nullptr)
//	, pBlendState_(nullptr)
//{
//}
//
//DebugPass::~DebugPass()
//{
//	CleanUp();
//}
//
//bool DebugPass::Init()
//{
//	if (false == InitVertexBuffer())
//	{
//		return false;
//	}
//	if (false == InitShaders())
//	{
//		return false;
//	}
//	if (false == InitBlendState())
//	{
//		return false;
//	}
//
//	return true;
//}
//
//
//HRESULT __stdcall DebugPass::QueryInterface(REFIID riid, void** ppvObject)
//{
//	return E_NOTIMPL;
//}
//
//ULONG __stdcall DebugPass::AddRef()
//{
//	return ++refCount_;
//}
//
//ULONG __stdcall DebugPass::Release()
//{
//	--refCount_;
//	ULONG tmpRefCount = refCount_;
//	if (0 == refCount_)
//	{
//		delete this;
//	}
//	return tmpRefCount;
//}
//
//void DebugPass::Render()
//{
//	if (lineList_.empty())
//	{
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
//	GRenderer->Draw((UINT)lineList_.size(), false);
//}
//
//void __stdcall DebugPass::DrawLine(const Float3& start, const Float3& end, const Float4& color)
//{
//	lineList_.push_back({ start, color });
//	lineList_.push_back({ end, color });
//}
//
//void __stdcall DebugPass::DrawRay(const Float3& origin, Float3& dir, float length, const Color& color)
//{
//	Float3 normDir;
//	MATH::VectorNormalize(normDir, dir);
//
//	Float3 d;
//	MATH::VectorScale(d, normDir, length);
//
//	Float3 endPos;
//	MATH::VectorAdd(endPos, origin, d);
//
//	DrawLine(origin, endPos, color);
//}
//
//void __stdcall DebugPass::Clear()
//{
//	lineList_.clear();
//}
//
//bool DebugPass::InitVertexBuffer()
//{
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
//		DEBUG_BREAK();
//		return false;
//	}
//
//	return true;
//}
//
//bool DebugPass::InitShaders()
//{
//	const wchar_t* pVSPath = L"DebugLineVS.cso";
//	const wchar_t* pPSPath = L"DebugLinePS.cso";
//
//	ShaderDesc debugLineShaderDesc;
//	debugLineShaderDesc.inputDesc_.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, false });
//	debugLineShaderDesc.inputDesc_.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, false });
//	debugLineShaderDesc.pathVS_ = pVSPath;
//	debugLineShaderDesc.pathPS_ = pPSPath;
//	pShader_ = Shader::Create(debugLineShaderDesc);
//
//	if (nullptr == pShader_)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	return true;
//}
//
//
//bool DebugPass::InitBlendState()
//{
//	pBlendState_ = BlendState::Create(E_BLEND_PRESET::OPAQUE_BLEND);
//
//	if (nullptr == pBlendState_)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	return true;
//}
//
//void DebugPass::CleanUp()
//{
//	lineList_.clear();
//
//	if (nullptr != pVertexBuffer_)
//	{
//		pVertexBuffer_->Release();
//		pVertexBuffer_ = nullptr;
//	}
//	if (nullptr != pShader_)
//	{
//		pShader_->Release();
//		pShader_ = nullptr;
//	}
//
//	if (nullptr != pBlendState_)
//	{
//		pBlendState_->Release();
//		pBlendState_ = nullptr;
//	}
//}
