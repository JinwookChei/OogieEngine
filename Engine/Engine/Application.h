#pragma once

class Application final
{
public:
	Application();

	virtual ~Application();

	static Application* Create();

	static void Destroy();

	static Application* Instance();

	bool Load
	(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int nCmdShow,
		const wchar_t* pMainWindowClassName,
		const wchar_t* pMainWindowText,
		const wchar_t* pIConPath,
		LPCWSTR libFileName
	);

	void WinPumpMessage();

	bool ApplicationQuit();

	void SetShowCursor(bool show);

	void* GetMainWindowHandle();

private:

	void CleanUp();

	IApplication* pApplicationImpl_;
	
	HMODULE applicationModule_;
};
