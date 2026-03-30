#pragma once

class EditorWidget;

class SceneWindow
	: public EditorWindow
{
public:
	SceneWindow();
	~SceneWindow() override;

	static SceneWindow* GSceneWindow;

	//void Init()  override;
	//void OnUpdate() override;
	//void OnRender() override;
	//bool Init() override;
	//void Update() override;
	//void OnGUI() override;
	//void Run() override;
	//void OnEnable() override;
	//void OnDisable() override;
	//void OnDestroy() override;
	//void OnRender() override;


	void Begin() override;
	void End() override;

protected:
	void CleanUp() override;
	
private:
	//GameObject* mEditorCameraObject;
	//EditorCamera* mEditorCamera;

	Float2 ViewportBounds[2];
	Float2 ViewportSize;
	bool ViewportFocused;
	bool ViewportHovered;
	int GuizmoType;
};

//extern SceneWindow* SceneWindow::GSceneWindow;
