#pragma once
struct IStartup;
class Application;
class RenderDevice;
class World;

class Engine final {
public:
	Engine();

	~Engine();

	ENGINE_API static Engine* CreateEngine();

	ENGINE_API static void DestroyEngine(Engine* pEngine);

	ENGINE_API bool Initialize
	(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int nCmdShow,
		const wchar_t* pMainWindowClassName,
		const wchar_t* pMainWindowText,
		const wchar_t* pIConPath,
		IStartup* pStartup
	);

	ENGINE_API void Run();

	ENGINE_API World* GetWorld() const;

private:
	bool LoadApplication
	(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int nCmdShow,
		const wchar_t* pMainWindowClassName,
		const wchar_t* pMainWindowText,
		const wchar_t* pIConPath
	);

	bool LoadRenderDevice();

	bool InitializeStartUp(IStartup* pStartUp);

	bool InitializeWorld();

	void CleanUp();

	IStartup* pStartUp_;

	Application* pApplication_;

	RenderDevice* pRenderDevice_;

	World* pWorld_;
};