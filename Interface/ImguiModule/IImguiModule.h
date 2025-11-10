#pragma once

enum class E_IMGUI_WIDGET_TYPE
{
	TextureWidget = 0,
};

struct ImGuiWidgetDesc
{
	E_IMGUI_WIDGET_TYPE widgetType;

	void* textureResourceView;

	char* text;
};

struct IImGuiManager : public IUnknown
{
	virtual  float __stdcall EnableDpiAwareness() = 0;

	virtual bool __stdcall InitImGui(IApplication* pApplication, IRenderer* pRenderer, float dpiScale) = 0;

	virtual  void __stdcall CleanUpImGui() = 0;

	virtual  void __stdcall OnRender() = 0;

	virtual bool __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	virtual bool __stdcall BindCamera(IImGuiBindCamera* pCamera) = 0;
};


struct IImGuiWidget : public IUnknown
{

};

struct IImGuiPanel : public IUnknown
{

};