#pragma once

class ImguiTextureWidget;
class GBufferViewer : public ImguiPanel
{
private:
	friend class ImguiManager;

	GBufferViewer();

	~GBufferViewer() override;

	static GBufferViewer* Create(IImguiBindCamera* pCamera);

	void Render() override;
private:
	void BindCamera(IImguiBindCamera* pCamera);

	void CleanUp() override;

private:
	IImguiBindCamera* pBoundCamera_;

	ImguiTextureWidget* pAlbedoWidget_;
	ImguiTextureWidget* pNormalWidget_;
	ImguiTextureWidget* pSpecularWidget_;
	ImguiTextureWidget* pDepthWidget_;
};
