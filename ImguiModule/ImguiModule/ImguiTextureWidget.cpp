#include "stdafx.h"
#include "ImguiWidget.h"
#include "ImguiTextureWidget.h"

ImguiTextureWidget::ImguiTextureWidget(ImTextureID* pSRV, std::string text)
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
	ImGui::Text(text_.c_str());
	ImGui::Image(pSRV_, ImVec2(256, 256));
	ImGui::End();
}

void ImguiTextureWidget::CleanUp()
{
}
