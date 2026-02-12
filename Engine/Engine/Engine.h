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

	bool InitializeStartUp(IStartup* pStartUp);

	void CleanUp();

	IStartup* pStartUp_;
};