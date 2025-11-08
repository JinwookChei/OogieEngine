#pragma once

class ImguiTextureWidget : public ImguiWidget
{
public:
	ImguiTextureWidget() = delete;

	ImguiTextureWidget(ImTextureID* pSRV, const char* text);

	~ImguiTextureWidget() override;

	void Render() override;

private:
	void CleanUp() override;

private:
	ImTextureID* pSRV_;

	const char* text_;
};

