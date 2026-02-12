#pragma once

struct IEditor : public IUnknown
{
	virtual bool __stdcall Init(IApplication* pApplication, IRenderer* pRenderer, float dpiScale) = 0;

	virtual bool __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	virtual  float __stdcall EnableDpiAwareness() = 0;

		
	virtual  void __stdcall OnRender() = 0;

	virtual bool __stdcall BindCamera(IEditorBindCamera* pCamera) = 0;

	virtual bool __stdcall BindPickedActor(IEditorBindPickedActor* pPickedActor) = 0;
};
