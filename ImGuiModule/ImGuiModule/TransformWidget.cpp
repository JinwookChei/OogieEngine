#include "stdafx.h"
#include "TransformWidget.h"

TransformWidget::TransformWidget()
	:pPickedActor_(nullptr)
{
}

TransformWidget::~TransformWidget()
{
}

void TransformWidget::Render()
{
	if (!IsActive())
	{
		return;
	}

    // Imgui 윈도우 시작
    ImGui::Begin("Actor Properties");

    if (pPickedActor_)
    {
        IImGuiBindTransform* pPickedTransform = pPickedActor_->GetTransformForImGui();
        Float4* scale = pPickedTransform->GetActorScaleForImGui();
        Float4* rotation = pPickedTransform->GetActorRotationForImGui();
        Float4* position = pPickedTransform->GetActorPositionForImGui();

        // 1. 이름 표시
        ImGui::Text("Selected Actor: %s", "ActorNameNoName");
        //ImGui::Text("Selected Actor: %s", g_SelectedActorTransform.actorName);
        ImGui::Separator();

        ImGui::Text("Transform");

        if (ImGui::InputFloat3("Scale", (float*)scale))
        {
            pPickedTransform->TransformUpdateForImGui();
        }
        if (ImGui::InputFloat3("Rotation", (float*)rotation))
        {
            pPickedTransform->TransformUpdateForImGui();
        }
        if (ImGui::InputFloat3("Position", (float*)position))
        {
            pPickedTransform->TransformUpdateForImGui();
        }
    }
    else
    {
        ImGui::Text("No Actor Selected");
    }

    // Imgui 윈도우 끝
    ImGui::End();
}

void TransformWidget::BindPickedActor(IImGuiBindPickedActor* pPickedActor)
{
    pPickedActor_ = pPickedActor;
}


void TransformWidget::CleanUp()
{
}
