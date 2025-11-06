#pragma once

namespace ImguiSystem
{
	IMGUI_API IImguiManager* GetImguiManager();
}

class ImguiManager 
	: public IImguiManager
{
public:
	ImguiManager();

	virtual ~ImguiManager();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef(void) override;

	ULONG __stdcall Release(void) override;

	float __stdcall EnableDpiAwareness() override;

	bool __stdcall InitImgui(IApplication* pApplication, IRenderer* pRenderer, float dpiScale) override;

	void __stdcall CleanUpImgui() override;

	void __stdcall RenderImgui() override; 

	bool __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
private:
	void CleanUp();

	ULONG refCount_;

	IApplication* pApplication_;

	IRenderer* pRenderer_;

	float dpiScale_;
};


