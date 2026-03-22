#pragma once

class TransformWidget
	: public EditorWidget
{
public:
	TransformWidget();
	~TransformWidget() override;

	void Update() override;
	void Render() override;

private:
	void CleanUp() override;
};
