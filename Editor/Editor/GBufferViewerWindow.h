#pragma once

class GBufferViewerWindow
	: public EditorWindow
{
public:
	GBufferViewerWindow();
	~GBufferViewerWindow() override;

	static GBufferViewerWindow* GGBufferViewerWindow;

	void Begin() override;
	void End() override;

protected:
	void CleanUp() override;
};
