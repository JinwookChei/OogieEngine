#pragma once

class ImGuiTextureWidget;
class GBufferViewer : public ImGuiPanel
{
private:
	friend class ImGuiManager;

	GBufferViewer();

	~GBufferViewer() override;

	static GBufferViewer* Create(IImGuiBindCamera* pCamera);

	void Render() override;
private:
	void BindCamera(IImGuiBindCamera* pCamera);

	void CleanUp() override;

private:
	IImGuiBindCamera* pBoundCamera_;

	ImGuiTextureWidget* pAlbedoWidget_;
	ImGuiTextureWidget* pNormalWidget_;
	ImGuiTextureWidget* pSpecularWidget_;
	ImGuiTextureWidget* pDepthWidget_;
};
