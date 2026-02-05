#pragma once

class Application
{
private:
	friend class Engine;
	friend class Renderer;
	static HMODULE GApplicationModule;
	static IApplication* GApplication;
	static bool LoadApplication
	(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int nCmdShow,
		const wchar_t* pMainWindowClassName,
		const wchar_t* pMainWindowText,
		const wchar_t* pIConPath
	);

public:
	static IApplication* Instance();
	static void ShutDown();
};
