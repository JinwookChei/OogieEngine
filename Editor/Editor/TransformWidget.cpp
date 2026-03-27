#include "stdafx.h"
#include "EditorWidget.h"
#include "TransformWidget.h"

ImFont* g_BoldFont = nullptr;

bool DrawVec3Control(const std::string& label, float values[3], float resetValue = 0.0f)
{
    bool modified = false;

    ImGui::PushID(label.c_str());

    // 테이블 Row
    ImGui::TableNextRow();

    // ---------------- Label ----------------
    ImGui::TableSetColumnIndex(0);

    // 가운데 정렬
    float columnWidth = ImGui::GetColumnWidth();
    float textWidth = ImGui::CalcTextSize(label.c_str()).x;
    float cursorX = ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f;
    ImGui::SetCursorPosX(cursorX);

    // Text
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label.c_str());

    // ---------------- Value ----------------
    ImGui::TableSetColumnIndex(1);
    ImGui::PushMultiItemsWidths(3, ImGui::GetContentRegionAvail().x);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4, 0 });

    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight, lineHeight };
    float fullWidth = ImGui::GetContentRegionAvail().x;
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float itemWidth = (fullWidth - (buttonSize.x * 3) - (spacing * 2)) / 3.0f;

    // ---------- X ----------
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 50, 50, 255));
    if (ImGui::Button("X", buttonSize))
    {
        //values[0] = resetValue;
        //modified = true;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(itemWidth);
    if (ImGui::DragFloat("##X", &values[0], 0.0f))
    {
        modified = true;
    }
    ImGui::SameLine();

    // ---------- Y ----------
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 200, 50, 255));
    if (ImGui::Button("Y", buttonSize))
    {
        //values[1] = resetValue;
        //modified = true;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(itemWidth);
    if (ImGui::DragFloat("##Y", &values[1], 0.0f))
    {
        modified = true;
    }

    ImGui::SameLine();

    // ---------- Z ----------
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 100, 200, 255));
    if (ImGui::Button("Z", buttonSize))
    {
        //values[2] = resetValue;
        //modified = true;
    }
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::SetNextItemWidth(itemWidth);
    if (ImGui::DragFloat("##Z", &values[2], 0.0f))
    {
        modified = true;
    }

    ImGui::PopStyleVar();
    ImGui::PopItemWidth(); // 3개 정리

    ImGui::PopID();

    return modified;
}

TransformWidget::TransformWidget()
{
}

TransformWidget::~TransformWidget()
{
	CleanUp();
}

void TransformWidget::Update()
{
}

void TransformWidget::Render()
{
    if (g_BoldFont == nullptr)
    {
        g_BoldFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 24.0f);
    }

    if (GPickedActor)
    {
        ImGui::PushFont(g_BoldFont);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 6));
        ImGui::Columns(2, nullptr, false);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Name");
        ImGui::NextColumn();

        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(60, 60, 65, 255));
        ImGui::Button("Hello", ImVec2(-FLT_MIN, 0));
        ImGui::PopStyleColor();

        ImGui::Columns(1);
        ImGui::PopStyleVar(2);
        ImGui::PopFont();


        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            IEditorBindTransform& transform = GPickedActor->GetTransformForEditor();
            Float4& actorLoc = transform.GetActorPositionForEditor();
            Float4& actorRot = transform.GetActorRotationForEditor();
            Float4& actorScale = transform.GetActorScaleForEditor();
            float loc[3] = { actorLoc.X, actorLoc.Y, actorLoc.Z };
            float rot[3] = { actorRot.X, actorRot.Y, actorRot.Z };
            float scale[3] = { actorScale.X, actorScale.Y, actorScale.Z };
                        
            //pTransform->TransformUpdateForEditor();

            // ---------- Table 시작 ----------
            if (ImGui::BeginTable("TransformTable", 2,
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_SizingStretchSame |
                ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

                bool updateFlag = false;
                if (DrawVec3Control("Location", loc, 0.0f))
                {
                    /*actorLoc.X = loc[0];
                    actorLoc.Y = loc[1];
                    actorLoc.Z = loc[2];
                    actorLoc.W = 1.0f;*/
                    transform.SetActorPositionForEditor({ loc[0], loc[1], loc[2], 1.0f });
                    updateFlag = true;
                    
                }
                if (DrawVec3Control("Rotation", rot, 0.0f))
                {
                    /*actorRot.X = rot[0];
                    actorRot.Y = rot[1];
                    actorRot.Z = rot[2];
                    actorRot.W = 0.0f;*/
                    transform.SetActorRotationForEditor({ rot[0] , rot[1], rot[2], 0.0f });
                    updateFlag = true;
                }
                if (DrawVec3Control("Scale", scale, 1.0f))
                {
                    /*actorScale.X = scale[0];
                    actorScale.Y = scale[1];
                    actorScale.Z = scale[2];
                    actorScale.W = 0.0f;*/
                    transform.SetActorScaleForEditor({ scale[0],scale[1] ,scale[2] ,0.0f});
                    updateFlag = true;
                }
                if (updateFlag)
                {
                    transform.TransformUpdateForEditor();
                }

                ImGui::EndTable();
            }
        }
    }
    else
    {
        ImGui::Text("Select an actor to view its properties.");
    }
}

void TransformWidget::CleanUp()
{
}
