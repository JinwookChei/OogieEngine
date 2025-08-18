#include "stdafx.h"
#include "StartUp.h"
#include "Engine.h"
#include "Test.h"


#include "Actor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "InputLayout.h"


InputManager* GInputManager = nullptr;

typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);
typedef bool (*DLL_FUNCTION_ARG1)(void**);

Engine::Engine()
	: startUp_(nullptr),
	application_(nullptr),
	renderer_(nullptr),
	applicationModule_(nullptr),
	rendererModule_(nullptr)
{
}

Engine::~Engine()
{
	Cleanup();
}

Engine* Engine::CreateEngine()
{
	return new Engine;
}

void Engine::DestroyEngine(Engine* engine)
{
	if (nullptr == engine)
	{
		DEBUG_BREAK();
		return;
	}

	delete engine;
}

bool Engine::Initialize
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* mainWindowClassName,
	const wchar_t* mainWindowText,
	const wchar_t* iConPath,
	IStartup* startup
)
{
	if (nullptr == startup)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == LoadApplication(hInstance, pCmdLine, nCmdShow, mainWindowClassName, mainWindowText, iConPath))
	{
		return false;
	}

	if (false == LoadRenderer())
	{
		return false;
	}

	GInputManager = new InputManager;
	if (nullptr == GInputManager)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == GInputManager->Initialize())
	{
		return false;
	}

	if (false == InitializeStartUp(startup))
	{
		return false;
	}

	return true;
}

void Engine::Run()
{
	std::vector<SimpleVertex> sphereVertices;
	std::vector<WORD> sphereIndices;
	CreateSphere(&sphereVertices, &sphereIndices);
	IVertex* pSphereVertex = renderer_->CreateVertex(sphereVertices.data(), (uint32_t)sizeof(SimpleVertex), (uint32_t)sphereVertices.size(), sphereIndices.data(), (uint32_t)sizeof(WORD), (uint32_t)sphereIndices.size());

	// Mesh
	Mesh* pMeshSphere = new Mesh;
	pMeshSphere->pVertex_ = pSphereVertex;
	pMeshSphere->pVertex_->AddInputLayout("POSITION", 0, 6, 0, false);
	pMeshSphere->pVertex_->AddInputLayout("COLOR", 0, 2, 0, false);
	pMeshSphere->pVertex_->AddInputLayout("NORMAL", 0, 6, 0, false);
	pMeshSphere->pVertex_->AddInputLayout("TEXCOORD", 0, 16, 0, false);
	pMeshSphere->pVertex_->AddInputLayout("TANGENT", 0, 2, 0, false);
	
	// Material
	Material* pMaterialSphere = new Material;
	pMaterialSphere->pMaterial_ = renderer_->CreateMaterial();
	IShader* pVertexShader = renderer_->CreateShader(ShaderType::Vertex, L"VertexShader.cso");
	IShader* pPixelShader = renderer_->CreateShader(ShaderType::Pixel, L"PixelShader.cso");
	pMaterialSphere->pMaterial_->SetVertexShader(pVertexShader);
	pMaterialSphere->pMaterial_->SetPixelShader(pPixelShader);
	pMaterialSphere->pVertexShader_ = pVertexShader;
	pMaterialSphere->pPixelShader_ = pPixelShader;

	// InputLayout
	InputLayout* pInputLayoutSphere = new InputLayout;
	pInputLayoutSphere->pInputLayout_ = renderer_->CreateLayout(pSphereVertex, pVertexShader);;

	Actor* actor = new Actor;
	actor->pRenderer_ = new Renderer;
	actor->pRenderer_->pMesh_ = pMeshSphere;
	actor->pRenderer_->pMaterial_ = pMaterialSphere;
	actor->pRenderer_->pInputLayout_ = pInputLayoutSphere;

	IConstantBuffer* constantBuffer = renderer_->CreateConstantBuffer((uint32_t)sizeof(ConstantBuffer));
	if (nullptr == constantBuffer) {
		DEBUG_BREAK();
		return;
	}
	// 월드, 뷰, 프로젝션 행렬 설정
	// 스케일 * 자전 * 이동 * 공전 * 부모
	DirectX::XMMATRIX parentPosition = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX world = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) * parentPosition;
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(-10.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f);

	// 상수 버퍼 업데이트
	ConstantBuffer cb;
	cb.world = DirectX::XMMatrixTranspose(world);
	cb.view = DirectX::XMMatrixTranspose(view);
	cb.projection = DirectX::XMMatrixTranspose(projection);

	while (false == application_->ApplicationQuit()) {

		application_->WinPumpMessage();

		renderer_->RenderBegin();

		// render
		{
			actor->pRenderer_->Setting();
			constantBuffer->Update(&cb);
			constantBuffer->VSSetting(0);
			constantBuffer->PSSetting(0);
			actor->pRenderer_->Draw();
		}

		renderer_->RenderEnd();
	}

	constantBuffer->Release();
	if (nullptr != startUp_)
	{
		startUp_->End();
		startUp_->Release();
		startUp_ = nullptr;
	}




	//	------------------------------------------------------------ TEST 용 --------------------------------------------------------------
	//std::vector<SimpleVertex> vertices;
	//std::vector<WORD> indices;
	//if (false == CreateSphere(&vertices, &indices)) {
	//	DEBUG_BREAK();
	//	return;
	//}

	//// 버텍스, 인덱스 버퍼 생성.
	//IVertex* vertex = renderer_->CreateVertex(vertices.data(), (uint32_t)sizeof(SimpleVertex), (uint32_t)vertices.size(), indices.data(), (uint32_t)sizeof(WORD), (uint32_t)indices.size());
	//if (nullptr == vertex) {
	//	DEBUG_BREAK();
	//	return;
	//}
	//vertex->AddInputLayout("POSITION", 0, 6, 0, false);
	//vertex->AddInputLayout("COLOR", 0, 2, 0, false);
	//vertex->AddInputLayout("NORMAL", 0, 6, 0, false);
	//vertex->AddInputLayout("TEXCOORD", 0, 16, 0, false);
	//vertex->AddInputLayout("TANGENT", 0, 2, 0, false);

	//IConstantBuffer* constantBuffer = renderer_->CreateConstantBuffer((uint32_t)sizeof(ConstantBuffer));
	//if (nullptr == constantBuffer) {
	//	DEBUG_BREAK();
	//	return;
	//}

	//IShader* vertexShader = renderer_->CreateShader(ShaderType::Vertex, L"VertexShader.cso");
	//if (nullptr == vertexShader)
	//{
	//	DEBUG_BREAK();
	//	return;
	//}

	//IShader* pixelShader = renderer_->CreateShader(ShaderType::Pixel, L"PixelShader.cso");
	//if (nullptr == pixelShader)
	//{
	//	DEBUG_BREAK();
	//	return;
	//}

	//IInputLayout* layout = renderer_->CreateLayout(vertex, vertexShader);
	//if (nullptr == layout)
	//{
	//	DEBUG_BREAK();
	//	return;
	//}

	//IMaterial* material = renderer_->CreateMaterial();
	//if (nullptr == material)
	//{
	//	DEBUG_BREAK();
	//	return;
	//}
	//material->SetVertexShader(vertexShader);
	//material->SetPixelShader(pixelShader);
	////material->SetSampler(sampler, 0);


	//// 월드, 뷰, 프로젝션 행렬 설정
	//// 스케일 * 자전 * 이동 * 공전 * 부모
	//DirectX::XMMATRIX parentPosition = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	//DirectX::XMMATRIX world = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) * parentPosition;
	//DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(-10.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	//DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f);

	//// 상수 버퍼 업데이트
	//ConstantBuffer cb;
	//cb.world = DirectX::XMMatrixTranspose(world);
	//cb.view = DirectX::XMMatrixTranspose(view);
	//cb.projection = DirectX::XMMatrixTranspose(projection);

	//while (false == application_->ApplicationQuit()) {

	//	application_->WinPumpMessage();

	//	renderer_->RenderBegin();

	//	// render
	//	{
	//		material->Setting();
	//		layout->Setting();
	//		vertex->Setting();

	//		//rasterizer->SetFillMode(FillModeType::WireFrame);
	//		//rasterizer->Setting();

	//		constantBuffer->Update(&cb);
	//		constantBuffer->VSSetting(0);
	//		constantBuffer->PSSetting(0);

	//		vertex->Draw();
	//	}

	//	renderer_->RenderEnd();
	//}

	//vertexShader->Release();
	//pixelShader->Release();
	//vertex->Release();
	//layout->Release();
	////sampler->Release();
	//material->Release();
	//constantBuffer->Release();
	////rasterizer->Release();

	//if (nullptr != startUp_)
	//{
	//	startUp_->End();
	//	startUp_->Release();
	//	startUp_ = nullptr;
	//}
}

bool Engine::LoadApplication
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* mainWindowClassName,
	const wchar_t* mainWindowText,
	const wchar_t* iConPath
)
{
	applicationModule_ = LoadLibrary(L"Application_x64_Debug.dll");
	if (!applicationModule_)
	{
		DEBUG_BREAK();
		return false;
	}

	DLL_FUNCTION_ARG5 CreateWindowsApplication = (DLL_FUNCTION_ARG5)GetProcAddress(applicationModule_, "CreateWindowsApplication");
	if (!CreateWindowsApplication)
	{
		DEBUG_BREAK();
		return false;
	}

	CreateWindowsApplication((void**)&application_, hInstance, pCmdLine, nCmdShow, iConPath);
	if (nullptr == application_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == application_->InitializeMainWindow(mainWindowClassName, mainWindowText))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Engine::LoadRenderer()
{
	rendererModule_ = LoadLibrary(L"RendererD3D11_x64_Debug.dll");
	if (!rendererModule_)
	{
		DEBUG_BREAK();
		return false;
	}

	DLL_FUNCTION_ARG1 CreateRenderer = (DLL_FUNCTION_ARG1)GetProcAddress(rendererModule_, "CreateRenderer");
	if (!CreateRenderer)
	{
		DEBUG_BREAK();
		return false;
	}

	CreateRenderer((void**)&renderer_);
	if (nullptr == renderer_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == renderer_->Initialize(application_->GetMainWindowHandle(), 800, 600))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Engine::InitializeStartUp(IStartup* startUp)
{
	if (nullptr == startUp)
	{
		DEBUG_BREAK();
		return false;
	}

	startUp_ = startUp;
	startUp_->AddRef();

	if (false == startUp_->Start())
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void Engine::Cleanup()
{
	if (nullptr != startUp_)
	{
		startUp_->Release();
		startUp_ = nullptr;
	}

	if (nullptr != GInputManager)
	{
		delete GInputManager;
		GInputManager = nullptr;
	}

	if (nullptr != renderer_)
	{
		renderer_->Release();
		renderer_ = nullptr;
	}

	if (nullptr != application_)
	{
		application_->Release();
		application_ = nullptr;
	}

	if (nullptr != rendererModule_)
	{
		FreeLibrary(rendererModule_);
		rendererModule_ = nullptr;
	}
	if (nullptr != applicationModule_)
	{
		FreeLibrary(applicationModule_);
		applicationModule_ = nullptr;
	}
}
