#pragma once

struct IImguiManager : public IUnknown
{
	virtual  float __stdcall EnableDpiAwareness() = 0;

	virtual bool __stdcall InitImgui(IApplication* pApplication, IRenderer* pRenderer, float dpiScale) = 0;

	virtual  void __stdcall CleanUpImgui() = 0;

	virtual  void __stdcall RenderImgui() = 0;

	virtual bool __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};