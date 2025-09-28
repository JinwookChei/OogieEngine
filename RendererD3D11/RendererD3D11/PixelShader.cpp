#include "stdafx.h"
#include "PixelShader.h"

D3D11PixelShader::D3D11PixelShader()
	: pShader_(nullptr)
{
}

D3D11PixelShader::~D3D11PixelShader()
{
	CleanUp();
}

void D3D11PixelShader::SetShader()
{
	GRenderer->DeviceContext()->PSSetShader(pShader_, nullptr, 0);
}

bool D3D11PixelShader::OnCreateShader(ID3DBlob* pBlob)
{
	if (nullptr == pBlob)
	{
		return false;
	}

	HRESULT hr = GRenderer->Device()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "PixelShader::pShader_";
	pShader_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	return true;
}

void D3D11PixelShader::CleanUp()
{
	if (nullptr != pShader_)
	{
		pShader_->Release();
		pShader_ = nullptr;
	}
}
