#pragma once

class ImguiTextureWidget : public ImguiWidget
{
public:
	ImguiTextureWidget() = delete;

	ImguiTextureWidget(ImTextureID* pSRV, std::string text);

	~ImguiTextureWidget() override;

	void Render() override;

private:
	void CleanUp() override;

private:
	ImTextureID* pSRV_;

	std::string text_;
};

