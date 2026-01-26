#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "DeferredTarget.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Particle.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "ConstantBuffer.h"
#include "Rasterizer.h"
#include "DebugPass.h"
#include "GeometryPass.h"
#include "LightPass.h"
#include "ParticlePass.h"
#include "MergePass.h"
#include "FinalPass.h"

Renderer* GRenderer = nullptr;

Renderer::Renderer()
	: coInit_(false)
	, refCount_(1)
	, drawCallCount_(0)
	, hWnd_(nullptr)
	, pDevice_(nullptr)
	, pDeviceContext_(nullptr)
	, pSwapChain_(nullptr)
	, pBackBuffer_(nullptr)
	, pGeometryPass_(nullptr)
	, pLightPass_(nullptr)
	, pParticlePass_(nullptr)
	, pDebugPass_(nullptr)
	, pMergePass_(nullptr)
	, pFinalPass_(nullptr)
{
	GRenderer = this;
}

Renderer::~Renderer()
{
	CleanUp();
}

HRESULT __stdcall Renderer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Renderer::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall Renderer::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

bool __stdcall Renderer::Initialize(void* hWnd, uint32_t width, uint32_t height)
{
	if (nullptr == hWnd)
	{
		DEBUG_BREAK();
		return false;
	}

	hWnd_ = (HWND)hWnd;

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	coInit_ = true;

	IDXGIAdapter* pBestAdapter = GetBestAdapter();
	if (nullptr == pBestAdapter)
	{
		return false;
	}

	int createDeviceFlag = 0;
#ifdef _DEBUG
	createDeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	hr = D3D11CreateDevice
	(
		pBestAdapter,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		createDeviceFlag,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&pDevice_,
		&featureLevel,
		&pDeviceContext_
	);

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "D3D11Renderer::pDevice_";
	pDevice_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	debugObjectName = "D3D11Renderer::pDeviceContext_";
	pDeviceContext_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	pBestAdapter->Release();
	pBestAdapter = nullptr;

	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	if (featureLevel < D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitSwapChain(width, height))
	{
		return false;
	}

	if (false == InitBackBuffer(width, height, { 0.0f, 0.0f, 0.3f, 1.0f }))
	{
		return false;
	}
	if (false == InitGeometryPass())
	{
		return false;
	}
	if (false == InitLightPass())
	{
		return false;
	}
	if (false == InitParticlePass())
	{
		return false;
	}
	if (false == InitDebugPass())
	{
		return false;
	}
	if (false == InitMergePass())
	{
		return false;
	}
	if (false == InitFinalPass())
	{
		return false;
	}

	return true;
}

void* __stdcall Renderer::GetDeviceHandle()
{
	return Device();
}

void* __stdcall Renderer::GetDeviceContextHandle()
{
	return DeviceContext();
}

void __stdcall Renderer::UpdateCameraFrame(const CameraFrameData& cameraFrameData)
{
	pGeometryPass_->UpdatePerFrameConstant(cameraFrameData);
}

void __stdcall Renderer::RenderGBuffer(const ObjectRenderData& objectData)
{
	pGeometryPass_->Render(objectData);
}

void __stdcall Renderer::RenderLightBegin(IRenderTarget* pGBufferTarget)
{
	pLightPass_->RenderBegin(pGBufferTarget);
}

void __stdcall Renderer::RenderLight(const LightRenderData& lightData)
{
	pLightPass_->Render(lightData);
}

void __stdcall Renderer::RenderLightEnd(IRenderTarget* pGBufferTarget)
{
	pLightPass_->RenderEnd(pGBufferTarget);
}

void __stdcall Renderer::UpdateParticles(IParticle* pParticle, double deltaTime)
{
	pParticlePass_->Tick(pParticle, deltaTime);
}

void __stdcall Renderer::RenderParticles(IParticle* pParticle, const Float4x4 worldTransform, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp)
{
	pParticlePass_->Render(pParticle, worldTransform, viewProj, cameraRight, cameraUp);
}

void __stdcall Renderer::DrawDebugLine(const Float3& start, const Float3& end, const Float4& color)
{
	pDebugPass_->DrawLine(start, end, color);
}

void __stdcall Renderer::DrawDebugRay(const Float3& origin, Float3& dir, float length, const Color& color)
{
	pDebugPass_->DrawRay(origin, dir, length, color);
}

void __stdcall Renderer::RenderDebug()
{
	pDebugPass_->Render();
}

void __stdcall Renderer::RenderMerge(IRenderTarget* pDepthTarget, IRenderTarget* pSrcTarget)
{
	pMergePass_->Render(pDepthTarget, pSrcTarget);
}

void __stdcall Renderer::RenderBegin()
{
	drawCallCount_ = 0;

	pBackBuffer_->Clear();

	pBackBuffer_->Bind();
}

void __stdcall Renderer::RenderFinal(IRenderTarget* pSrcTarget)
{
	pFinalPass_->Render(pSrcTarget);
}

void __stdcall Renderer::RenderEnd()
{
	pSwapChain_->Present(0, 0);
}

uint64_t __stdcall Renderer::DrawCallCount()
{
	return drawCallCount_;
}

IMesh* __stdcall Renderer::CreateMesh(const MeshDesc& desc)
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

IMaterial* __stdcall Renderer::CreateMaterial(const MaterialDesc& materialDesc)
{

	Material* pMaterial = nullptr;
	do
	{
		pMaterial = new Material;
		if (false == pMaterial->Init(materialDesc.shineness, materialDesc.specularColor))
		{
			DEBUG_BREAK();
			break;
		}

		return pMaterial;

	} while (false);

	if (nullptr != pMaterial)
	{
		pMaterial->Release();
		pMaterial = nullptr;
	}

	return nullptr;
}


IRasterizer* __stdcall Renderer::CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling)
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


IRenderTarget* __stdcall Renderer::CreateRenderTarget(const RenderTargetDesc& desc)
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

IRenderTarget* __stdcall Renderer::CreateForwardRenderTarget(const RenderTargetDesc& desc)
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

IRenderTarget* __stdcall Renderer::CreateDeferredRenderTarget(const RenderTargetDesc& desc)
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

IParticle* __stdcall Renderer::CreateParticle(const ParticleDesc& desc)
{
	IParticle* pParticle = Particle::Create(desc);
	return pParticle;
}

ITexture* __stdcall Renderer::LoadTextureFromDirectXTex(const wchar_t* fileName, bool isNormalMap)
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

		hr = DirectX::CreateTexture(pDevice_, pImg, 1, metadata, (ID3D11Resource**)&pTexture);
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

		hr = pDevice_->CreateShaderResourceView(pTexture, &srvDesc, &pSRV);
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

ITexture* __stdcall Renderer::CreateTexture(const TextureDesc& desc)
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


ITexture* Renderer::CreateTexture(const Float2& size, DXGI_FORMAT format, uint32_t flag)
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

ITexture* Renderer::CreateTexture(const D3D11_TEXTURE2D_DESC& desc)
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

ID3D11ShaderResourceView* CreateWhiteTextureSRV(ID3D11Device* device)
{
	UINT color = 0xFFFFFFFF;

	D3D11_TEXTURE2D_DESC td = {};
	td.Width = 1;
	td.Height = 1;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = &color;
	initData.SysMemPitch = sizeof(UINT);

	ID3D11Texture2D* tex = nullptr;
	if (FAILED(device->CreateTexture2D(&td, &initData, &tex)))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	ID3D11ShaderResourceView* srv = nullptr;
	if (FAILED(device->CreateShaderResourceView(tex, nullptr, &srv)))
	{
		DEBUG_BREAK();
		tex->Release();
		return nullptr;
	}

	tex->Release();
	return srv;
}


void Renderer::Draw(UINT count, bool useIndex)
{
	if(useIndex)
	{
		GRenderer->DeviceContext()->DrawIndexed(count, 0, 0);
	}
	else
	{
		GRenderer->DeviceContext()->Draw(count, 0);
	}

	GRenderer->IncrementDrawCall();
}

void Renderer::IncrementDrawCall()
{
	++drawCallCount_;
}


ID3D11Device* Renderer::Device()
{
	return pDevice_;
}

ID3D11DeviceContext* Renderer::DeviceContext()
{
	return pDeviceContext_;
}

IDXGIAdapter* Renderer::GetBestAdapter()
{
	IDXGIFactory* pFactory = nullptr;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	IDXGIAdapter* pBestAdapter = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	size_t maxDedicatedVedioMemory = 0;

	for (UINT n = 0; pFactory->EnumAdapters(n, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++n)
	{
		DXGI_ADAPTER_DESC desc;
		pAdapter->GetDesc(&desc);

		if (maxDedicatedVedioMemory < desc.DedicatedVideoMemory)
		{
			if (pBestAdapter)
			{
				pBestAdapter->Release();
			}
			pBestAdapter = pAdapter;
			maxDedicatedVedioMemory = desc.DedicatedVideoMemory;
		}
		else
		{
			pAdapter->Release();
		}
	}

	pFactory->Release();
	return pBestAdapter;
}

bool Renderer::InitSwapChain(UINT width, UINT height)
{
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0x00, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 디스플레이의 스캔라인 출력 방식을 지정하는 속성 
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; // 백버퍼의 크기와 실제 출력 디바이스 해상도가 다를 때 어떻게 크기를 조정할지 지정
	sd.SampleDesc.Count = 1;	// 멀티 샘플링 수.
	sd.SampleDesc.Quality = 0;	// 멀티 샘플링 품질.
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백 버퍼 용도.
	sd.OutputWindow = hWnd_;
	sd.Windowed = TRUE; // 창 모드.
	sd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;	// 	백버퍼 프레젠트 후 처리 방식 지정
	sd.Flags = 0; // 스왑 체인의 동작에 대한 추가적인 옵션을 지정하는 비트 플래그. 옵션 없음.

	IDXGIDevice* pSwapDevice = nullptr;
	IDXGIAdapter* pSwapAdapter = nullptr;
	IDXGIFactory* pSwapFactory = nullptr;

	HRESULT hr = pDevice_->QueryInterface(__uuidof(IDXGIDevice), (void**)&pSwapDevice);
	if (FAILED(hr) || nullptr == pSwapDevice)
	{
		DEBUG_BREAK();
		goto lb_return;
	}

	hr = pSwapDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pSwapAdapter);
	if (FAILED(hr) || nullptr == pSwapAdapter)
	{
		DEBUG_BREAK();
		goto lb_return;
	}

	hr = pSwapAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pSwapFactory);
	if (FAILED(hr) || nullptr == pSwapFactory)
	{
		DEBUG_BREAK();
		goto lb_return;
	}

	hr = pSwapFactory->CreateSwapChain(pDevice_, &sd, &pSwapChain_);
	if (FAILED(hr) || nullptr == pSwapChain_)
	{
		DEBUG_BREAK();
		goto lb_return;
	}

lb_return:
	if (nullptr != pSwapDevice)
	{
		pSwapDevice->Release();
		pSwapDevice = nullptr;
	}
	if (nullptr != pSwapAdapter)
	{
		pSwapAdapter->Release();
		pSwapAdapter = nullptr;
	}
	if (nullptr != pSwapFactory)
	{
		pSwapFactory->Release();
		pSwapFactory = nullptr;
	}
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}
	return true;
}

bool Renderer::InitBackBuffer(UINT width, UINT height, const Color& clearColor)
{
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	Texture* pRenderTexture = nullptr;
	Texture* pDepthTexture = nullptr;

	do
	{
		HRESULT hr = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}
		if (nullptr == pBackBufferTexture)
		{
			DEBUG_BREAK();
			break;
		}

		pRenderTexture = new Texture;
		if (false == pRenderTexture->Init(pBackBufferTexture))
		{
			DEBUG_BREAK();
			break;
		}

		pBackBuffer_ = new RenderTarget;
		RenderTargetDesc desc{ E_RENDER_TECHNIQUE_TYPE::Forward };
		desc.size_ = { (float)width, (float)height };
		desc.clearColor_ = clearColor;
		desc.forwardDesc_.useDepthStencil_ = false;

		bool ret = pBackBuffer_->Init(desc, pRenderTexture, nullptr);
		if (false == ret)
		{
			DEBUG_BREAK();
			break;
		}

		return true;

	} while (false);

	if (nullptr != pBackBufferTexture)
	{
		pBackBufferTexture->Release();
		pBackBufferTexture = nullptr;
	}
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
	if (nullptr != pBackBuffer_)
	{
		pBackBuffer_->Release();
		pBackBuffer_ = nullptr;
	}
	return false;
}

bool Renderer::InitGeometryPass()
{
	pGeometryPass_ = new GeometryPass;
	if (false == pGeometryPass_->Init())
	{
		DEBUG_BREAK();
		pGeometryPass_->Release();
		pGeometryPass_ = nullptr;
		return false;
	}

	return true;
}

bool Renderer::InitLightPass()
{
	pLightPass_ = new LightPass;
	if (false == pLightPass_->Init())
	{
		DEBUG_BREAK();
		pLightPass_->Release();
		pLightPass_ = nullptr;
		return false;
	}

	return true;
}

bool Renderer::InitParticlePass()
{
	pParticlePass_ = new ParticlePass;
	if (false == pParticlePass_->Init())
	{
		DEBUG_BREAK();
		pParticlePass_->Release();
		pParticlePass_ = nullptr;
		return false;
	}

	return true;
}

bool Renderer::InitDebugPass()
{
	pDebugPass_ = new DebugPass;
	if (false == pDebugPass_->Init())
	{
		DEBUG_BREAK();
		pDebugPass_->Release();
		pDebugPass_ = nullptr;
		return false;
	}

	return true;
}

bool Renderer::InitMergePass()
{
	pMergePass_ = new MergePass;
	if (false == pMergePass_->Init())
	{
		DEBUG_BREAK();
		pMergePass_->Release();
		pMergePass_ = nullptr;
		return false;
	}

	return true;
}

bool Renderer::InitFinalPass()
{
	pFinalPass_ = new FinalPass;
	if (false == pFinalPass_->Init())
	{
		DEBUG_BREAK();
		pFinalPass_->Release();
		pFinalPass_ = nullptr;
		return false;
	}

	return true;
}

void Renderer::CleanUp()
{
	if (nullptr != pFinalPass_)
	{
		pFinalPass_->Release();
		pFinalPass_ = nullptr;
	}
	if (nullptr != pMergePass_)
	{
		pMergePass_->Release();
		pMergePass_ = nullptr;
	}
	if (nullptr != pDebugPass_)
	{
		pDebugPass_->Release();
		pDebugPass_ = nullptr;
	}
	if (nullptr != pParticlePass_)
	{
		pParticlePass_->Release();
		pParticlePass_ = nullptr;
	}


	if (nullptr != pLightPass_)
	{
		pLightPass_->Release();
		pLightPass_ = nullptr;
	}

	if (nullptr != pGeometryPass_)
	{
		pGeometryPass_->Release();
		pGeometryPass_ = nullptr;
	}

	if (nullptr != pDeviceContext_)
	{
		pDeviceContext_->ClearState();
	}

	if (nullptr != pBackBuffer_)
	{
		pBackBuffer_->Release();
		pBackBuffer_ = nullptr;
	}
	if (nullptr != pSwapChain_)
	{
		pSwapChain_->Release();
		pSwapChain_ = nullptr;
	}
	if (nullptr != pDeviceContext_)
	{
		pDeviceContext_->Release();
		pDeviceContext_ = nullptr;
	}
	if (nullptr != pDevice_)
	{
		pDevice_->Release();
		pDevice_ = nullptr;
	}
	if (coInit_)
	{
		CoUninitialize();
	}
}

