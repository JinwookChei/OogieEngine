#pragma once


class TransformWidget : public ImGuiWidget
{
public:
	TransformWidget();
	~TransformWidget() override;

	void Render() override;

	void BindPickedActor(IImGuiBindPickedActor* pPickedActor);

private:
	void CleanUp() override;

	//Float4x4* pTransform_;
	IImGuiBindPickedActor* pPickedActor_;
};