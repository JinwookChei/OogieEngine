#include "stdafx.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "DeferredTarget.h"
#include "Rasterizer.h"
#include "Particle.h"
#include "PipelineStateObject.h"
#include "RenderResourceFactory.h"

RenderResourceFactory* GResourceFactory = nullptr;
RenderResourceFactory::RenderResourceFactory()
	: refCount_(1)
{
	GResourceFactory = this;
}

RenderResourceFactory::~RenderResourceFactory()
{
}

HRESULT __stdcall RenderResourceFactory::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall RenderResourceFactory::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall RenderResourceFactory::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}
	return tmpRefCount;
}

IMesh* __stdcall RenderResourceFactory::CreateMesh(const MeshDesc& desc)
{
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;
	Mesh* pNewMesh = nullptr;
	void* copyVertices = nullptr;
	void* copyIndices = nullptr;

	do
	{
		if (nullptr == desc.vertices)
		{
			Assert("DESC::Vertices is NULL");
			break;
		}

		D3D11_BUFFER_DESC bd;
		memset(&bd, 0x00, sizeof(D3D11_BUFFER_DESC));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = desc.vertexFormatSize * desc.vertexCount;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		memset(&InitData, 0x00, sizeof(D3D11_SUBRESOURCE_DATA));
		InitData.pSysMem = desc.vertices;

		HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}

		// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
		const char* debugObjectName = "VertexBuffer::pVertexBuffer_";
		pVertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
		// ---------------------------------------------------------------------------

		// 인덱스 버퍼
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = desc.indexFormatSize * desc.indexCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = desc.indices;

		hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, &pIndexBuffer);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}

		// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
		debugObjectName = "VertexBuffer::pIndexBuffer_";
		pIndexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
		// ---------------------------------------------------------------------------


		// DeepCopy - Vertices 
		if (desc.vertexCount != 0)
		{
			size_t dataSize = desc.vertexCount * desc.vertexFormatSize;
			copyVertices = malloc(dataSize);   // 또는 new char[dataSize];
			memcpy(copyVertices, desc.vertices, dataSize);
		}

		// DeepCopy - Indices 
		if (desc.indexCount != 0)
		{
			size_t dataSize = desc.indexCount * desc.indexFormatSize;
			copyIndices = malloc(dataSize);   // 또는 new char[dataSize];
			memcpy(copyIndices, desc.indices, dataSize);
		}


		pNewMesh = new Mesh;
		if (false == pNewMesh->Init
		(
			desc.vertexFormat,
			desc.vertexFormatSize,
			desc.vertexCount,
			copyVertices,
			pVertexBuffer,
			desc.indexFormatSize,
			desc.indexCount,
			copyIndices,
			pIndexBuffer
		))
		{
			Assert("Mesh Init Fail !!");
			break;
		}

		return pNewMesh;

	} while (false);



	if (nullptr != copyVertices)
	{
		free(copyVertices);
		copyVertices = nullptr;
	}
	if (nullptr != copyIndices)
	{
		free(copyIndices);
		copyIndices = nullptr;
	}
	if (nullptr != pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}
	if (nullptr != pIndexBuffer)
	{
		pIndexBuffer->Release();
		pIndexBuffer = nullptr;
	}
	if (nullptr != pNewMesh)
	{
		pNewMesh->Release();
		pNewMesh = nullptr;
	}

	return nullptr;
}

IMaterial* __stdcall RenderResourceFactory::CreateMaterial(const MaterialDesc& desc)
{
	IMaterial* pMaterial = Material::Create(desc);
	return pMaterial;
}

ITexture* __stdcall RenderResourceFactory::CreateTextureFromFile(const wchar_t* fileName, bool isNormalMap)
{
	Texture* pNewTexture = nullptr;
	ID3D11Texture2D* pTexture = nullptr;
	ID3D11ShaderResourceView* pSRV = nullptr;
	do
	{
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage scratchImg;
		HRESULT hr = DirectX::LoadFromWICFile(fileName, DirectX::WIC_FLAGS_NONE, &metadata, scratchImg);
		if (FAILED(hr))
		{
			hr = DirectX::LoadFromDDSFile(fileName, DirectX::DDS_FLAGS_NONE, &metadata, scratchImg);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}
		}
		const DirectX::Image* pImg = scratchImg.GetImage(0, 0, 0);

		DirectX::ScratchImage convImg;
		if (isNormalMap)
		{
			// Normal Map → UNORM (Linear)
			if (metadata.format != DXGI_FORMAT_R8G8B8A8_UNORM)
			{
				hr = DirectX::Convert(
					pImg, 1, metadata,
					DXGI_FORMAT_R8G8B8A8_UNORM,
					DirectX::TEX_FILTER_DEFAULT,
					DirectX::TEX_THRESHOLD_DEFAULT,
					convImg
				);
				if (FAILED(hr))
				{
					DEBUG_BREAK();
					break;
				}
				pImg = convImg.GetImage(0, 0, 0);
				metadata.format = DXGI_FORMAT_R8G8B8A8_UNORM;
			}
		}
		else
		{
			if (metadata.format != DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
			{
				hr = DirectX::Convert(
					pImg, 1, metadata,
					DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
					DirectX::TEX_FILTER_DEFAULT,
					DirectX::TEX_THRESHOLD_DEFAULT,
					convImg
				);
				if (FAILED(hr))
				{
					DEBUG_BREAK();
					break;
				}

				pImg = convImg.GetImage(0, 0, 0);
				metadata.format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			}
		}

		hr = DirectX::CreateTexture(GRenderer->Device(), pImg, 1, metadata, (ID3D11Resource**)&pTexture);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = metadata.format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = (UINT)metadata.mipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		hr = GRenderer->Device()->CreateShaderResourceView(pTexture, &srvDesc, &pSRV);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}

		pNewTexture = new Texture;
		if (false == pNewTexture->Init(pTexture, pSRV))
		{
			DEBUG_BREAK();
			break;
		}

		return pNewTexture;

	} while (true);

	if (nullptr != pSRV)
	{
		pSRV->Release();
		pSRV = nullptr;
	}
	if (nullptr != pTexture)
	{
		pTexture->Release();
		pTexture = nullptr;
	}
	if (nullptr != pNewTexture)
	{
		pNewTexture->Release();
		pNewTexture = nullptr;
	}
	return nullptr;
}

ITexture* __stdcall RenderResourceFactory::CreateTexture(const TextureDesc& desc)
{
	HRESULT hr;
	Texture* pNewTexture = nullptr;
	ID3D11Texture2D* pTexture2D = nullptr;

	do
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = desc.size_.X;
		textureDesc.Height = desc.size_.Y;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = &desc.colorData_;
		initData.SysMemPitch = sizeof(desc.colorData_);

		hr = GRenderer->Device()->CreateTexture2D(&textureDesc, &initData, &pTexture2D);
		if (FAILED(hr))
		{
			Assert("CreateTexter2D is FAIL!!");
			break;
		}

		pNewTexture = new Texture;
		if (false == pNewTexture->Init(pTexture2D))
		{
			Assert("Texture Init() is FAIL!!");
			break;
		}

		return pNewTexture;
	} while (false);


	if (nullptr == pTexture2D)
	{
		pTexture2D->Release();
		pTexture2D = nullptr;
	}
	if (nullptr == pNewTexture)
	{
		pNewTexture->Release();
		pNewTexture = nullptr;
	}

	return nullptr;
}

ITexture* RenderResourceFactory::CreateTexture(const Float2& size, DXGI_FORMAT format, uint32_t flag)
{
	D3D11_TEXTURE2D_DESC desc = { 0, };
	desc.ArraySize = 1;
	desc.Width = (UINT)size.X;
	desc.Height = (UINT)size.Y;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MipLevels = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = flag;

	return CreateTexture(desc);
}

ITexture* RenderResourceFactory::CreateTexture(const D3D11_TEXTURE2D_DESC& desc)
{
	ID3D11Texture2D* pTexture = nullptr;
	Texture* pNewTexture = nullptr;
	do
	{
		HRESULT hr = GRenderer->Device()->CreateTexture2D(&desc, nullptr, &pTexture);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}

		pNewTexture = new Texture;
		if (false == pNewTexture->Init(pTexture))
		{
			DEBUG_BREAK();
			break;
		}

		// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
		const char* debugObjectName = "ID3D11Texture2D";
		pTexture->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
		// ---------------------------------------------------------------------------

		return pNewTexture;

	} while (false);

	if (nullptr != pTexture)
	{
		pTexture->Release();
		pTexture = nullptr;
	}
	if (nullptr != pNewTexture)
	{
		pNewTexture->Release();
		pNewTexture = nullptr;
	}

	return nullptr;
}

IRenderTarget* __stdcall RenderResourceFactory::CreateRenderTarget(const RenderTargetDesc& desc)
{
	switch (desc.renderTechniqueType_)
	{
	case E_RENDER_TECHNIQUE_TYPE::Forward:
		return CreateForwardRenderTarget(desc);
		break;
	case E_RENDER_TECHNIQUE_TYPE::Deferred:
		return CreateDeferredRenderTarget(desc);
		break;
	default:
		break;
	}
	return nullptr;
}

IRenderTarget* __stdcall RenderResourceFactory::CreateForwardRenderTarget(const RenderTargetDesc& desc)
{
	Texture* pRenderTexture = nullptr;
	Texture* pDepthTexture = nullptr;
	RenderTarget* pRenderTarget = nullptr;

	ForwardRenderingDesc forwardDesc = desc.forwardDesc_;
	if (true == forwardDesc.useDepthStencil_)
	{
		if (forwardDesc.fmtDepth_ == DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
		{
			forwardDesc.fmtDepth_ = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
		}
		else if (forwardDesc.fmtDepth_ == DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT)
		{
			forwardDesc.fmtDepth_ = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
		}
	}

	do
	{
		pRenderTexture = static_cast<Texture*>(CreateTexture(desc.size_, (DXGI_FORMAT)forwardDesc.fmtColor_, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE));
		if (nullptr == pRenderTexture)
		{
			DEBUG_BREAK();
			break;
		}

		if (true == forwardDesc.useDepthStencil_)
		{
			pDepthTexture = static_cast<Texture*>(CreateTexture(desc.size_, (DXGI_FORMAT)forwardDesc.fmtDepth_, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE));
		}

		pRenderTarget = new RenderTarget;
		bool ret = pRenderTarget->Init(desc, pRenderTexture, pDepthTexture);
		if (false == ret)
		{
			DEBUG_BREAK();
			break;
		}

		return pRenderTarget;
	} while (false);

	if (nullptr != pRenderTexture)
	{
		pRenderTexture->Release();
		pRenderTexture = nullptr;
	}
	if (nullptr != pDepthTexture)
	{
		pDepthTexture->Release();
		pDepthTexture = nullptr;
	}
	if (nullptr != pRenderTarget)
	{
		pRenderTarget->Release();
		pRenderTarget = nullptr;
	}

	return nullptr;
}

IRenderTarget* __stdcall RenderResourceFactory::CreateDeferredRenderTarget(const RenderTargetDesc& desc)
{
	DeferredTarget* pDeferredTarget = nullptr;
	Texture* pRenderTextureAlbedo = nullptr;
	Texture* pRenderTextureNormal_ = nullptr;
	Texture* pRenderTextureMaterial_ = nullptr;
	Texture* pDepthTexture_ = nullptr;

	DeferredRenderingDesc deferredDesc = desc.deferredDesc_;
	// DXGI_FORMAT_D24_UNORM_S8_UINT, DXGI_FORMAT_D32_FLOAT 포맷은 Depth 전용 포맷 임. -> ShaderResource로 사용하려하면 에러 발생.
	// 따라서 Depth로도 쓰고 ShaderResource로도 쓸려면 Type 없는 포맷으로 변환 해야함.
	if (deferredDesc.fmtDepth_ == DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT)
	{
		deferredDesc.fmtDepth_ = DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
	}
	else if (deferredDesc.fmtDepth_ == DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT)
	{
		deferredDesc.fmtDepth_ = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	}

	do
	{
		pRenderTextureAlbedo = static_cast<Texture*>(CreateTexture(desc.size_, (DXGI_FORMAT)deferredDesc.fmtAlbedo_, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE));
		if (nullptr == pRenderTextureAlbedo)
		{
			Assert("pRenderTextureAlbedo = NULL");
			break;
		}
		pRenderTextureNormal_ = static_cast<Texture*>(CreateTexture(desc.size_, (DXGI_FORMAT)deferredDesc.fmtNormal_, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE));
		if (nullptr == pRenderTextureNormal_)
		{
			Assert("pRenderTextureNormal_ = NULL");
			break;
		}
		pRenderTextureMaterial_ = static_cast<Texture*>(CreateTexture(desc.size_, (DXGI_FORMAT)deferredDesc.fmtMaterial_, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE));
		if (nullptr == pRenderTextureMaterial_)
		{
			Assert("pRenderTextureMaterial_ = NULL");
			break;
		}
		pDepthTexture_ = static_cast<Texture*>(CreateTexture(desc.size_, (DXGI_FORMAT)deferredDesc.fmtDepth_, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE));
		if (nullptr == pDepthTexture_)
		{
			Assert("pDepthTexture_ = NULL");
			break;
		}

		pDeferredTarget = new DeferredTarget;
		bool ret = pDeferredTarget->Init(desc, pRenderTextureAlbedo, pRenderTextureNormal_, pRenderTextureMaterial_, pDepthTexture_);
		if (false == ret)
		{
			Assert("DeferredTarget->Init() FAIL");
			break;
		}

		return pDeferredTarget;

	} while (false);

	if (nullptr != pDeferredTarget)
	{
		pDeferredTarget->Release();
		pDeferredTarget = nullptr;
	}
	if (nullptr != pRenderTextureAlbedo)
	{
		pRenderTextureAlbedo->Release();
		pRenderTextureAlbedo = nullptr;
	}
	if (nullptr != pRenderTextureNormal_)
	{
		pRenderTextureNormal_->Release();
		pRenderTextureNormal_ = nullptr;
	}
	if (nullptr != pRenderTextureMaterial_)
	{
		pRenderTextureMaterial_->Release();
		pRenderTextureMaterial_ = nullptr;
	}
	if (nullptr != pDepthTexture_)
	{
		pDepthTexture_->Release();
		pDepthTexture_ = nullptr;
	}

	return nullptr;
}

IRasterizer* __stdcall  RenderResourceFactory::CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling)
{
	ID3D11RasterizerState* pSolidState = nullptr;
	ID3D11RasterizerState* pWireframeState = nullptr;
	Rasterizer* pNewRasterizer = nullptr;

	do
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.CullMode = backFaceCulling ? D3D11_CULL_MODE::D3D11_CULL_BACK : D3D11_CULL_MODE::D3D11_CULL_FRONT;
		desc.FrontCounterClockwise = frontCounterClockwise ? TRUE : FALSE;
		desc.FillMode = D3D11_FILL_SOLID;
		HRESULT hr = GRenderer->Device()->CreateRasterizerState(&desc, &pSolidState);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}

		desc.FillMode = D3D11_FILL_WIREFRAME;
		hr = GRenderer->Device()->CreateRasterizerState(&desc, &pWireframeState);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}

		// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
		const char* debugObjectName = "Rasterizer::pSolidState_";
		pSolidState->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
		debugObjectName = "Rasterizer::pWireframeState_";
		pWireframeState->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
		// ---------------------------------------------------------------------------

		pNewRasterizer = new Rasterizer;
		if (false == pNewRasterizer->Init(pSolidState, pWireframeState))
		{
			DEBUG_BREAK();
			break;
		}

		return pNewRasterizer;

	} while (false);


	if (nullptr != pSolidState)
	{
		pSolidState->Release();
		pSolidState = nullptr;
	}
	if (nullptr != pWireframeState)
	{
		pWireframeState->Release();
		pWireframeState = nullptr;
	}
	if (nullptr != pNewRasterizer)
	{
		pNewRasterizer->Release();
		pNewRasterizer = nullptr;
	}

	return nullptr;
}
IParticle* __stdcall RenderResourceFactory::CreateParticle(const ParticleDesc& desc)
{
	IParticle* pParticle = Particle::Create(desc);
	return pParticle;
}

IPSO* __stdcall RenderResourceFactory::CreatePipelineStateObject(const PipelineStateDesc& desc)
{
	IPSO* pPSO = PipelineStateObject::Create(desc);
	return pPSO;
}
