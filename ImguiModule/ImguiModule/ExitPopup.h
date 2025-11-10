#pragma once

class ExitPopup : ImGuiPanel
{
	friend class ImGuiManager;

	ExitPopup();
	//ExitPopup(IApplication* pApplication);
	~ExitPopup() override;

	static ExitPopup* Create(IApplication* pApplication);

	bool Init(IApplication* pApplication);


	void Render() override;

private:
	void CleanUp() override;

private:
	IApplication* pApplication_;

	bool bShowExitPopup_;
};
