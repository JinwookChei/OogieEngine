#include "stdafx.h"
#include "ImguiTextureWidget.h"

ImguiTextureWidget::ImguiTextureWidget(ImTextureID* pSRV, const char* text)
	: pSRV_(pSRV),
	text_(text)
{
}

ImguiTextureWidget::~ImguiTextureWidget()
{
	CleanUp();
}

void ImguiTextureWidget::Render()
{
	ImGui::Begin("G-Buffer Viewer");
	ImGui::Text(text_);
	ImGui::Image(pSRV_, ImVec2(256, 256));
	ImGui::End();
}

void ImguiTextureWidget::CleanUp()
{
}
