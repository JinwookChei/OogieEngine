#pragma once

class ImGuiTextureWidget : public ImGuiWidget
{
public:
	ImGuiTextureWidget() = delete;
	ImGuiTextureWidget(const char* text);

	~ImGuiTextureWidget() override;

	void Render() override;

	bool BindTexture(ImTextureID* pSRV);

private:
	void CleanUp() override;

private:
	ImTextureID* pSRV_;

	const char* text_;
};

