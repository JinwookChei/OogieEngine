#pragma once

struct IApplication : public IUnknown {
	virtual bool __stdcall InitializeMainWindow(const wchar_t* className, const wchar_t* windowText, const Float2& windowSize) = 0;

	virtual void __stdcall WinPumpMessage() = 0;

	virtual bool __stdcall ApplicationQuit() = 0;

	virtual void __stdcall SetShowCursor(bool show) = 0;

	virtual void* __stdcall GetMainWindowHandle() = 0;

	virtual const Float2& __stdcall GetMousePosition() const = 0;

	virtual void __stdcall Quit() = 0;
};