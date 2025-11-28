#pragma once

namespace ImGuiSystem
{
	IMGUI_API IImGuiManager* GetImGuiManager();
}

class ExitPopup;
class GBufferViewer;
class ImGuiManager 
	: public IImGuiManager
{
public:
	ImGuiManager();

	virtual ~ImGuiManager();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef(void) override;

	ULONG __stdcall Release(void) override;

	float __stdcall EnableDpiAwareness() override;

	bool __stdcall InitImGui(IApplication* pApplication, IRenderer* pRenderer, float dpiScale) override;

	void __stdcall CleanUpImGui() override;

	void __stdcall OnRender() override; 

	bool __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

	bool __stdcall BindCamera(IImGuiBindCamera* pCamera) override;

	bool __stdcall BindPickedActor(IImGuiBindPickedActor* pPickedActor) override;
	
private:
	void CleanUp();

	ULONG refCount_;

	IApplication* pApplication_;

	IRenderer* pRenderer_;

	float dpiScale_;

	// Panel
	ExitPopup* pExitPopup_;

	GBufferViewer* pGBufferViewer_;
	// Panel End
};


