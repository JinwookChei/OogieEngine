#pragma once

class ImGuiTextureWidget : public ImGuiWidget
{
public:
	ImGuiTextureWidget() = delete;

	ImGuiTextureWidget(ImTextureID* pSRV, const char* text);

	~ImGuiTextureWidget() override;

	void Render() override;

private:
	void CleanUp() override;

private:
	ImTextureID* pSRV_;

	const char* text_;
};

