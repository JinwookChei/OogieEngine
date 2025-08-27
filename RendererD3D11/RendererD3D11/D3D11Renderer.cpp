#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "SamplerState.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "Rasterizer.h"


D3D11Renderer* GRenderer = nullptr;

D3D11Renderer::D3D11Renderer()
	: coInit_(false),
	refCount_(1),
	drawCallCount_(0),
	hWnd_(nullptr),
	device_(nullptr),
	deviceContext_(nullptr),
	swapChain_(nullptr),
	backBuffer_(nullptr)
{
	GRenderer = this;
}

D3D11Renderer::~D3D11Renderer()
{
	Cleanup();
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
		&device_,
		&featureLevel,
		&deviceContext_
	);

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

	if (false == CreateRenderTarget())
	{
		return false;
	}

	return true;
}

void __stdcall D3D11Renderer::RenderBegin()
{
	drawCallCount_ = 0;

	backBuffer_->Clear();

	backBuffer_->Setting();
}

void __stdcall D3D11Renderer::RenderEnd()
{
	swapChain_->Present(0, 0);
}

uint64_t __stdcall D3D11Renderer::DrawCallCount()
{
	return drawCallCount_;
}

IInputLayout* __stdcall D3D11Renderer::CreateLayout(IVertex* vertex, IShader* vertexShader)
{
	InputLayout* newInputLayout = new InputLayout;
	if (false == newInputLayout->Create(vertex, vertexShader))
	{
		newInputLayout->Release();
		newInputLayout = nullptr;
	}

	return newInputLayout;
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

IShader* __stdcall D3D11Renderer::CreateShader(ShaderType shaderType, const wchar_t* path)
{
	ID3DBlob* blob = nullptr;
	HRESULT hr = D3DReadFileToBlob(path, &blob);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	BaseShader* shader = nullptr;
	switch (shaderType)
	{
	case ShaderType::Vertex:
		shader = new VertexShader;
		break;
	case ShaderType::Pixel:
		shader = new PixelShader;
		break;
	}

	if (nullptr != shader && false == shader->CreateShader(blob))
	{
		shader->Release();
		shader = nullptr;
	}

	return shader;
}

IMaterial* __stdcall D3D11Renderer::CreateMaterial()
{
	return new Material;

}
ISamplerState* __stdcall D3D11Renderer::CreateSampler(bool linear, bool clamp)
{
	SamplerState* sampler = new SamplerState;
	if (false == sampler->CreateSampler(linear, clamp))
	{
		sampler->Release();
		sampler = nullptr;
	}

	return sampler;
}

IRasterizer* __stdcall D3D11Renderer::CreateRasterizer(bool back)
{
	Rasterizer* rasterizer = new Rasterizer;
	if (false == rasterizer->CreateRasterizer(false, back))
	{
		rasterizer->Release();
		rasterizer = nullptr;
	}

	return rasterizer;
}

ID3D11Device*  D3D11Renderer::Device()
{
	return device_;
}

ID3D11DeviceContext* D3D11Renderer::DeviceContext()
{
	return deviceContext_;
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

	IDXGIDevice* swapDevice = nullptr;
	IDXGIAdapter* swapAdapter = nullptr;
	IDXGIFactory* swapFactory = nullptr;

	HRESULT hr = device_->QueryInterface(__uuidof(IDXGIDevice), (void**)&swapDevice);
	if (FAILED(hr) || nullptr == swapDevice)
	{
		DEBUG_BREAK();
		goto lb_return;
	}

	hr = swapDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&swapAdapter);
	if (FAILED(hr) || nullptr == swapAdapter)
	{
		DEBUG_BREAK();
		goto lb_return;
	}

	hr = swapAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&swapFactory);
	if (FAILED(hr) || nullptr == swapFactory)
	{
		DEBUG_BREAK();
		goto lb_return;
	}

	hr = swapFactory->CreateSwapChain(device_, &sd, &swapChain_);
	if (FAILED(hr) || nullptr == swapChain_)
	{
		DEBUG_BREAK();
		goto lb_return;
	}

lb_return:
	if (nullptr != swapDevice)
	{
		swapDevice->Release();
		swapDevice = nullptr;
	}
	if (nullptr != swapAdapter)
	{
		swapAdapter->Release();
		swapAdapter = nullptr;
	}
	if (nullptr != swapFactory)
	{
		swapFactory->Release();
		swapFactory = nullptr;
	}
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}
	return true;
}

bool D3D11Renderer::CreateRenderTarget()
{
	ID3D11Texture2D* backBufferTexture = nullptr;

	HRESULT hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBufferTexture);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr == backBufferTexture)
	{
		DEBUG_BREAK();
		return false;
	}

	Texture* newTexture = new Texture;
	if (nullptr == newTexture)
	{
		backBufferTexture->Release();
		DEBUG_BREAK();
		return false;
	}

	if (false == newTexture->SetTexture(backBufferTexture))
	{
		newTexture->Release();
		return false;
	}

	backBuffer_ = new RenderTarget;
	if (nullptr == backBuffer_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == backBuffer_->SetTexture(newTexture))
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == backBuffer_->CreateDepthTexture())
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void D3D11Renderer::Cleanup()
{

	if (nullptr != deviceContext_)
	{
		deviceContext_->ClearState();
	}

	if (nullptr != backBuffer_)
	{
		backBuffer_->Release();
		backBuffer_ = nullptr;
	}
	if (nullptr != swapChain_)
	{
		swapChain_->Release();
		swapChain_ = nullptr;
	}
	if (nullptr != deviceContext_)
	{
		deviceContext_->Release();
		deviceContext_ = nullptr;
	}
	if (nullptr != device_)
	{
		device_->Release();
		device_ = nullptr;
	}
	if (coInit_)
	{
		CoUninitialize();
	}
}
