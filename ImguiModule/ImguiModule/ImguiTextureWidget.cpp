#include "stdafx.h"
#include "ImGuiTextureWidget.h"

ImGuiTextureWidget::ImGuiTextureWidget(ImTextureID* pSRV, const char* text)
	: pSRV_(pSRV),
	text_(text)
{
}

ImGuiTextureWidget::~ImGuiTextureWidget()
{
	CleanUp();
}

void ImGuiTextureWidget::Render()
{
	ImGui::Begin("G-Buffer Viewer");
	ImGui::Text(text_);
	ImGui::Image(pSRV_, ImVec2(256, 256));
	ImGui::End();
}

void ImGuiTextureWidget::CleanUp()
{
}
