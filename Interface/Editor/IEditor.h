#pragma once

//enum class E_IMGUI_WIDGET_TYPE
//{
//	TextureWidget = 0,
//};

//struct ImGuiWidgetDesc
//{
//	E_IMGUI_WIDGET_TYPE widgetType;
//
//	void* textureResourceView;
//
//	char* text;
//};

struct IEditor : public IUnknown
{
	virtual bool __stdcall Init(IApplication* pApplication, IRenderer* pRenderer, float dpiScale) = 0;

	virtual bool __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	virtual  float __stdcall EnableDpiAwareness() = 0;

	//virtual  void __stdcall CleanUp() = 0;

	//virtual  void __stdcall OnUpdate() = 0;
		
	virtual  void __stdcall OnRender() = 0;

	virtual bool __stdcall BindCamera(IEditorBindCamera* pCamera) = 0;

	virtual bool __stdcall BindPickedActor(IEditorBindPickedActor* pPickedActor) = 0;
};

//
//struct IImGuiWidget : public IUnknown
//{
//
//};
//
//struct IImGuiPanel : public IUnknown
//{
//
//};