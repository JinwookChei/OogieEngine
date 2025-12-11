#include "stdafx.h"
#include "Shader.h"


Shader::Shader()
	: refCount_(1)
	, pInputLayout_(nullptr)
	, pCS_(nullptr)
	, pVS_(nullptr)
	, pGS_(nullptr)
	, pPS_(nullptr)
{
}

Shader::~Shader()
{
	CleanUp();
}

bool Shader::Init(const ShaderDesc& desc)
{
	ID3DBlob* pBlobCS = nullptr;
	ID3DBlob* pBlobVS = nullptr;
	ID3DBlob* pBlobGS = nullptr;
	ID3DBlob* pBlobPS = nullptr;
	HRESULT hr;
	do
	{
		//shaderType_ = desc.type_;

		if (desc.pathCS_)
		{
			hr = D3DReadFileToBlob(desc.pathCS_, &pBlobCS);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}

			hr = GRenderer->Device()->CreateComputeShader(pBlobCS->GetBufferPointer(), pBlobCS->GetBufferSize(), nullptr, &pCS_);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}
		}

		if (desc.pathVS_)
		{
			hr = D3DReadFileToBlob(desc.pathVS_, &pBlobVS);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}

			hr = GRenderer->Device()->CreateVertexShader(pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), nullptr, &pVS_);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}

			if (desc.inputDesc_.size() > 0)
			{
				UINT offset = 0;
				std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;
				for (int i = 0; i < desc.inputDesc_.size(); ++i)
				{
					D3D11_INPUT_ELEMENT_DESC elem = { 0 };
					elem.SemanticName = desc.inputDesc_[i].semanticName_;
					elem.SemanticIndex = (UINT)desc.inputDesc_[i].semanticIndex_;
					elem.Format = (DXGI_FORMAT)desc.inputDesc_[i].format_;
					elem.InputSlot = (UINT)desc.inputDesc_[i].inputSlot_;
					elem.AlignedByteOffset = offset;
					offset += InputFormatSize(elem.Format);
					elem.InputSlotClass = desc.inputDesc_[i].isInstanceData_ ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
					elem.InstanceDataStepRate = desc.inputDesc_[i].isInstanceData_ ? 1 : 0;
					inputDesc.push_back(elem);
				}

				hr = GRenderer->Device()->CreateInputLayout(inputDesc.data(), (UINT)inputDesc.size(), pBlobVS->GetBufferPointer(), pBlobVS->GetBufferSize(), &pInputLayout_);
				if (FAILED(hr))
				{
					DEBUG_BREAK();
					break;
				}
			}
		}

		if (desc.pathGS_)
		{
			hr = D3DReadFileToBlob(desc.pathGS_, &pBlobGS);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}

			hr = GRenderer->Device()->CreateGeometryShader(pBlobGS->GetBufferPointer(), pBlobGS->GetBufferSize(), nullptr, &pGS_);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}
		}

		if (desc.pathPS_)
		{
			hr = D3DReadFileToBlob(desc.pathPS_, &pBlobPS);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}

			hr = GRenderer->Device()->CreatePixelShader(pBlobPS->GetBufferPointer(), pBlobPS->GetBufferSize(), nullptr, &pPS_);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}
		}

		if (nullptr != pBlobCS)
		{
			pBlobCS->Release();
			pBlobCS = nullptr;
		}
		if (nullptr != pBlobVS)
		{
			pBlobVS->Release();
			pBlobVS = nullptr;
		}
		if (nullptr != pBlobGS)
		{
			pBlobGS->Release();
			pBlobGS = nullptr;
		}
		if (nullptr != pBlobPS)
		{
			pBlobPS->Release();
			pBlobPS = nullptr;
		}
		return true;
	} while (true);

	if (nullptr != pBlobCS)
	{
		pBlobCS->Release();
		pBlobCS = nullptr;
	}
	if (nullptr != pBlobVS)
	{
		pBlobVS->Release();
		pBlobVS = nullptr;
	}
	if (nullptr != pBlobGS)
	{
		pBlobGS->Release();
		pBlobGS = nullptr;
	}
	if (nullptr != pBlobPS)
	{
		pBlobPS->Release();
		pBlobPS = nullptr;
	}

	if (nullptr != pPS_)
	{
		pPS_->Release();
		pPS_ = nullptr;
	}
	if (nullptr != pGS_)
	{
		pGS_->Release();
		pGS_ = nullptr;
	}
	if (nullptr != pVS_)
	{
		pVS_->Release();
		pVS_ = nullptr;
	}
	if (nullptr != pCS_)
	{
		pCS_->Release();
		pCS_ = nullptr;
	}

	if (nullptr != pInputLayout_)
	{
		pInputLayout_->Release();
		pInputLayout_ = nullptr;
	}

	return false;
}

Shader* Shader::Create(const ShaderDesc& desc)
{
	Shader* pNewShader = new Shader;
	if (false == pNewShader->Init(desc))
	{
		pNewShader->Release();
		pNewShader = nullptr;
	}

	return pNewShader;
}



HRESULT __stdcall Shader::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Shader::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall Shader::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

void Shader::Bind()
{
	GRenderer->DeviceContext()->IASetInputLayout(pInputLayout_);
	GRenderer->DeviceContext()->CSSetShader(pCS_, nullptr, 0);
	GRenderer->DeviceContext()->VSSetShader(pVS_, nullptr, 0);
	GRenderer->DeviceContext()->GSSetShader(pGS_, nullptr, 0);
	GRenderer->DeviceContext()->PSSetShader(pPS_, nullptr, 0);
}

void Shader::UnBind()
{
	GRenderer->DeviceContext()->IASetInputLayout(nullptr);
	GRenderer->DeviceContext()->CSSetShader(nullptr, nullptr, 0);
	GRenderer->DeviceContext()->VSSetShader(nullptr, nullptr, 0);
	GRenderer->DeviceContext()->GSSetShader(nullptr, nullptr, 0);
	GRenderer->DeviceContext()->PSSetShader(nullptr, nullptr, 0);
}

UINT Shader::InputFormatSize(DXGI_FORMAT format)
{
	switch (format)
	{
	case DXGI_FORMAT_UNKNOWN:
		return -1;
	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
	case DXGI_FORMAT_R32G32B32A32_UINT:
	case DXGI_FORMAT_R32G32B32A32_SINT:
		return 16;
	case DXGI_FORMAT_R32G32B32_TYPELESS:
	case DXGI_FORMAT_R32G32B32_FLOAT:
	case DXGI_FORMAT_R32G32B32_UINT:
	case DXGI_FORMAT_R32G32B32_SINT:
		return 12;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
	case DXGI_FORMAT_R16G16B16A16_UNORM:
	case DXGI_FORMAT_R16G16B16A16_UINT:
	case DXGI_FORMAT_R16G16B16A16_SNORM:
	case DXGI_FORMAT_R16G16B16A16_SINT:
	case DXGI_FORMAT_R32G32_TYPELESS:
	case DXGI_FORMAT_R32G32_FLOAT:
	case DXGI_FORMAT_R32G32_UINT:
	case DXGI_FORMAT_R32G32_SINT:
	case DXGI_FORMAT_R32G8X24_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
		return 8;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
	case DXGI_FORMAT_R10G10B10A2_UNORM:
	case DXGI_FORMAT_R10G10B10A2_UINT:
	case DXGI_FORMAT_R11G11B10_FLOAT:
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
	case DXGI_FORMAT_R16G16_TYPELESS:
	case DXGI_FORMAT_R16G16_FLOAT:
	case DXGI_FORMAT_R16G16_UNORM:
	case DXGI_FORMAT_R16G16_UINT:
	case DXGI_FORMAT_R16G16_SNORM:
	case DXGI_FORMAT_R16G16_SINT:
	case DXGI_FORMAT_R32_TYPELESS:
	case DXGI_FORMAT_D32_FLOAT:
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_R32_UINT:
	case DXGI_FORMAT_R32_SINT:
	case DXGI_FORMAT_R24G8_TYPELESS:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		return 4;
	case DXGI_FORMAT_R8G8_TYPELESS:
	case DXGI_FORMAT_R8G8_UNORM:
	case DXGI_FORMAT_R8G8_UINT:
	case DXGI_FORMAT_R8G8_SNORM:
	case DXGI_FORMAT_R8G8_SINT:
	case DXGI_FORMAT_R16_TYPELESS:
	case DXGI_FORMAT_R16_FLOAT:
	case DXGI_FORMAT_D16_UNORM:
	case DXGI_FORMAT_R16_UNORM:
	case DXGI_FORMAT_R16_UINT:
	case DXGI_FORMAT_R16_SNORM:
	case DXGI_FORMAT_R16_SINT:
		return 2;
	case DXGI_FORMAT_R8_TYPELESS:
	case DXGI_FORMAT_R8_UNORM:
	case DXGI_FORMAT_R8_UINT:
	case DXGI_FORMAT_R8_SNORM:
	case DXGI_FORMAT_R8_SINT:
	case DXGI_FORMAT_A8_UNORM:
		return 1;
	case DXGI_FORMAT_R1_UNORM:
		break;
		/*case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
			break;
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
			break;
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
			break;
		case DXGI_FORMAT_BC1_TYPELESS:
			break;
		case DXGI_FORMAT_BC1_UNORM:
			break;
		case DXGI_FORMAT_BC1_UNORM_SRGB:
			break;
		case DXGI_FORMAT_BC2_TYPELESS:
			break;
		case DXGI_FORMAT_BC2_UNORM:
			break;
		case DXGI_FORMAT_BC2_UNORM_SRGB:
			break;
		case DXGI_FORMAT_BC3_TYPELESS:
			break;
		case DXGI_FORMAT_BC3_UNORM:
			break;
		case DXGI_FORMAT_BC3_UNORM_SRGB:
			break;
		case DXGI_FORMAT_BC4_TYPELESS:
			break;
		case DXGI_FORMAT_BC4_UNORM:
			break;
		case DXGI_FORMAT_BC4_SNORM:
			break;
		case DXGI_FORMAT_BC5_TYPELESS:
			break;
		case DXGI_FORMAT_BC5_UNORM:
			break;
		case DXGI_FORMAT_BC5_SNORM:
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
			break;
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			break;
		case DXGI_FORMAT_B8G8R8X8_UNORM:
			break;
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			break;
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			break;
		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
			break;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			break;
		case DXGI_FORMAT_BC6H_TYPELESS:
			break;
		case DXGI_FORMAT_BC6H_UF16:
			break;
		case DXGI_FORMAT_BC6H_SF16:
			break;
		case DXGI_FORMAT_BC7_TYPELESS:
			break;
		case DXGI_FORMAT_BC7_UNORM:
			break;
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			break;
		case DXGI_FORMAT_AYUV:
			break;
		case DXGI_FORMAT_Y410:
			break;
		case DXGI_FORMAT_Y416:
			break;
		case DXGI_FORMAT_NV12:
			break;
		case DXGI_FORMAT_P010:
			break;
		case DXGI_FORMAT_P016:
			break;
		case DXGI_FORMAT_420_OPAQUE:
			break;
		case DXGI_FORMAT_YUY2:
			break;
		case DXGI_FORMAT_Y210:
			break;
		case DXGI_FORMAT_Y216:
			break;
		case DXGI_FORMAT_NV11:
			break;
		case DXGI_FORMAT_AI44:
			break;
		case DXGI_FORMAT_IA44:
			break;
		case DXGI_FORMAT_P8:
			break;
		case DXGI_FORMAT_A8P8:
			break;
		case DXGI_FORMAT_B4G4R4A4_UNORM:
			break;
		case DXGI_FORMAT_P208:
			break;
		case DXGI_FORMAT_V208:
			break;
		case DXGI_FORMAT_V408:
			break;
		case DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:
			break;
		case DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
			break;
		case DXGI_FORMAT_FORCE_UINT:
			break;*/
	default:
		return -1;
	}
	return -1;
}

void Shader::CleanUp()
{
	if (nullptr != pInputLayout_)
	{
		pInputLayout_->Release();
		pInputLayout_ = nullptr;
	}
	if (nullptr != pCS_)
	{
		pCS_->Release();
		pCS_ = nullptr;
	}
	if (nullptr != pVS_)
	{
		pVS_->Release();
		pVS_ = nullptr;
	}
	if (nullptr != pGS_)
	{
		pGS_->Release();
		pGS_ = nullptr;
	}
	if (nullptr != pPS_)
	{
		pPS_->Release();
		pPS_ = nullptr;
	}
}