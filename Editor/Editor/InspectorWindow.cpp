#include "stdafx.h"
#include "TransformWidget.h"
#include "EditorWidget.h"
#include "EditorWindow.h"
#include "InspectorWindow.h"

InspectorWindow::InspectorWindow()
{
	SetName("Inspector");
	SetSize(ImVec2(300, 600));
	pTransformWidget_ = CreateWidget<TransformWidget>();
}

InspectorWindow::~InspectorWindow()
{
	CleanUp();
}

void InspectorWindow::Begin()
{
	bool Active = (bool)GetState();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin(GetName().c_str(), &Active, GetFlag());
}

void InspectorWindow::End()
{
	ImGui::End();
	ImGui::PopStyleVar();
}

void InspectorWindow::CleanUp()
{

}
