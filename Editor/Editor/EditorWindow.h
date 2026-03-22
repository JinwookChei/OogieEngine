#pragma once

class EditorWidget;

class EditorWindow 
	: public IUnknown
	, public Label
{
public:
	enum class eState
	{
		Disable,
		Active,
		Destroy,
	};

	EditorWindow();
	virtual ~EditorWindow();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;

	virtual void OnRender();
	virtual void Begin();
	virtual void End();

	//virtual void Run();
	//virtual void OnEnable();
	//virtual void OnDisable();
	//virtual void OnDestroy();
	//virtual void OnEvent(ya::Event& e) {}

	ImGuiWindowFlags GetFlag() const { return flags_; }
	eState GetState() const { return state_; }
	void SetState(eState state) { state_ = state; }
	ImVec2 GetSize() { return size_; }
	void SetSize(const ImVec2& size) { size_ = size; }

	template<typename T>
	T* CreateWidget()
	{
		EditorWidget* pNewWidgets = new T;
		widgets_.push_back(pNewWidgets);
		return static_cast<T*>(pNewWidgets);
	}

protected:
	virtual void Update();
	virtual void Render();
	virtual void CleanUp();

private:
	ULONG refCount_;
	ImGuiWindowFlags flags_;
	eState state_;	
	ImVec2 size_;

	std::vector<EditorWidget*> widgets_;
};