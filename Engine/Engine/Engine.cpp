#include "stdafx.h"
#include "StartUp.h"
#include "Level.h"
#include "World.h"
#include "Engine.h"
#include "Test.h"
#include "Actor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "InputLayout.h"

InputManager* GInputManager = nullptr;
TimeManager* GTimeManager = nullptr;

typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);
typedef bool (*DLL_FUNCTION_ARG1)(void**);

Engine::Engine()
	: pStartUp_(nullptr),
	pWorld_(nullptr),
	pApplication_(nullptr),
	pRenderer_(nullptr),
	applicationModule_(nullptr),
	rendererModule_(nullptr)
{
}

Engine::~Engine()
{
	CleanUp();
}

Engine* Engine::CreateEngine()
{
	return new Engine;
}

void Engine::DestroyEngine(Engine* pEngine)
{
	if (nullptr == pEngine)
	{
		DEBUG_BREAK();
		return;
	}

	delete pEngine;
}

bool Engine::Initialize
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* pMainWindowClassName,
	const wchar_t* pMainWindowText,
	const wchar_t* pIConPath,
	IStartup* pStartup
)
{
	if (nullptr == pStartup)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == LoadApplication(hInstance, pCmdLine, nCmdShow, pMainWindowClassName, pMainWindowText, pIConPath))
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

	GTimeManager = new TimeManager;
	if (nullptr == GTimeManager)
	{
		DEBUG_BREAK();
		return false;
	}


	if (false == InitializeStartUp(pStartup))
	{
		return false;
	}

	if (false == InitializeWorld())
	{
		return false;
	}

	return true;
}

void Engine::Run()
{
	while (false == pApplication_->ApplicationQuit()) {

		pApplication_->WinPumpMessage();

		double deltaTime = GTimeManager->CalcDeltaTime();

		// GameLoop
		pWorld_->CheckChangeLevel();

		pWorld_->UpdateTick(deltaTime);

		// Render
		pRenderer_->RenderBegin();

		pRenderer_->RenderEnd();
	}



	// --------------------- Ragacy --------------------------------
	//std::vector<SimpleVertex> sphereVertices;
	//std::vector<WORD> sphereIndices;
	//CreateSphere(&sphereVertices, &sphereIndices);
	//IVertex* pSphereVertex = renderer_->CreateVertex(sphereVertices.data(), (uint32_t)sizeof(SimpleVertex), (uint32_t)sphereVertices.size(), sphereIndices.data(), (uint32_t)sizeof(WORD), (uint32_t)sphereIndices.size());

	//// Mesh
	//Mesh* pMeshSphere = new Mesh;
	//pMeshSphere->pVertex_ = pSphereVertex;
	//pMeshSphere->pVertex_->AddRef();
	//pMeshSphere->pVertex_->AddInputLayout("POSITION", 0, 6, 0, false);
	//pMeshSphere->pVertex_->AddInputLayout("COLOR", 0, 2, 0, false);
	//pMeshSphere->pVertex_->AddInputLayout("NORMAL", 0, 6, 0, false);
	//pMeshSphere->pVertex_->AddInputLayout("TEXCOORD", 0, 16, 0, false);
	//pMeshSphere->pVertex_->AddInputLayout("TANGENT", 0, 2, 0, false);

	//// Material
	//Material* pMaterialSphere = new Material;
	//IMaterial* pMaterial = renderer_->CreateMaterial();
	//pMaterialSphere->pMaterial_ = pMaterial;
	//pMaterialSphere->pMaterial_->AddRef();
	//IShader* pVertexShader = renderer_->CreateShader(ShaderType::Vertex, L"VertexShader.cso");
	//IShader* pPixelShader = renderer_->CreateShader(ShaderType::Pixel, L"PixelShader.cso");
	//pMaterialSphere->pMaterial_->SetVertexShader(pVertexShader);
	//pMaterialSphere->pMaterial_->SetPixelShader(pPixelShader);
	//pMaterialSphere->pVertexShader_ = pVertexShader;
	//pMaterialSphere->pVertexShader_->AddRef();
	//pMaterialSphere->pPixelShader_ = pPixelShader;
	//pMaterialSphere->pPixelShader_->AddRef();

	//// InputLayout
	//InputLayout* pInputLayoutSphere = new InputLayout;
	//IInputLayout* pInputLayout = renderer_->CreateLayout(pSphereVertex, pVertexShader);
	//pInputLayoutSphere->pInputLayout_ = pInputLayout;
	//pInputLayoutSphere->pInputLayout_->AddRef();

	//Actor* actor = new Actor;
	//actor->pRenderer_ = new Renderer;
	//actor->pRenderer_->pMesh_ = pMeshSphere;
	//actor->pRenderer_->pMaterial_ = pMaterialSphere;
	//actor->pRenderer_->pInputLayout_ = pInputLayoutSphere;

	//IConstantBuffer* constantBuffer = renderer_->CreateConstantBuffer((uint32_t)sizeof(ConstantBuffer));
	//if (nullptr == constantBuffer) {
	//	DEBUG_BREAK();
	//	return;
	//}

	//Vector moveDir{ 0.0f, 0.0f, 0.0f };
	//Vector actorPosition{ 0.0f, 0.0f, 0.0f };

	//while (false == application_->ApplicationQuit()) {

	//	application_->WinPumpMessage();

	//	// Calc Tick
	//	ULONGLONG curTick = GetTickCount64();
	//	if (prevUpdateTick_ == 0) {
	//		prevUpdateTick_ = curTick;
	//	}
	//	
	//	unsigned long long deltaTick = curTick - prevUpdateTick_;
	//	prevUpdateTick_ = curTick;

	//	if (deltaTick < 16)
	//	{
	//	}
	//	else if (20 <= deltaTick)
	//	{
	//		deltaTick = 16;
	//	}
	//	
	//	// Game Loop
	//	{

	//		InputManager::Instance()->Tick(deltaTick);
	//	}


	//	// render
	//	renderer_->RenderBegin();
	//	{
	//		moveDir = { 0.0f, 0.0f , 0.0f };
	//		if (InputManager::Instance()->IsPress('W'))
	//		{
	//			moveDir.X += 0.01f;
	//		}
	//		if (InputManager::Instance()->IsPress('S'))
	//		{
	//			moveDir.X -= 0.01f;
	//		}
	//		if (InputManager::Instance()->IsPress('A'))
	//		{
	//			moveDir.Y -= 0.01f;
	//		}
	//		if (InputManager::Instance()->IsPress('D'))
	//		{
	//			moveDir.Y += 0.01f;
	//		}
	//		if (InputManager::Instance()->IsPress('Q'))
	//		{
	//			moveDir.Z -= 0.01f;
	//		}
	//		if (InputManager::Instance()->IsPress('E'))
	//		{
	//			moveDir.Z += 0.01f;
	//		}
	//		actorPosition += moveDir;


	//		DirectX::XMMATRIX position = DirectX::XMMatrixTranslation(actorPosition.X, actorPosition.Y, actorPosition.Z);
	//		DirectX::XMMATRIX world = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) * position;
	//		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(-10.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	//		DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f);

	//		// 상수 버퍼 업데이트
	//		ConstantBuffer cb;
	//		cb.world = DirectX::XMMatrixTranspose(world);
	//		cb.view = DirectX::XMMatrixTranspose(view);
	//		cb.projection = DirectX::XMMatrixTranspose(projection);

	//		actor->pRenderer_->Setting();
	//		constantBuffer->Update(&cb);
	//		constantBuffer->VSSetting(0);
	//		constantBuffer->PSSetting(0);
	//		actor->pRenderer_->Draw();
	//	}

	//	renderer_->RenderEnd();
	//}



	//constantBuffer->Release();
	//pSphereVertex->Release();
	//pMaterial->Release();
	//pVertexShader->Release();
	//pPixelShader->Release();
	//pInputLayout->Release();
	//if (nullptr != actor)
	//{
	//	delete actor;
	//	actor = nullptr;
	//}
	//if (nullptr != startUp_)
	//{
	//	startUp_->End();
	//	startUp_->Release();
	//	startUp_ = nullptr;
	//}




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

World* Engine::GetWorld() const
{
	return pWorld_;
}

bool Engine::LoadApplication
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* pMainWindowClassName,
	const wchar_t* pMainWindowText,
	const wchar_t* pIConPath
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

	CreateWindowsApplication((void**)&pApplication_, hInstance, pCmdLine, nCmdShow, pIConPath);
	if (nullptr == pApplication_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == pApplication_->InitializeMainWindow(pMainWindowClassName, pMainWindowText))
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

	CreateRenderer((void**)&pRenderer_);
	if (nullptr == pRenderer_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == pRenderer_->Initialize(pApplication_->GetMainWindowHandle(), 800, 600))
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

	pStartUp_ = startUp;
	pStartUp_->AddRef();

	if (false == pStartUp_->Start())
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Engine::InitializeWorld()
{
	pWorld_ = new World;
	if (nullptr == pWorld_)
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void Engine::CleanUp()
{
	if (nullptr != pWorld_)
	{
		delete pWorld_;
		pWorld_ = nullptr;
	}

	if (nullptr != pStartUp_)
	{
		pStartUp_->Release();
		pStartUp_ = nullptr;
	}

	if (nullptr != GTimeManager)
	{
		delete GTimeManager;
		GTimeManager = nullptr;
	}

	if (nullptr != GInputManager)
	{
		delete GInputManager;
		GInputManager = nullptr;
	}

	if (nullptr != pRenderer_)
	{
		pRenderer_->Release();
		pRenderer_ = nullptr;
	}

	if (nullptr != pApplication_)
	{
		pApplication_->Release();
		pApplication_ = nullptr;
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
