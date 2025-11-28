#pragma once


class TransformWidget : public ImGuiWidget
{
public:
	TransformWidget();
	~TransformWidget() override;

	void Render() override;


private:
	void CleanUp() override;

private:
	ULONG refCount_;

	bool isActive_;
};