#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"

#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"


#include "D3D11Renderer.h"



D3D11Renderer* GRenderer = nullptr;

D3D11Renderer::D3D11Renderer()
	: coInit_(false),
	refCount_(1),
	hWnd_(nullptr),
	device_(nullptr),
	deviceContext_(nullptr),
	swapChain_(nullptr),
	backBuffer_(nullptr),
	mesh_(nullptr),
	vertexShader_(nullptr),
	pixelShader_(nullptr),
	inputLayout_(nullptr)
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

bool __stdcall D3D11Renderer::Initialize(void* hWnd, UINT width, UINT height)
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

void __stdcall D3D11Renderer::BeginRender()
{
	backBuffer_->Clear();

	backBuffer_->Setting();
}

void __stdcall D3D11Renderer::EndRender()
{
	swapChain_->Present(0, 0);
}

void __stdcall D3D11Renderer::Render()
{
	deviceContext_->Draw(3, 0);
}

bool D3D11Renderer::CreateTriangle()
{
	SimpleVertex vertices[] = {
{DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
{DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	// Shader 코드
	const char* g_VS = R"(
struct VS_INPUT
{
    float3 pos : POSITION;
	float4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
	float4 color : COLOR;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.pos = float4(input.pos, 1.0f);
	output.color = input.color;
    return output;
}
)";

	const char* g_PS = R"(
struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 PS(PS_INPUT input) : SV_Target
{
	return input.color; // 빨간색으로 출력
}
)";
	mesh_ = new VertexBuffer;
	if (nullptr == mesh_)
	{
		return false;
	}

	if (false == mesh_->Initialize(vertices, sizeof(SimpleVertex)*_countof(vertices)))
	{
		mesh_->Release();
		mesh_ = nullptr;
		return false;
	}

	// Vertex Shader
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = D3DCompile(g_VS, strlen(g_VS), nullptr, nullptr, nullptr, "VS", "vs_4_0", 0, 0, &pVSBlob, nullptr);
	if (FAILED(hr))
	{
		mesh_->Release();
		mesh_ = nullptr;
		return false;
	}

	vertexShader_ = new VertexShader;
	if (nullptr == vertexShader_)
	{
		pVSBlob->Release();
		mesh_->Release();
		mesh_ = nullptr;
		return false;
	}

	if (false == vertexShader_->CreateShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize()))
	{
		mesh_->Release();
		mesh_ = nullptr;
		vertexShader_->Release();
		vertexShader_ = nullptr;
		return false;
	}

	// Layout
	//mesh_->AddInputLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, false);
	//mesh_->AddInputLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, false);
	//ID3D11InputLayout* inputLayout = nullptr;
	//hr = device_->CreateInputLayout(mesh_->LayoutDesc().data(), (UINT)mesh_->LayoutDesc().size(), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &inputLayout);
	//if (FAILED(hr))
	//{
	//	mesh_->Release();
	//	mesh_ = nullptr;
	//	vertexShader_->Release();
	//	vertexShader_ = nullptr;
	//	return false;
	//}

	D3D11_INPUT_ELEMENT_DESC layout[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = device_->CreateInputLayout(layout, ARRAYSIZE(layout), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &inputLayout_);
	pVSBlob->Release();
	if (FAILED(hr))
	{
		mesh_->Release();
		mesh_ = nullptr;
		vertexShader_->Release();
		vertexShader_ = nullptr;
		return false;
	}

	// Pixel Shader
	ID3DBlob* pPSBlob = nullptr;
	hr = D3DCompile(g_PS, strlen(g_PS), nullptr, nullptr, nullptr, "PS", "ps_4_0", 0, 0, &pPSBlob, nullptr);
	if (FAILED(hr))
	{
		mesh_->Release();
		mesh_ = nullptr;
		vertexShader_->Release();
		vertexShader_ = nullptr;
		inputLayout_->Release();
		inputLayout_ = nullptr;
		return false;
	}
	pixelShader_ = new PixelShader;

	if (nullptr == pixelShader_)
	{
		pPSBlob->Release();
		mesh_->Release();
		mesh_ = nullptr;
		vertexShader_->Release();
		vertexShader_ = nullptr;
		inputLayout_->Release();
		inputLayout_ = nullptr;
		return false;
	}

	if (false == pixelShader_->CreateShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize()))
	{
		mesh_->Release();
		mesh_ = nullptr;
		vertexShader_->Release();
		vertexShader_ = nullptr;
		pixelShader_->Release();
		pixelShader_ = nullptr;
		inputLayout_->Release();
		inputLayout_ = nullptr;
		return false;
	}
	pPSBlob->Release();

	deviceContext_->IASetInputLayout(inputLayout_);
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	deviceContext_->IASetVertexBuffers(0, 1, &mesh_->vertexBuffer_, &stride, &offset);
	deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext_->VSSetShader(vertexShader_->shader_, nullptr, 0);
	deviceContext_->PSSetShader(pixelShader_->shader_, nullptr, 0);

	return true;
}

ID3D11Device* D3D11Renderer::Device()
{
	return device_;
}

ID3D11DeviceContext* D3D11Renderer::DeviceContext()
{
	return deviceContext_;
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

void D3D11Renderer::CleanUp()
{
	if (nullptr != inputLayout_)
	{
		inputLayout_->Release();
		inputLayout_ = nullptr;
	}

	if (nullptr != mesh_)
	{
		mesh_->Release();
		mesh_ = nullptr;
	}
	if (nullptr != vertexShader_)
	{
		vertexShader_->Release();
		vertexShader_ = nullptr;
	}
	if (nullptr != pixelShader_)
	{
		pixelShader_->Release();
		pixelShader_ = nullptr;
	}

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
