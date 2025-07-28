#include "stdafx.h"
#include "BaseShader.h"

BaseShader::BaseShader()
	: refCount_(1)
{
}

BaseShader::~BaseShader()
{
	//CleanUp();
}

HRESULT __stdcall BaseShader::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall BaseShader::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall BaseShader::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

//bool VertexShader::CreateShader(void* shaderByteCode, size_t byteCodeLength)
//{
//	if (nullptr == shaderByteCode)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	if (0 == byteCodeLength)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	HRESULT hr = GRenderer->Device()->CreateVertexShader(shaderByteCode, byteCodeLength, nullptr, &shader_);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	return true;
//}
//
//void VertexShader::SetShader()
//{
//	if (nullptr == shader_)
//	{
//		DEBUG_BREAK();
//		return;
//	}
//	GRenderer->DeviceContext()->VSSetShader(shader_, nullptr, 0);
//}
//
//void VertexShader::CleanUp()
//{
//	if (nullptr != shader_)
//	{
//		shader_->Release();
//		shader_ = nullptr;
//	}
//}
//
//bool PixelShader::CreateShader(void* shaderByteCode, size_t byteCodeLength)
//{
//	if (nullptr == shaderByteCode)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//	if (0 == byteCodeLength)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	HRESULT hr = GRenderer->Device()->CreatePixelShader(shaderByteCode, byteCodeLength, nullptr, &shader_);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	return true;
//}
//
//void PixelShader::SetShader()
//{
//	if (nullptr == shader_)
//	{
//		DEBUG_BREAK();
//		return;
//	}
//
//	GRenderer->DeviceContext()->PSSetShader(shader_, nullptr, 0);
//}
//
//void PixelShader::CleanUp()
//{
//	if (nullptr != shader_)
//	{
//		shader_->Release();
//		shader_ = nullptr;
//	}
//}
