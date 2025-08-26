#pragma once

class Application final
{
public:
	Application();

	virtual ~Application();

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

	static Application* Instance();

	void WinPumpMessage();

	bool ApplicationQuit();

	void SetShowCursor(bool show);

	void* GetMainWindowHandle();

private:

	void CleanUp();

	IApplication* pApplicationInterface_;
	
	HMODULE applicationModule_;
};
