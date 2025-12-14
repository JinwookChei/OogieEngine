#pragma once

class EditorWidget;

class SceneWindow
	: public EditorWindow
{
public:
	SceneWindow();
	~SceneWindow() override;

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

	void OnRender() override;
	void Begin() override;
	void End() override;

private:
	void Update() override;
	void Render() override;

	void CleanUp();
	std::vector<EditorWidget*> widgets_;

	//GameObject* mEditorCameraObject;
	//EditorCamera* mEditorCamera;

	Float2 ViewportBounds[2];
	Float2 ViewportSize;
	bool ViewportFocused;
	bool ViewportHovered;
	int GuizmoType;
};