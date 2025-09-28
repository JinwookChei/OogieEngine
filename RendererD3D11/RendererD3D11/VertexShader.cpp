#include "stdafx.h"
#include "VertexShader.h"

D3D11VertexShader::D3D11VertexShader()
	:pShader_(nullptr)
{
}

D3D11VertexShader::~D3D11VertexShader()
{
	CleanUp();
}

void D3D11VertexShader::SetShader()
{
	GRenderer->DeviceContext()->VSSetShader(pShader_, nullptr, 0);
}

bool D3D11VertexShader::OnCreateShader(ID3DBlob* pBlob)
{
	if (nullptr == pBlob)
	{
		return false;
	}

	HRESULT hr = GRenderer->Device()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pShader_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "VertexShader::pShader_";
	pShader_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	return true;
}

void D3D11VertexShader::CleanUp()
{
	if (nullptr != pShader_)
	{
		pShader_->Release();
		pShader_ = nullptr;
	}
}
