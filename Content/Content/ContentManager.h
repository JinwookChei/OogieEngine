#pragma once

struct IEngineContainer
{
	virtual Engine* GetEngine() = 0;
};

class ContentManager final
	: public IStartup,
	public IEngineContainer {
public:
	ContentManager();

	~ContentManager();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool  __stdcall Start() override;

	void  __stdcall End() override;

	bool Initialize(HINSTANCE hInstance, PWSTR pCmdLine, int cmdShow);

	void EngineLoop();

	Engine* GetEngine() override;

private:
	ULONG refCount_;

	Engine* engine_;
};