#pragma once


class LightDetailWidget
	: public EditorWidget
{
public:
	LightDetailWidget();
	~LightDetailWidget() override;

	void Update() override;
	void Render() override;

	void DrawColorDetail(const char* label, Color& color) const;
	void DrawValueDetail(const char* label, float& value, float min, float max, float speed) const;

private:
	void CleanUp() override;
};
