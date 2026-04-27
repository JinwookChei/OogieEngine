#include "stdafx.h"
#include "GBufferViewerWindow.h"

GBufferViewerWindow::GBufferViewerWindow()
{
	SetName("GBufferViewer");
	SetSize(ImVec2(300, 100));
	AddFlag(ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
	SetState(eState::Disable);
}

GBufferViewerWindow::~GBufferViewerWindow()
{
	CleanUp();
}

void GBufferViewerWindow::Begin()
{
	ImGui::SetNextWindowSize(ImVec2(1300, 400), ImGuiCond_Always);
	bool Active = (bool)GetState();
	ImGui::Begin(GetName().c_str(), &Active, GetFlag());
	if (Active == false) SetState(eState::Disable);
}

void GBufferViewerWindow::End()
{
	void* pFinal = GBoundCamera->GetFinalRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo);
	void* pAlbedo = GBoundCamera->GetGBufferRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Albedo);
	void* pNormal = GBoundCamera->GetGBufferRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Normal);
	void* pSpecular = GBoundCamera->GetGBufferRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Specular);
	void* pPosition = GBoundCamera->GetGBufferRenderTargetForEditor()->GetShaderResourceView(E_RENDER_TEXTURE_TYPE::Position);

	auto DrawTexture = [](const char* name, void* texID)
		{
			ImGui::BeginGroup();
			ImGui::Text("%s", name);
			if (ImGui::ImageButton(name, (ImTextureID)texID, ImVec2(400, 400)))
			{
				GCurrentRenderResource = (ImTextureID*)texID;
			}
			ImGui::EndGroup();
		};

	DrawTexture("Final", pFinal);
	ImGui::SameLine();
	DrawTexture("Albedo", pAlbedo);
	ImGui::SameLine();
	DrawTexture("Normal", pNormal);
	ImGui::SameLine();
	DrawTexture("Specular", pSpecular);
	ImGui::SameLine();
	DrawTexture("Position", pPosition);

	ImGui::End();
}

void GBufferViewerWindow::CleanUp()
{
}
