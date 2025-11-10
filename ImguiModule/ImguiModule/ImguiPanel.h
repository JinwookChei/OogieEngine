#pragma once

class ImGuiPanel : public IImGuiPanel
{
public:
	ImGuiPanel();
	virtual ~ImGuiPanel();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef(void) override;

	ULONG __stdcall Release(void) override;

	bool IsActive() const;

	void OnActive();

	void OffActive();

	virtual void Render() = 0;

private:
	virtual void CleanUp();

private:
	ULONG refCount_;

	bool isActive_;
};
