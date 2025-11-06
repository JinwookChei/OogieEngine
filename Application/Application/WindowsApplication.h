#pragma once

class Window;

class WindowsApplication : public IApplication
{
public:
	WindowsApplication() = delete;
	WindowsApplication
	(
		HINSTANCE hInstance, 
		PWSTR pCmdLine, 
		int nCmdShow, 
		const wchar_t* iconPath
	);

	~WindowsApplication();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef(void) override;

	ULONG __stdcall Release(void) override;

	bool __stdcall InitializeMainWindow(const wchar_t* className, const wchar_t* windowText, const Float2& windowSize) override;

	void __stdcall WinPumpMessage() override;

	bool __stdcall ApplicationQuit() override;

	void __stdcall SetShowCursor(bool show) override;

	void* __stdcall GetMainWindowHandle() override;

	const Float2& __stdcall GetMousePosition() const override;

	void __stdcall Quit() override;

	void UpdateMousePosition(const Float2& mousePos);

	const HINSTANCE HandleInstance() const;

	Window* MainWindow() const;

	const PWSTR CmdLine() const;

	const int CmdShowParam() const;
	
private:
	HINSTANCE hInstance_;

	PWSTR pCmdLine_;

	int nCmdShow_;

	const wchar_t* iconPath_;

	HICON iCon_;

	Window* mainWindow_;

	ULONG refCount_;

	bool isApplicationQuit_;

	Float2 mousePos_;
};