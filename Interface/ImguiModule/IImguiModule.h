#pragma once

enum class EWidgetType
{
	TextureWidget = 0,
};

struct ImguiWidgetDesc
{
	EWidgetType widgetType;

	void* textureResourceView;

	std::string text;
};

struct IImguiManager : public IUnknown
{
	virtual  float __stdcall EnableDpiAwareness() = 0;

	virtual bool __stdcall InitImgui(IApplication* pApplication, IRenderer* pRenderer, float dpiScale) = 0;

	virtual bool __stdcall CreateWidget(const ImguiWidgetDesc& desc) = 0;

	virtual  void __stdcall CleanUpImgui() = 0;

	virtual  void __stdcall OnRender() = 0;

	virtual bool __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};


struct IImguiWidget : public IUnknown
{

};