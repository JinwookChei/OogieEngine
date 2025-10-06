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

	// ---------------- �޸� ���� ������ �̸� ����. ----------------------------
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
	if (nullptr == vertex || nullptr == vertexShader)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	D3D11InputLayout* pNewInputLayout = new D3D11InputLayout;
	if (false == pNewInputLayout->Create(vertex, vertexShader))
	{
		pNewInputLayout->Release();
		pNewInputLayout = nullptr;
	}

	return pNewInputLayout;
}

IVertex* __stdcall D3D11Renderer::CreateVertex(void* vertices, uint32_t vertexSize, uint32_t vertexCount, void* indices, uint32_t indexTypeSize, uint32_t indexCount)
{
	D3D11VertexBuffer* vertexBuffer = new D3D11VertexBuffer;
	if (false == vertexBuffer->Initialize(vertices, (UINT)vertexSize, (UINT)vertexCount, indices, (UINT)indexTypeSize, (UINT)indexCount))
	{
		vertexBuffer->Release();
		return nullptr;
	}

	return vertexBuffer;
}

IConstantBuffer* __stdcall D3D11Renderer::CreateConstantBuffer(uint32_t bufferSize)
{
	D3D11ConstantBuffer* buffer = new D3D11ConstantBuffer;
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

	D3D11BaseShader* pShader = nullptr;
	switch (shaderType)
	{
	case ShaderType::Vertex:
		pShader = new D3D11VertexShader;
		break;
	case ShaderType::Pixel:
		pShader = new D3D11PixelShader;
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
	D3D11SamplerState* pSampler = new D3D11SamplerState;
	if (false == pSampler->CreateSampler(linear, clamp))
	{
		pSampler->Release();
		pSampler = nullptr;
	}

	return pSampler;
}

IRasterizer* __stdcall D3D11Renderer::CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling)
{
	D3D11Rasterizer* pRasterizer = new D3D11Rasterizer;
	if (false == pRasterizer->CreateRasterizer(frontCounterClockwise, backFaceCulling))
	{
		pRasterizer->Release();
		pRasterizer = nullptr;
	}

	return pRasterizer;
}

IRenderTarget* __stdcall D3D11Renderer::CreateRenderTarget(const Float2& size, const Color& clearColor, bool useDepthStencil /*= true*/)
{
	D3D11RenderTarget* pRenderTarget = new D3D11RenderTarget;

	D3D11Texture* pRenderTexture = D3D11Texture::Create(size, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE);
	if (nullptr == pRenderTexture)
	{
		return nullptr;
	}

	D3D11Texture* pDepthTexture = nullptr;
	if (true == useDepthStencil)
	{
		pDepthTexture = D3D11Texture::Create(pRenderTexture->Size(), DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
	}

	pRenderTarget->SetClearColor(clearColor);
	if (false == pRenderTarget->SetTexture(pRenderTexture, pDepthTexture))
	{
		pRenderTarget->Release();
		return nullptr;
	}

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
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // ���÷����� ��ĵ���� ��� ����� �����ϴ� �Ӽ� 
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; // ������� ũ��� ���� ��� ����̽� �ػ󵵰� �ٸ� �� ��� ũ�⸦ �������� ����
	sd.SampleDesc.Count = 1;	// ��Ƽ ���ø� ��.
	sd.SampleDesc.Quality = 0;	// ��Ƽ ���ø� ǰ��.
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �� ���� �뵵.
	sd.OutputWindow = hWnd_;
	sd.Windowed = TRUE; // â ���.
	sd.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;	// 	����� ������Ʈ �� ó�� ��� ����
	sd.Flags = 0; // ���� ü���� ���ۿ� ���� �߰����� �ɼ��� �����ϴ� ��Ʈ �÷���. �ɼ� ����.

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

	D3D11Texture* pRenderTexture = new D3D11Texture;
	if (false == pRenderTexture->SetTexture(pBackBufferTexture))
	{
		pRenderTexture->Release();
		return false;
	}

	D3D11Texture* pDepthTexture = D3D11Texture::Create(pRenderTexture->Size(), DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
	if (nullptr == pDepthTexture)
	{
		DEBUG_BREAK();
		pRenderTexture->Release();
		return false;
	}

	pBackBuffer_ = new D3D11RenderTarget;
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


	//if (false == pBackBuffer_->CreateDepthTexture())
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}

	
	//ID3D11Texture2D* pBackBufferTexture = nullptr;

	//HRESULT hr = pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture);
	//if (FAILED(hr))
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}

	//if (nullptr == pBackBufferTexture)
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}

	//D3D11Texture* pNewTexture = new D3D11Texture;
	//if (nullptr == pNewTexture)
	//{
	//	pBackBufferTexture->Release();
	//	DEBUG_BREAK();
	//	return false;
	//}

	//if (false == pNewTexture->SetTexture(pBackBufferTexture))
	//{
	//	pNewTexture->Release();
	//	return false;
	//}

	//pBackBuffer_ = new D3D11RenderTarget;
	//if (nullptr == pBackBuffer_)
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}

	//if (false == pBackBuffer_->SetTexture(pNewTexture))
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}

	//if (false == pBackBuffer_->CreateDepthTexture())
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}

	//return true;
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
