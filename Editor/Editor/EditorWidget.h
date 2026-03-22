#pragma once

class EditorWidget
	: public IUnknown
{	
	/*enum class eState
	{
		Paused,
		Active,
		Disabled,
		Destroyed
	};*/
public:
	EditorWidget();
	virtual ~EditorWidget();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;

	//virtual void Init();
	virtual void Update() = 0;
	virtual void Render() = 0;
	/*virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnDestroy();*/

private:
	virtual void CleanUp();

	ULONG refCount_;

};