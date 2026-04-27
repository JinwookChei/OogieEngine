#pragma once

class MaterialDetailWidget
	: public EditorWidget
{
public:
	MaterialDetailWidget();
	~MaterialDetailWidget() override;

	void Update() override;
	void Render() override;

	void DrawColorDetail(const char* label, Float3& color) const;
	void DrawValueDetail(const char* label, float& value, float min, float max, float speed) const;
private:
	void CleanUp() override;
};
