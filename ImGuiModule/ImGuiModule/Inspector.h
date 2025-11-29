#pragma once

class TransformWidget;
class Inspector : public ImGuiPanel
{
private:
	friend class ImGuiManager;
	Inspector();

	~Inspector() override;

	static Inspector* Create();

	bool Init();

	void BindPickedActor(IImGuiBindPickedActor* pPickedActor);

	void Render() override;

private:
	void CleanUp() override;

private:
	IImGuiBindPickedActor* pPickedActor_;

	TransformWidget* pTransformWidget_;
};
