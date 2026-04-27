#include "stdafx.h"
#include "MaterialDetailWidget.h"

MaterialDetailWidget::MaterialDetailWidget()
{
}

MaterialDetailWidget::~MaterialDetailWidget()
{
	CleanUp();
}

void MaterialDetailWidget::Update()
{
}

void MaterialDetailWidget::Render()
{
    if (GPickedActor == nullptr) return;
    IMaterial* pMaterial = GPickedActor->GetMaterialForEditor();
    if (nullptr == pMaterial) return;

    if (ImGui::CollapsingHeader("MaterialDetail", ImGuiTreeNodeFlags_DefaultOpen))
    {

        Float3 specular = pMaterial->GetSpecularColor();
		DrawColorDetail("Specular Color", specular);
		pMaterial->SetSpecularColor(specular);

		float shineness = pMaterial->GetShineness();
		DrawValueDetail("Shineness", shineness, 0.01f, 1.0f, 0.001);
		pMaterial->SetShineness(shineness);
    }
}

void MaterialDetailWidget::DrawColorDetail(const char* label, Float3& color) const
{
	if (ImGui::BeginTable("ColorTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV))
	{
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::PushID(label);

		// Label column
		ImGui::TableSetColumnIndex(0);
		float columnWidth = ImGui::GetColumnWidth();
		float textWidth = ImGui::CalcTextSize(label).x;
		float cursorX = ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f;
		ImGui::SetCursorPosX(cursorX);
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(label);
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-1.0f);

		float c[3] = { color.X, color.Y, color.Z };
		ImGui::ColorEdit3("", c);
		color.X = c[0];
		color.Y = c[1];
		color.Z = c[2];

		ImGui::PopID();
		ImGui::EndTable();
	}
}

void MaterialDetailWidget::DrawValueDetail(const char* label, float& value, float min, float max, float speed) const
{
	if (ImGui::BeginTable("ValueTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersInnerV))
	{
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::PushID(label);

		// Label column
		ImGui::TableSetColumnIndex(0);
		float columnWidth = ImGui::GetColumnWidth();
		float textWidth = ImGui::CalcTextSize(label).x;
		float cursorX = ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f;
		ImGui::SetCursorPosX(cursorX);
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(label);

		// Value column
		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(-1.0f);

		ImGui::DragFloat("", &value, speed, min, max, "%.2f");

		ImGui::PopID();
		ImGui::EndTable();
	}
}

void MaterialDetailWidget::CleanUp()
{
}

