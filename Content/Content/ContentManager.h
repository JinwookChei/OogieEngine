#pragma once

struct IEngineContainer
{
	virtual Engine* GetEngine() = 0;
};

class ContentManager final
	: public IStartup,
	public IEngineContainer
{
public:
	ContentManager();

	virtual ~ContentManager();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool  __stdcall Start() override;

	void  __stdcall End() override;

	bool Initialize
	(
		HINSTANCE hInstance,
		PWSTR pCmdLine,
		int cmdShow,
		const wchar_t* pMainWindowClassName,
		const wchar_t* pMainWindowText,
		const wchar_t* pIConPath
	);

	void EngineLoop();

	Engine* GetEngine() override;

private:
	ULONG refCount_;

	Engine* pEngine_;
};