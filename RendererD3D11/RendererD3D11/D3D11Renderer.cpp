#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Mesh.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "SamplerState.h"
#include "ConstantBuffer.h"
#include "Rasterizer.h"


Renderer* GRenderer = nullptr;

Renderer::Renderer()
	: coInit_(false),
	refCount_(1),
	drawCallCount_(0),
	hWnd_(nullptr),
	pDevice_(nullptr),
	pDeviceContext_(nullptr),
	pSwapChain_(nullptr),
	pBackBuffer_(nullptr)
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

	if (false == CreateSwapChain(width, height))
	{
		return false;
	}

	if (false == CreateBackBuffer())
	{
		return false;
	}

	return true;
}

void __stdcall Renderer::RenderBegin()
{
	drawCallCount_ = 0;

	pBackBuffer_->Clear();

	pBackBuffer_->Setting();
}

void __stdcall Renderer::RenderEnd()
{
	pSwapChain_->Present(0, 0);
}

uint64_t __stdcall Renderer::DrawCallCount()
{
	return drawCallCount_;
}

IInputLayout* __stdcall Renderer::CreateLayout(IMesh* pMesh, IShader* pVertexShadr)
{
	if (nullptr == pMesh || nullptr == pVertexShadr)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	Mesh* pMeshImpl = (Mesh*)pMesh;
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc = pMeshImpl->GetDesc();

	VertexShader* pVertexShaderImpl = (VertexShader*)pVertexShadr;
	ID3D11InputLayout* pInputLayout = nullptr;
	HRESULT hr = GRenderer->Device()->CreateInputLayout(layoutDesc.data(), (UINT)layoutDesc.size(), pVertexShaderImpl->GetBufferPointer(), pVertexShaderImpl->GetBufferSize(), &pInputLayout);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "InputLayout::pInputLayout_";
	pInputLayout->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------


	InputLayout* newInputLayout = new InputLayout(pInputLayout);
	return newInputLayout;
}

IMesh* __stdcall Renderer::CreateMesh(void* pVertices, uint32_t vertexSize, uint32_t vertexCount, void* pIndices /*= nullptr*/, uint32_t indexTypeSize /*= 0*/, uint32_t indexCount /*= 0*/)
{
	if (nullptr == pVertices)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	if (0 == vertexSize || 0 == vertexCount)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0x00, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertexSize * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	memset(&InitData, 0x00, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = pVertices;								// pSysMem: 리소스 생성 시 GPU로 복사할 시스템 메모리상의 데이터 포인터입니다.


	ID3D11Buffer* pVertexBuffer;
	HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "VertexBuffer::pVertexBuffer_";
	pVertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	if (nullptr == pIndices && 0 != indexCount)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	if (nullptr == pIndices)
	{
		Mesh* pNewMesh = new Mesh(vertexSize, pVertexBuffer, 0, nullptr);
		return pNewMesh;
	}

	if (0 == indexCount)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	// 인덱스 버퍼
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = indexTypeSize * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = pIndices;

	ID3D11Buffer* pIndexBuffer;
	hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, &pIndexBuffer);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	debugObjectName = "VertexBuffer::pIndexBuffer_";
	pIndexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	Mesh* pNewMesh = new Mesh(vertexSize, pVertexBuffer, indexCount, pIndexBuffer);
	return pNewMesh;
}

IMaterial* __stdcall Renderer::CreateMaterial(const wchar_t* VS, const wchar_t* PS, bool samplerLinear, bool samplerClamp)
{
	VertexShader* pVertexShader = nullptr;
	if (nullptr != VS)
	{
		pVertexShader = (VertexShader*)CreateShader(EShaderType::Vertex, VS);
	}

	PixelShader* pPixelShader = nullptr;
	if (nullptr != PS)
	{
		pPixelShader = (PixelShader*)CreateShader(EShaderType::Pixel, PS);
	}

	SamplerState* pSamplerState = nullptr;
	pSamplerState = (SamplerState*)CreateSampler(samplerLinear, samplerClamp);


	Material* pMaterial = new Material(pVertexShader, pPixelShader, pSamplerState);
	return pMaterial;
}

IConstantBuffer* __stdcall Renderer::CreateConstantBuffer(uint32_t bufferSize)
{
	D3D11_BUFFER_DESC desc{};
	memset(&desc, 0x00, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = bufferSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;

	ID3D11Buffer* pBuffer = nullptr;
	HRESULT hr = GRenderer->Device()->CreateBuffer(&desc, nullptr, &pBuffer);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "ConstantBuffer::pBuffer_";
	pBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	ConstantBuffer* pNewInputLayout = new ConstantBuffer(pBuffer);
	return pNewInputLayout;
}

IShader* __stdcall Renderer::CreateShader(EShaderType shaderType, const wchar_t* pPath)
{
	ID3DBlob* pBlob = nullptr;
	HRESULT hr = D3DReadFileToBlob(pPath, &pBlob);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	BaseShader* pShader = nullptr;
	switch (shaderType)
	{
	case EShaderType::Vertex:
		pShader = new VertexShader;
		break;
	case EShaderType::Pixel:
		pShader = new PixelShader;
		break;
	}

	if (nullptr != pShader && false == pShader->CreateShader(pBlob))
	{
		pShader->Release();
		pShader = nullptr;
	}

	return pShader;
}

ISamplerState* __stdcall Renderer::CreateSampler(bool linear, bool clamp)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;

	ID3D11SamplerState* pSamplerState = nullptr;
	HRESULT hr = GRenderer->Device()->CreateSamplerState(&samplerDesc, &pSamplerState);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "SamplerState::pSamplerState_";
	pSamplerState->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	SamplerState* pNewSamplerState = new SamplerState(pSamplerState);

	return pNewSamplerState;
}

IRasterizer* __stdcall Renderer::CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling)
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = backFaceCulling ? D3D11_CULL_MODE::D3D11_CULL_BACK : D3D11_CULL_MODE::D3D11_CULL_FRONT;
	desc.FrontCounterClockwise = frontCounterClockwise ? TRUE : FALSE;

	ID3D11RasterizerState* pSolidState = nullptr;
	desc.FillMode = D3D11_FILL_SOLID;
	HRESULT hr = GRenderer->Device()->CreateRasterizerState(&desc, &pSolidState);
	if (FAILED(hr))
	{
		return nullptr;
	}

	desc.FillMode = D3D11_FILL_WIREFRAME;
	ID3D11RasterizerState* pWireframeState = nullptr;
	hr = GRenderer->Device()->CreateRasterizerState(&desc, &pWireframeState);
	if (FAILED(hr))
	{
		return nullptr;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "Rasterizer::pSolidState_";
	pSolidState->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	debugObjectName = "Rasterizer::pWireframeState_";
	pWireframeState->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	Rasterizer* pNewRasterizer = new Rasterizer(pSolidState, pWireframeState);
	pNewRasterizer->SetFillMode(EFillModeType::Solid);

	return pNewRasterizer;
}

IRenderTarget* __stdcall Renderer::CreateRenderTarget(const Float2& size, const Color& clearColor, bool useDepthStencil /*= true*/)
{
	RenderTarget* pRenderTarget = new RenderTarget;

	Texture* pRenderTexture = Texture::Create(size, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
	if (nullptr == pRenderTexture)
	{
		return nullptr;
	}

	Texture* pDepthTexture = nullptr;
	if (true == useDepthStencil)
	{
		pDepthTexture = Texture::Create(pRenderTexture->Size(), DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
	}

	pRenderTarget->SetClearColor(clearColor);
	if (false == pRenderTarget->SetTexture(pRenderTexture, pDepthTexture))
	{
		pRenderTarget->Release();
		return nullptr;
	}

	return pRenderTarget;
}

ID3D11Device*  Renderer::Device()
{
	return pDevice_;
}

ID3D11DeviceContext* Renderer::DeviceContext()
{
	return pDeviceContext_;
}

void Renderer::IncrementDrawCall()
{
	++drawCallCount_;
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

bool Renderer::CreateSwapChain(UINT width, UINT height)
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

bool Renderer::CreateBackBuffer()
{
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HRESULT hr = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}
	if (nullptr == pBackBufferTexture)
	{
		DEBUG_BREAK();
		return false;
	}

	Texture* pRenderTexture = new Texture;
	if (false == pRenderTexture->SetTexture(pBackBufferTexture))
	{
		pRenderTexture->Release();
		return false;
	}

	Texture* pDepthTexture = Texture::Create(pRenderTexture->Size(), DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
	if (nullptr == pDepthTexture)
	{
		DEBUG_BREAK();
		pRenderTexture->Release();
		return false;
	}

	pBackBuffer_ = new RenderTarget;
	if (nullptr == pBackBuffer_)
	{
		DEBUG_BREAK();
		pRenderTexture->Release();
		pDepthTexture->Release();
		return false;

	}

	if (false == pBackBuffer_->SetTexture(pRenderTexture, pDepthTexture))
	{
		DEBUG_BREAK();
		pBackBuffer_->Release();
		pRenderTexture->Release();
		pDepthTexture->Release();
		return false;
	}

	return true;
}

void Renderer::CleanUp()
{

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
