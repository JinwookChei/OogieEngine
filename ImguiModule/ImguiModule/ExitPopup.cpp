#include "stdafx.h"
#include "ExitPopup.h"

ExitPopup::ExitPopup()
	:pApplication_(nullptr),
	bShowExitPopup_(false)
{
}

ExitPopup::~ExitPopup()
{
	CleanUp();
}

ExitPopup* ExitPopup::Create(IApplication* pApplication)
{
	ExitPopup* pNewImguiPanel = new ExitPopup;
	pNewImguiPanel->Init(pApplication);

	return pNewImguiPanel;
}

bool ExitPopup::Init(IApplication* pApplication)
{
	if (nullptr == pApplication)
	{
		Assert("Application is NULL");
		return false;
	}

	pApplication_ = pApplication;
	pApplication_->AddRef();

	return true;
}

void ExitPopup::Render()
{
	if (false == IsActive())
	{
		return;
	}

	// ESC 키 감지
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
	{
		bShowExitPopup_ = true;
		ImGui::OpenPopup("Exit Program");
	}
	// 종료 팝업
	if (bShowExitPopup_)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 150));
		if (ImGui::BeginPopupModal("Exit Program", NULL, ImGuiWindowFlags_NoResize))
		{
			ImGui::Text(" Do you want to exit the program ? ");
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Quit", ImVec2(120, 0)))
			{
				pApplication_->Quit();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				bShowExitPopup_ = false;
			}

			ImGui::EndPopup();
		}
	}
}

void ExitPopup::CleanUp()
{
	if (nullptr != pApplication_)
	{
		pApplication_->Release();
		pApplication_ = nullptr;
	}
}
