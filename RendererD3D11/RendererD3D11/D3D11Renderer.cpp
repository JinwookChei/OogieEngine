#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "DeferredTarget.h"
#include "Shader.h"
#include "Mesh.h"
#include "Material.h"
#include "Particle.h"
#include "DepthState.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "ConstantBuffer.h"
#include "Rasterizer.h"
#include "PipelineStateObject.h"
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
	//, pGeometryPass_(nullptr)
	//, pLightPass_(nullptr)
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
	//if (false == InitGeometryPass())
	//{
	//	return false;
	//}
	//if (false == InitLightPass())
	//{
	//	return false;
	//}
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

	Shader::InitGlobalShaders();
	ConstantBuffer::InitGlobalConstant();
	SamplerState::InitGlobalSamplers();
	DepthState::InitGlobalDepthStates();
	BlendState::InitGlobalBlendStates();
	Rasterizer::InitGlobalRasterizer();

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
	//pGeometryPass_->UpdatePerFrameConstant(cameraFrameData);
	Float4x4 viewTrans;
	MATH::MatrixTranspose(viewTrans, cameraFrameData.view);
	Float4x4 projectionTrans;
	MATH::MatrixTranspose(projectionTrans, cameraFrameData.projection);
	Float4x4 invViewTrans;
	MATH::MatrixInverse(invViewTrans, cameraFrameData.view);
	MATH::MatrixTranspose(invViewTrans, invViewTrans);
	Float4x4 invProjectionTrans;
	MATH::MatrixInverse(invProjectionTrans, cameraFrameData.projection);
	MATH::MatrixTranspose(invProjectionTrans, invProjectionTrans);

	CBPerFrame cbPerFrame;
	cbPerFrame.camPos = cameraFrameData.camPos;
	cbPerFrame.view = viewTrans;
	cbPerFrame.projection = projectionTrans;
	cbPerFrame.inverseView = invViewTrans;
	cbPerFrame.inverseProjection = invProjectionTrans;
	cbPerFrame.screenOffset = cameraFrameData.screenOffset;
	cbPerFrame.screenScale = cameraFrameData.screenScale;
	cbPerFrame.screenResolution = cameraFrameData.screenResolution;

	ConstantBuffer::GConstantPerFrame->Update(&cbPerFrame);
	ConstantBuffer::GConstantPerFrame->BindConstantBufferVS(0);
	ConstantBuffer::GConstantPerFrame->BindConstantBufferPS(0);
}

void __stdcall Renderer::UpdateObjectFrame(const ObjectFrameData& objectFrameData)
{
	CBPerObject cbPerObject;
	MATH::MatrixTranspose(cbPerObject.world, objectFrameData.world);
	cbPerObject.materialShineness = 1.0f;
	cbPerObject.materialSpecular = { 0.7f, 0.7f, 0.7f };

	ConstantBuffer::GConstantPerObject->Update(&cbPerObject);
	ConstantBuffer::GConstantPerObject->BindConstantBufferVS(1);
	ConstantBuffer::GConstantPerObject->BindConstantBufferPS(1);
}

void __stdcall Renderer::UpdateLightFrame(const LightRenderData& lightFrameData)
{
	CBPerLight cbPerLight;
	cbPerLight.lightDiffuse = lightFrameData.lightDiffuse;
	cbPerLight.lightSpecular = lightFrameData.lightSpecular;
	cbPerLight.lightAmbient = lightFrameData.lightAmbient;
	cbPerLight.direction_D_S = lightFrameData.direction_D_S;
	cbPerLight.range_S_P = lightFrameData.range_S_P;
	cbPerLight.position_S_P = lightFrameData.position_S_P;
	cbPerLight.exponent_S = lightFrameData.exponent_S;
	cbPerLight.innerAngle_S = lightFrameData.innerAngle_S;
	cbPerLight.outerAngle_S = lightFrameData.outerAngle_S;
	cbPerLight.attenuationConst_S_P = lightFrameData.attenuationConst_S_P;
	cbPerLight.attenuationLinear_S_P = lightFrameData.attenuationLinear_S_P;
	cbPerLight.attenuationQuad_S_P = lightFrameData.attenuationQuad_S_P;
	cbPerLight.lightType = lightFrameData.lightType;
	cbPerLight.pad = lightFrameData.pad;

	ConstantBuffer::GConstantPerLight->Update(&cbPerLight);
	ConstantBuffer::GConstantPerLight->BindConstantBufferVS(1);
	ConstantBuffer::GConstantPerLight->BindConstantBufferPS(1);
}

//void __stdcall Renderer::RenderGBuffer(const ObjectRenderData& objectData)
//{
//	pGeometryPass_->Render(objectData);
//}

void __stdcall Renderer::RenderTest(IPSO* pipelineStateObject)
{
	PipelineStateObject* pPSO = static_cast<PipelineStateObject*>(pipelineStateObject);
	Mesh* pMesh = static_cast<Mesh*>(pPSO->GetMesh());
	Material* pMaterial = static_cast<Material*>(pPSO->GetMaterial());

	pMesh->Bind();
	pMaterial->Bind();

	switch (pPSO->GetSamplerState())
	{
		case E_SAMPLER_PRESET::LINEAR_CLAMP:
		{
			SamplerState::GSamplerLinearClamp->BindPS(0);
			break;
		}
		case E_SAMPLER_PRESET::LINEAR_WARP:
		{
			SamplerState::GSamplerLinearWarp->BindPS(0);
			break;
		}
		case E_SAMPLER_PRESET::ANISOTROPIC_CLAMP:
		{
			SamplerState::GSamplerAnisotropicClamp->BindPS(0);
			break;
		}
		case E_SAMPLER_PRESET::ANISOTROPIC_WARP:
		{
			SamplerState::GSamplerAnisotropicClamp->BindPS(0);
			break;
		}
		default:
		{
			DEBUG_BREAK();
			break;
		}
	}

	switch (pPSO->GetBlendState())
	{
		case E_BLEND_PRESET::OPAQUE_BLEND:
		{
			BlendState::GOpaqueBlend->Bind();
			break;
		}
		case E_BLEND_PRESET::ALPHA_BLEND:
		{
			BlendState::GAlphaBlend->Bind();
			break;
		}
		case E_BLEND_PRESET::ADDITIVE_BLEND:
		{
			BlendState::GAdditiveBlend->Bind();
			break;
		}
		default:
		{
			DEBUG_BREAK();
			break;
		}
	}

	switch (pPSO->GetDepthState())
	{
		case E_DEPTH_PRESET::DEPTH_ENABLE_WRITE:
		{
			DepthState::GDepthEnableWrite->Bind();
			break;
		}
		case E_DEPTH_PRESET::DEPTH_ENABLE_READ_ONLY:
		{
			DepthState::GDepthEnableReadOnly->Bind();
			break;
		}
		case E_DEPTH_PRESET::DEPTH_DISABLE:
		{
			DepthState::GDepthDisable->Bind();
			break;
		}
		default:
		{
			DEBUG_BREAK();
			break;
		}
	}

	switch (pPSO->GetRasterizerMode())
	{
		case E_RASTERIZER_PRESET::SOLID:
		{
			Rasterizer::GRasterizer->ChangeFillMode(E_FILLMODE_TYPE::SOLID);
			break;
		}
		case E_RASTERIZER_PRESET::WIRE_FRAME:
		{
			Rasterizer::GRasterizer->ChangeFillMode(E_FILLMODE_TYPE::WIREFRAME);
			break;
		}
		case E_RASTERIZER_PRESET::DISABLE:
		{
			break;
		}
		default:
		{
			DEBUG_BREAK();
			break;
		}
	}

	GRenderer->Draw(pMesh->GetIndexCount(), true);
}

void __stdcall Renderer::UnBindSRVs(bool bVS, bool bPS)
{
	ID3D11ShaderResourceView* nullSRVs[16] = {};
	if (bVS)
	{
		pDeviceContext_->VSSetShaderResources(0, 16, nullSRVs);
	}
	if (bPS)
	{
		pDeviceContext_->PSSetShaderResources(0, 16, nullSRVs);
	}
}

//
//void __stdcall Renderer::RenderLightBegin(IRenderTarget* pGBufferTarget)
//{
//	pLightPass_->RenderBegin(pGBufferTarget);
//}
//
//void __stdcall Renderer::RenderLight(const LightRenderData& lightData)
//{
//	pLightPass_->Render(lightData);
//}
//
//void __stdcall Renderer::RenderLightEnd(IRenderTarget* pGBufferTarget)
//{
//	pLightPass_->RenderEnd(pGBufferTarget);
//}

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

void __stdcall Renderer::RenderMerge(IRenderTarget* pSrcTarget)
{
	pMergePass_->Render(pSrcTarget);
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

//bool Renderer::InitGeometryPass()
//{
//	pGeometryPass_ = new GeometryPass;
//	if (false == pGeometryPass_->Init())
//	{
//		DEBUG_BREAK();
//		pGeometryPass_->Release();
//		pGeometryPass_ = nullptr;
//		return false;
//	}
//
//	return true;
//}

//bool Renderer::InitLightPass()
//{
//	pLightPass_ = new LightPass;
//	if (false == pLightPass_->Init())
//	{
//		DEBUG_BREAK();
//		pLightPass_->Release();
//		pLightPass_ = nullptr;
//		return false;
//	}
//
//	return true;
//}

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
	Shader::ShutDown();
	ConstantBuffer::ShutDown();
	SamplerState::ShutDown();
	DepthState::ShutDown();
	BlendState::ShutDown();
	Rasterizer::ShutDown();

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

	//if (nullptr != pLightPass_)
	//{
	//	pLightPass_->Release();
	//	pLightPass_ = nullptr;
	//}

	//if (nullptr != pGeometryPass_)
	//{
	//	pGeometryPass_->Release();
	//	pGeometryPass_ = nullptr;
	//}

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

