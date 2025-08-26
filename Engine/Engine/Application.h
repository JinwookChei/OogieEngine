#pragma once

class Application final
{
public:
	friend Engine;

	Application();

	virtual ~Application();

	static Application* Instance();

	bool InitializeMainWindow(const wchar_t* className, const wchar_t* windowText);

	void WinPumpMessage();

	bool ApplicationQuit();

	void SetShowCursor(bool show);

	void* GetMainWindowHandle();

private:
	void SetApplicationInterface(IApplication* pApplication);

	void SetApplicationModule(HMODULE appModule);

	void CleanUp();

	IApplication* pApplicationInterface_;
	
	HMODULE applicationModule_;
};
