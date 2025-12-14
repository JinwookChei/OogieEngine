#pragma once


class EditorContext
	: public IUnknown
{
	friend class EditorCore;
	EditorContext();
	~EditorContext();

	bool Init(IApplication* pApplication, IRenderer* pRenderer, float dpiScale);
public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;

	void Begin();
	void End();

private:
	void SetThemeColors();

	void CleanUp();

	ULONG refCount_;

};
