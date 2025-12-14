#pragma once
class EditorWidget;
class InspectorWindow
	: public EditorWindow
{
public:
	InspectorWindow();
	~InspectorWindow() override;

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

private:
	void Update() override;
	void Render() override;

	void CleanUp();
	std::vector<EditorWidget*> widgets_;
};