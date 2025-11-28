#pragma once

class TransformWidget;
class Inspector : public ImGuiPanel
{
public:
	Inspector();

	~Inspector() override;

	static Inspector* Create(IImGuiBindPickedActor* pPickedActor);

	void Render() override;
private:
	void BindPickedActor(IImGuiBindPickedActor* pPickedActor);

	void CleanUp() override;

private:
	IImGuiBindPickedActor* pPickedActor_;

	TransformWidget* pTransformWidget_;
};
