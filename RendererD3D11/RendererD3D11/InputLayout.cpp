#include "stdafx.h"
#include "InputLayout.h"

//InputLayout::InputLayout()
//	: refCount_(1),
//	offset_(0),
//	desc_()
//{
//}
//
//InputLayout::~InputLayout()
//{
//}
//
//HRESULT __stdcall InputLayout::QueryInterface(REFIID riid, void** ppvObject)
//{
//	return E_NOTIMPL;
//}
//
//ULONG __stdcall InputLayout::AddRef()
//{
//	return ++refCount_;
//}
//
//ULONG __stdcall InputLayout::Release()
//{
//	--refCount_;
//	ULONG tmpRefCount = refCount_;
//	if (0 == refCount_) {
//		delete this;
//	}
//
//	return tmpRefCount;
//}
//
//bool InputLayout::AddInputLayout(const char* semanticName, unsigned int semanticIndex, unsigned int format, unsigned int inputSlot, bool isInstanceData)
//{
//	D3D11_INPUT_ELEMENT_DESC desc = { 0 };
//	desc.SemanticName = semanticName;
//	desc.SemanticIndex = semanticIndex;
//	desc.Format = (DXGI_FORMAT)format;
//	desc.InputSlot = inputSlot;
//	desc.AlignedByteOffset = offset_;
//	offset_ += FormatSize(desc.Format);
//	desc.InputSlotClass = isInstanceData ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
//	desc.InstanceDataStepRate = isInstanceData ? 0 : 1;
//
//	desc_.push_back(desc);
//	return true;
//}
//
//UINT InputLayout::FormatSize(DXGI_FORMAT format)
//{
//	switch (format)
//	{
//	case DXGI_FORMAT_UNKNOWN:
//		return -1;
//	case DXGI_FORMAT_R32G32B32A32_TYPELESS:
//	case DXGI_FORMAT_R32G32B32A32_FLOAT:
//	case DXGI_FORMAT_R32G32B32A32_UINT:
//	case DXGI_FORMAT_R32G32B32A32_SINT:
//		return 16;
//	case DXGI_FORMAT_R32G32B32_TYPELESS:
//	case DXGI_FORMAT_R32G32B32_FLOAT:
//	case DXGI_FORMAT_R32G32B32_UINT:
//	case DXGI_FORMAT_R32G32B32_SINT:
//		return 12;
//	case DXGI_FORMAT_R16G16B16A16_TYPELESS:
//	case DXGI_FORMAT_R16G16B16A16_FLOAT:
//	case DXGI_FORMAT_R16G16B16A16_UNORM:
//	case DXGI_FORMAT_R16G16B16A16_UINT:
//	case DXGI_FORMAT_R16G16B16A16_SNORM:
//	case DXGI_FORMAT_R16G16B16A16_SINT:
//	case DXGI_FORMAT_R32G32_TYPELESS:
//	case DXGI_FORMAT_R32G32_FLOAT:
//	case DXGI_FORMAT_R32G32_UINT:
//	case DXGI_FORMAT_R32G32_SINT:
//	case DXGI_FORMAT_R32G8X24_TYPELESS:
//	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
//	case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
//	case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
//		return 8;
//	case DXGI_FORMAT_R10G10B10A2_TYPELESS:
//	case DXGI_FORMAT_R10G10B10A2_UNORM:
//	case DXGI_FORMAT_R10G10B10A2_UINT:
//	case DXGI_FORMAT_R11G11B10_FLOAT:
//	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
//	case DXGI_FORMAT_R8G8B8A8_UNORM:
//	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
//	case DXGI_FORMAT_R8G8B8A8_UINT:
//	case DXGI_FORMAT_R8G8B8A8_SNORM:
//	case DXGI_FORMAT_R8G8B8A8_SINT:
//	case DXGI_FORMAT_R16G16_TYPELESS:
//	case DXGI_FORMAT_R16G16_FLOAT:
//	case DXGI_FORMAT_R16G16_UNORM:
//	case DXGI_FORMAT_R16G16_UINT:
//	case DXGI_FORMAT_R16G16_SNORM:
//	case DXGI_FORMAT_R16G16_SINT:
//	case DXGI_FORMAT_R32_TYPELESS:
//	case DXGI_FORMAT_D32_FLOAT:
//	case DXGI_FORMAT_R32_FLOAT:
//	case DXGI_FORMAT_R32_UINT:
//	case DXGI_FORMAT_R32_SINT:
//	case DXGI_FORMAT_R24G8_TYPELESS:
//	case DXGI_FORMAT_D24_UNORM_S8_UINT:
//	case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
//	case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
//		return 4;
//	case DXGI_FORMAT_R8G8_TYPELESS:
//	case DXGI_FORMAT_R8G8_UNORM:
//	case DXGI_FORMAT_R8G8_UINT:
//	case DXGI_FORMAT_R8G8_SNORM:
//	case DXGI_FORMAT_R8G8_SINT:
//	case DXGI_FORMAT_R16_TYPELESS:
//	case DXGI_FORMAT_R16_FLOAT:
//	case DXGI_FORMAT_D16_UNORM:
//	case DXGI_FORMAT_R16_UNORM:
//	case DXGI_FORMAT_R16_UINT:
//	case DXGI_FORMAT_R16_SNORM:
//	case DXGI_FORMAT_R16_SINT:
//		return 2;
//	case DXGI_FORMAT_R8_TYPELESS:
//	case DXGI_FORMAT_R8_UNORM:
//	case DXGI_FORMAT_R8_UINT:
//	case DXGI_FORMAT_R8_SNORM:
//	case DXGI_FORMAT_R8_SINT:
//	case DXGI_FORMAT_A8_UNORM:
//		return 1;
//	case DXGI_FORMAT_R1_UNORM:
//		break;
//		/*case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
//			break;
//		case DXGI_FORMAT_R8G8_B8G8_UNORM:
//			break;
//		case DXGI_FORMAT_G8R8_G8B8_UNORM:
//			break;
//		case DXGI_FORMAT_BC1_TYPELESS:
//			break;
//		case DXGI_FORMAT_BC1_UNORM:
//			break;
//		case DXGI_FORMAT_BC1_UNORM_SRGB:
//			break;
//		case DXGI_FORMAT_BC2_TYPELESS:
//			break;
//		case DXGI_FORMAT_BC2_UNORM:
//			break;
//		case DXGI_FORMAT_BC2_UNORM_SRGB:
//			break;
//		case DXGI_FORMAT_BC3_TYPELESS:
//			break;
//		case DXGI_FORMAT_BC3_UNORM:
//			break;
//		case DXGI_FORMAT_BC3_UNORM_SRGB:
//			break;
//		case DXGI_FORMAT_BC4_TYPELESS:
//			break;
//		case DXGI_FORMAT_BC4_UNORM:
//			break;
//		case DXGI_FORMAT_BC4_SNORM:
//			break;
//		case DXGI_FORMAT_BC5_TYPELESS:
//			break;
//		case DXGI_FORMAT_BC5_UNORM:
//			break;
//		case DXGI_FORMAT_BC5_SNORM:
//			break;
//		case DXGI_FORMAT_B5G6R5_UNORM:
//			break;
//		case DXGI_FORMAT_B5G5R5A1_UNORM:
//			break;
//		case DXGI_FORMAT_B8G8R8A8_UNORM:
//			break;
//		case DXGI_FORMAT_B8G8R8X8_UNORM:
//			break;
//		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
//			break;
//		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
//			break;
//		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
//			break;
//		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
//			break;
//		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
//			break;
//		case DXGI_FORMAT_BC6H_TYPELESS:
//			break;
//		case DXGI_FORMAT_BC6H_UF16:
//			break;
//		case DXGI_FORMAT_BC6H_SF16:
//			break;
//		case DXGI_FORMAT_BC7_TYPELESS:
//			break;
//		case DXGI_FORMAT_BC7_UNORM:
//			break;
//		case DXGI_FORMAT_BC7_UNORM_SRGB:
//			break;
//		case DXGI_FORMAT_AYUV:
//			break;
//		case DXGI_FORMAT_Y410:
//			break;
//		case DXGI_FORMAT_Y416:
//			break;
//		case DXGI_FORMAT_NV12:
//			break;
//		case DXGI_FORMAT_P010:
//			break;
//		case DXGI_FORMAT_P016:
//			break;
//		case DXGI_FORMAT_420_OPAQUE:
//			break;
//		case DXGI_FORMAT_YUY2:
//			break;
//		case DXGI_FORMAT_Y210:
//			break;
//		case DXGI_FORMAT_Y216:
//			break;
//		case DXGI_FORMAT_NV11:
//			break;
//		case DXGI_FORMAT_AI44:
//			break;
//		case DXGI_FORMAT_IA44:
//			break;
//		case DXGI_FORMAT_P8:
//			break;
//		case DXGI_FORMAT_A8P8:
//			break;
//		case DXGI_FORMAT_B4G4R4A4_UNORM:
//			break;
//		case DXGI_FORMAT_P208:
//			break;
//		case DXGI_FORMAT_V208:
//			break;
//		case DXGI_FORMAT_V408:
//			break;
//		case DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:
//			break;
//		case DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
//			break;
//		case DXGI_FORMAT_FORCE_UINT:
//			break;*/
//	default:
//		return -1;
//	}
//	return -1;
//}
