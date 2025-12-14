#pragma once

typedef int ImGuiWindowFlags;      
typedef int ImGuiDockNodeFlags;    

class EditorContext;
class EditorWindow;

namespace Editor
{
	EDITOR_API IEditor* GetEditor();
}

class EditorCore
	: public IEditor
{
public:
	enum class eState
	{
		Disable,
		Active,
		Destroy,
	};

	EditorCore();
	~EditorCore();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;

	bool __stdcall Init(IApplication* pApplication, IRenderer* pRenderer, float dpiScale) override;
	bool __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	float __stdcall EnableDpiAwareness() override;

	void __stdcall OnRender() override;
	//bool __stdcall BindCamera(IImGuiBindCamera* pCamera) override;
	//bool __stdcall BindPickedActor(IImGuiBindPickedActor* pPickedActor) override;

private:
	bool InitContext(IApplication* pApplication, IRenderer* pRenderer, float dpiScale);
	bool InitWindows();

	void Update();
	void Render();

	void CleanUp();

	ULONG refCount_;
	IApplication* pApplication_;
	IRenderer* pRenderer_;
	float dpiScale_;

	EditorContext* pEditorContext_;
	std::map<std::wstring, EditorWindow*> editorWindows_;

	ImGuiWindowFlags flags_;
	ImGuiDockNodeFlags dockspaceFlags_;
	eState state_;
	bool fullScreen_;
	Float2 viewportSize_;
	bool viewportFocused_;
	bool viewportHovered_;
	int guizmoType_;


public:
	// TEMP
	bool __stdcall BindCamera(IEditorBindCamera* pCamera) override;

	bool __stdcall BindPickedActor(IEditorBindPickedActor* pPickedActor) override;
	IEditorBindCamera* pBoundCamera_;
	IEditorBindPickedActor* pPickedActor_;
};



//
//class EditorApplication
//{
//public:
//	enum class eState
//	{
//		Disable,
//		Active,
//		Destroy,
//	};
//
//	template <typename T>
//	T* GetWindow(const std::wstring& name)
//	{
//		auto iter = EditorWindows.find(name);
//		if (iter == EditorWindows.end())
//			return nullptr;
//
//		return dynamic_cast<T*>(iter->second);
//	}
//
//	///<summary>
//	///에디터를 초기화합니다.
//	///</summary>
//	static bool Initialize();
//
//	/// <summary>
//	/// 에디터를 로직을 업데이트합니다.
//	/// </summary>
//	static void Update();
//
//	/// <summary>
//	/// 에디터 렌더링을 업데이트합니다.
//	/// </summary>
//	static void OnGUI();
//
//	/// <summary>
//	/// 에디터 업데이트
//	/// </summary>
//	static void Run();
//
//	/// <summary>
//	/// 에디터를 종료합니다.
//	/// </summary>
//	static void Release();
//
//	/// <summary>
//	/// EditorApplication 이벤트 처리
//	/// </summary>
//	/*static void OnEvent(ya::Event& e);
//
//	static void OpenProject();
//	static void NewScene();
//	static void SaveScene();
//	static void SaveSceneAs();
//	static void OpenScene(const std::filesystem::path& path);
//	static void OnImGuiRender();*/
//
//	/// <summary>
//	/// Event Callback
//	/// </summary>
//	/*static void SetKeyPressed(int keyCode, int scancode, int action, int mods);
//	static void SetCursorPos(double x, double y);
//	static bool OnKeyPressed(ya::KeyPressedEvent& e);*/
//
//	/// <summary>
//	/// Getter and Setter
//	/// </summary>
//	/*static void SetGuizmoType(int type) { GuizmoType = type; }
//	static ImguiEditor* GetImguiEditor() { return ImguiEditor; }*/
//
//private:
//	//static ImguiEditor* ImguiEditor;
//
//	//static std::map<std::wstring, EditorWindow*> EditorWindows;
//	//static ImGuiWindowFlags Flag;
//	//static ImGuiDockNodeFlags DockspaceFlags;
//	//static eState State;
//	//static bool FullScreen;
//	////static ya::math::Vector2 ViewportBounds[2];
//	//static ya::math::Vector2 ViewportSize;
//	//static bool ViewportFocused;
//	//static bool ViewportHovered;
//	//static int GuizmoType;
//
//	//static ya::graphics::RenderTarget* FrameBuffer;
//	//static ya::EventCallbackFn EventCallback;
//};
//};