#include "stdafx.h"
#include "EditorWidget.h"
#include "EditorWindow.h"
#include "InspectorWindow.h"

InspectorWindow::InspectorWindow()
{
	SetName("Inspector");
	SetSize(ImVec2(300, 600));
}

InspectorWindow::~InspectorWindow()
{
	CleanUp();
}

void InspectorWindow::Update()
{
	for (EditorWidget* widget : widgets_)
	{
		widget->Update();
	}
}

void InspectorWindow::Render()
{
	for (EditorWidget* editor : widgets_)
	{
		editor->Render();
	}
}

void InspectorWindow::CleanUp()
{

}
