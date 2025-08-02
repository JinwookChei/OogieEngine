#pragma once
struct IStartup;
class Engine final {
public:
	Engine();

	~Engine();

	ENGINE_API static Engine* CreateEngine();

	ENGINE_API static void DestroyEngine(Engine* engine);

	ENGINE_API bool Initialize
	(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int nCmdShow,
		const wchar_t* mainWindowClassName,
		const wchar_t* mainWindowText,
		const wchar_t* iConPath,
		IStartup* startup
	);

	ENGINE_API void Run();


private:
	bool LoadApplication
	(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int nCmdShow,
		const wchar_t* mainWindowClassName,
		const wchar_t* mainWindowText,
		const wchar_t* iConPath
	);

	bool LoadRenderer();

	bool InitializeStartUp(IStartup* startUp);

	void CleanUp();

	IStartup* startUp_;

	IApplication* application_;

	IRenderer* renderer_;

	HMODULE applicationModule_;

	HMODULE rendererModule_;


	// TEST : Actor Class
	IVertex* mesh_;

	IShader* vertexShader_;

	IShader* pixelShader_;

};