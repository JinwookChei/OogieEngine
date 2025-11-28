#include "stdafx.h"
#include "ImGuiTextureWidget.h"


ImGuiTextureWidget::ImGuiTextureWidget(const char* text)
	: pSRV_(nullptr)
	, text_(text)
{

}

ImGuiTextureWidget::~ImGuiTextureWidget()
{
	CleanUp();
}

void ImGuiTextureWidget::Render()
{
	if (!IsActive())
	{
		return;
	}

	ImGui::Begin("G-Buffer Viewer");
	ImGui::Text(text_);
	ImGui::Image(pSRV_, ImVec2(256, 256));
	ImGui::End();
}

bool ImGuiTextureWidget::BindTexture(ImTextureID* pSRV)
{
	pSRV_ = pSRV;
	
	return true;
}

void ImGuiTextureWidget::CleanUp()
{
}
