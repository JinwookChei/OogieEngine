#pragma once


class ImGuiWidget : public IImGuiWidget
{
public:
	ImGuiWidget();

	virtual ~ImGuiWidget();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef(void) override;

	ULONG __stdcall Release(void) override;

	virtual void Render() = 0;

	bool IsActive() const;

	void OnActive();

	void OffActive();

	//LINK_ITEM* MangerLink();

private:
	virtual void CleanUp();

private:
	ULONG refCount_;

	bool isActive_;

	//LINK_ITEM managerLink_;
};