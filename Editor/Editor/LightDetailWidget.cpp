#include "stdafx.h"
#include "LightDetailWidget.h"

LightDetailWidget::LightDetailWidget()
{
}

LightDetailWidget::~LightDetailWidget()
{
	CleanUp();
}

void LightDetailWidget::Update()
{
}

void LightDetailWidget::Render()
{
	if (GPickedActor == nullptr) return;
	if (GPickedActor->GetActorTypeForEditor() != E_ACTOR_TYPE::LIGHT) return;

	// 0 - Direction
	if (ImGui::CollapsingHeader("LightDetail", ImGuiTreeNodeFlags_DefaultOpen))
	{
		Color diffuse = GPickedActor->GetDiffuseColorForEditor();
		//Color ambient = GPickedActor->GetAmbientColorForEditor();
		//Color specular = GPickedActor->GetSpecularColorForEditor();
		DrawColorDetail("LightColor", diffuse);
		//DrawColorDetail("AmbientColor", ambient);
		//DrawColorDetail("SpecularColor", specular);
		GPickedActor->SetDiffuseColorForEditor(diffuse);
		//GPickedActor->SetAmbientColorForEditor(ambient);
		//GPickedActor->SetSpecularColorForEditor(specular);

		float intensity = GPickedActor->GetLightIntensity();
		DrawValueDetail("Intensity", intensity, 0.0f, 1.0f, 0.01f);
		GPickedActor->SetLightIntensity(intensity);

		// 0 - Direction
		if (GPickedActor->GetLightTypeForEditor() == 0)
		{
		}
		// 1 - Spot
		else if (GPickedActor->GetLightTypeForEditor() == 1)
		{
			float range = GPickedActor->GetLightRange();
			DrawValueDetail("Range", range, 0.0f, 1000.0f, 1.0f);
			GPickedActor->SetLightRange(range);

			float innerAngle = GPickedActor->GetLightInnerAngle();
			DrawValueDetail("InnerAngle", innerAngle, 0.0f, 180.0f, 1.0f);
			GPickedActor->SetLightInnerAngle(innerAngle);

			float outerAngle = GPickedActor->GetLightOuterAngle();
			DrawValueDetail("OuterAngle", outerAngle, 0.0f, 180.0f, 1.0f);
			GPickedActor->SetLightOuterAngle(outerAngle);

			float smooth = GPickedActor->GetLightSmooth();
			DrawValueDetail("Smooth", smooth, 1.0f, 512.0f, 1.0f);
			GPickedActor->SetLightSmooth(smooth);
		}
		// 2 - Point
		else if (GPickedActor->GetLightTypeForEditor() == 2)
		{
			float range = GPickedActor->GetLightRange();
			DrawValueDetail("Range", range, 0.0f, 1000.0f, 1.0f);
			GPickedActor->SetLightRange(range);
		}
		else
		{
			DEBUG_BREAK();
		}
	}
}

void LightDetailWidget::DrawColorDetail(const char* label, Color& color) const
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

void LightDetailWidget::DrawValueDetail(const char* label, float& value, float min, float max, float speed) const
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

void LightDetailWidget::CleanUp()
{
}
