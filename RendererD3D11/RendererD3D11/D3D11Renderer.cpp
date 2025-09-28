#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "SamplerState.h"
#include "ConstantBuffer.h"
#include "Rasterizer.h"


D3D11Renderer* GRenderer = nullptr;

D3D11Renderer::D3D11Renderer()
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

D3D11Renderer::~D3D11Renderer()
{
	CleanUp();
}

HRESULT __stdcall D3D11Renderer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall D3D11Renderer::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall D3D11Renderer::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

bool __stdcall D3D11Renderer::Initialize(void* hWnd, uint32_t width, uint32_t height)
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

void __stdcall D3D11Renderer::RenderBegin()
{
	drawCallCount_ = 0;

	pBackBuffer_->Clear();

	pBackBuffer_->Setting();
}

void __stdcall D3D11Renderer::RenderEnd()
{
	pSwapChain_->Present(0, 0);
}

uint64_t __stdcall D3D11Renderer::DrawCallCount()
{
	return drawCallCount_;
}

IInputLayout* __stdcall D3D11Renderer::CreateLayout(IVertex* vertex, IShader* vertexShader)
{
	InputLayout* pNewInputLayout = new InputLayout;
	if (false == pNewInputLayout->Create(vertex, vertexShader))
	{
		pNewInputLayout->Release();
		pNewInputLayout = nullptr;
	}

	return pNewInputLayout;
}

IVertex* __stdcall D3D11Renderer::CreateVertex(void* vertices, uint32_t vertexSize, uint32_t vertexCount, void* indices, uint32_t indexTypeSize, uint32_t indexCount)
{
	VertexBuffer* vertexBuffer = new VertexBuffer;
	if (nullptr == vertexBuffer)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	if (false == vertexBuffer->Initialize(vertices, (UINT)vertexSize, (UINT)vertexCount, indices, (UINT)indexTypeSize, (UINT)indexCount))
	{
		vertexBuffer->Release();
		return nullptr;
	}

	return vertexBuffer;
}

IConstantBuffer* __stdcall D3D11Renderer::CreateConstantBuffer(uint32_t bufferSize)
{
	ConstantBuffer* buffer = new ConstantBuffer;
	if (false == buffer->CreateBuffer(bufferSize))
	{
		buffer->Release();
		buffer = nullptr;
	}
	return buffer;
}

IShader* __stdcall D3D11Renderer::CreateShader(ShaderType shaderType, const wchar_t* pPath)
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
	case ShaderType::Vertex:
		pShader = new VertexShader;
		break;
	case ShaderType::Pixel:
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

ISamplerState* __stdcall D3D11Renderer::CreateSampler(bool linear, bool clamp)
{
	SamplerState* pSampler = new SamplerState;
	if (false == pSampler->CreateSampler(linear, clamp))
	{
		pSampler->Release();
		pSampler = nullptr;
	}

	return pSampler;
}

IRasterizer* __stdcall D3D11Renderer::CreateRasterizer(bool back)
{
	Rasterizer* pRasterizer = new Rasterizer;
	if (false == pRasterizer->CreateRasterizer(false, back))
	{
		pRasterizer->Release();
		pRasterizer = nullptr;
	}

	return pRasterizer;
}

IRenderTarget* __stdcall D3D11Renderer::CreateRenderTarget(const Float2& size, const Color& clearColor)
{
	Texture* pTexture = Texture::Create(size, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
	if (nullptr == pTexture)
	{
		return nullptr;
	}

	RenderTarget* pRenderTarget = new RenderTarget;
	if (nullptr == pRenderTarget)
	{
		DEBUG_BREAK();
		pTexture->Release();
		return nullptr;
	}
	if (false == pRenderTarget->SetTexture(pTexture))
	{
		pRenderTarget->Release();
		return nullptr;
	}
	
	pRenderTarget->SetClearColor(clearColor);

	return pRenderTarget;
}

ID3D11Device*  D3D11Renderer::Device()
{
	return pDevice_;
}

ID3D11DeviceContext* D3D11Renderer::DeviceContext()
{
	return pDeviceContext_;
}

void D3D11Renderer::IncrementDrawCall()
{
	++drawCallCount_;
}

IDXGIAdapter* D3D11Renderer::GetBestAdapter()
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

bool D3D11Renderer::CreateSwapChain(UINT width, UINT height)
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

bool D3D11Renderer::CreateBackBuffer()
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

	Texture* pNewTexture = new Texture;
	if (nullptr == pNewTexture)
	{
		pBackBufferTexture->Release();
		DEBUG_BREAK();
		return false;
	}

	if (false == pNewTexture->SetTexture(pBackBufferTexture))
	{
		pNewTexture->Release();
		return false;
	}

	pBackBuffer_ = new RenderTarget;
	if (nullptr == pBackBuffer_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == pBackBuffer_->SetTexture(pNewTexture))
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == pBackBuffer_->CreateDepthTexture())
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void D3D11Renderer::CleanUp()
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
